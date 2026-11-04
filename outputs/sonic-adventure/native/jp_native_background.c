#include "jp_native_background.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
static float scalar(uint32_t word){float f;memcpy(&f,&word,4);return f;}
static void color(uint32_t word,float out[4])
{out[0]=(float)((word>>16)&255)/255;out[1]=(float)((word>>8)&255)/255;out[2]=(float)(word&255)/255;out[3]=(float)(word>>24)/255;}
static int room(size_t *at,size_t count,size_t width,size_t budget)
{if(*at>budget||count>(budget-*at)/width)return 0;*at+=count*width;return 1;}
int jp_background_prepend(const struct jp_video_snapshot *s,const struct jp_packet_plan *old,size_t budget,struct jp_packet_plan **out)
{
    if(!s||!old||!out||!s->logical_width||!s->logical_height||s->logical_width>8192||s->logical_height>8192)return JP_PACKET_ARGUMENT;
    const uint32_t *p=s->background_packet;uint32_t isp=p[0],tsp=p[1],tcw=p[2];
    int textured=!!(isp&0x02000000),offset=textured&&!!(isp&0x01000000),uv16=!!(isp&0x00400000);
    size_t stride=4+(textured?(uv16?1:2)+offset:0),words=3+3*stride;
    if(s->background_bytes!=words*4 || words>62)return JP_PACKET_TRUNCATED;
    /* Low ISP bits are reserved in the captured background format. */
    if(isp&0x000fffff)return JP_PACKET_UNSUPPORTED;
    struct jp_packet_vertex v[3];memset(v,0,sizeof v);
    for(unsigned i=0;i<3;i++){
        size_t at=3+i*stride;v[i].x=scalar(p[at++]);v[i].y=scalar(p[at++]);v[i].inv_w=scalar(p[at++]);
        if(!isfinite(v[i].x)||!isfinite(v[i].y)||!isfinite(v[i].inv_w)||v[i].inv_w<=0)return JP_PACKET_NONFINITE;
        if(textured){
            if(uv16){uint32_t uv=p[at++];v[i].u=scalar(uv&0xffff0000);v[i].v=scalar(uv<<16);}
            else{v[i].u=scalar(p[at++]);v[i].v=scalar(p[at++]);}
            if(!isfinite(v[i].u)||!isfinite(v[i].v))return JP_PACKET_NONFINITE;
        }
        color(p[at++],v[i].base);if(offset)color(p[at++],v[i].offset);v[i].base_intensity=v[i].offset_intensity=1;
    }
    double ax=v[1].x-v[0].x,ay=v[1].y-v[0].y,bx=v[2].x-v[0].x,by=v[2].y-v[0].y,det=ax*by-ay*bx;
    if(!isfinite(det)||det==0)return JP_PACKET_UNSUPPORTED;
    if(old->batch_count==SIZE_MAX||old->vertex_count>SIZE_MAX-4||old->triangle_count>SIZE_MAX-2)return JP_PACKET_OVERFLOW;
    size_t nb=old->batch_count+1,nv=old->vertex_count+4,nt=old->triangle_count+2,cost=sizeof(struct jp_packet_plan);
    if(!room(&cost,nb,sizeof(struct jp_packet_batch),budget)||!room(&cost,nv,sizeof(struct jp_packet_vertex),budget))return JP_PACKET_BUDGET;
    size_t align=_Alignof(struct jp_packet_triangle),pad=(align-cost%align)%align;
    if(!room(&cost,pad,1,budget))return JP_PACKET_BUDGET;size_t triangles_at=cost;
    if(!room(&cost,nt,sizeof(struct jp_packet_triangle),budget)||old->allocation_bytes>budget-cost)return JP_PACKET_BUDGET;
    struct jp_packet_vertex corners[4];memset(corners,0,sizeof corners);
    for(unsigned i=0;i<4;i++){
        struct jp_packet_vertex *c=&corners[i];c->x=(i&1)?(float)s->logical_width:0;c->y=(i&2)?(float)s->logical_height:0;
        double x=(double)c->x-v[0].x,y=(double)c->y-v[0].y,w[3];w[1]=(x*by-y*bx)/det;w[2]=(ax*y-ay*x)/det;w[0]=1-w[1]-w[2];
        double q=0,uq=0,vq=0;for(unsigned j=0;j<3;j++){double f=w[j]*v[j].inv_w;q+=f;uq+=f*v[j].u;vq+=f*v[j].v;}
        if(!isfinite(q)||q<1e-20||q>1e20)return JP_PACKET_NONFINITE;
        c->inv_w=(float)q;c->u=(float)(uq/q);c->v=(float)(vq/q);c->base_intensity=c->offset_intensity=1;
        for(unsigned ch=0;ch<4;ch++){
            double base=0,extra=0;for(unsigned j=0;j<3;j++){double f=w[j]*v[j].inv_w;base+=f*v[j].base[ch];extra+=f*v[j].offset[ch];}
            c->base[ch]=(float)(base/q);c->offset[ch]=(float)(extra/q);
            if(!isfinite(c->base[ch])||!isfinite(c->offset[ch]))return JP_PACKET_NONFINITE;
        }
        if(!isfinite(c->u)||!isfinite(c->v))return JP_PACKET_NONFINITE;
    }
    struct jp_packet_plan *plan=malloc(cost);if(!plan)return JP_PACKET_ALLOCATION;
    plan->allocation_bytes=cost;plan->batch_count=nb;plan->vertex_count=nv;plan->triangle_count=nt;
    struct jp_packet_batch *b=(void *)(plan+1);struct jp_packet_vertex *vertices=(void *)(b+nb);struct jp_packet_triangle *triangles=(void *)((unsigned char *)plan+triangles_at);
    plan->batches=b;plan->vertices=vertices;plan->triangles=triangles;memset(b,0,sizeof *b);
    b->header[0]=0x80000002|(textured?8:0)|(offset?4:0);b->header[1]=isp;b->header[2]=tsp;b->header[3]=tcw;b->header_words=8;b->vertex_type=textured?3:0;b->vertex_count=4;b->triangle_count=2;
    memcpy(vertices,corners,sizeof corners);triangles[0]=(struct jp_packet_triangle){{0,1,2}};triangles[1]=(struct jp_packet_triangle){{1,3,2}};
    for(size_t i=0;i<old->batch_count;i++){b[i+1]=old->batches[i];b[i+1].first_vertex+=4;b[i+1].first_triangle+=2;}
    if(old->vertex_count)memcpy(vertices+4,old->vertices,old->vertex_count*sizeof *vertices);
    for(size_t i=0;i<old->triangle_count;i++){triangles[i+2]=old->triangles[i];for(unsigned j=0;j<3;j++)triangles[i+2].indices[j]+=4;}
    *out=plan;return JP_PACKET_OK;
}
