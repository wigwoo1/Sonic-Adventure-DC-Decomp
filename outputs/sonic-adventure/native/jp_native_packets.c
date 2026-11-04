/* Layout facts: pinned external sdk/include/kamui.h KMVERTEX0..8 and
 * sdk/ninja/kmvertex_kamui_lib.c header type/word tables. Authored parser.
 * Does not replace any frozen producer, queue or frame owner.
 */
#include "jp_native_packets.h"
#include <stdlib.h>
#include <string.h>

struct counts { size_t batches,vertices,triangles; };
static uint32_t word(const unsigned char *p,unsigned i)
{
    p+=(size_t)i*4;
    return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
static float number(uint32_t bits) { float f;memcpy(&f,&bits,4);return f; }
static int finite(float f) { uint32_t b;memcpy(&b,&f,4);return (b&0x7f800000)!=0x7f800000; }
static int fail(struct jp_packet_failure *f,int code,unsigned list,size_t offset)
{ if(f){f->code=code;f->list=list;f->offset=offset;}return code; }
static void packed(float out[4],uint32_t c)
{
    out[0]=(float)((c>>16)&255)/255.0f;out[1]=(float)((c>>8)&255)/255.0f;
    out[2]=(float)(c&255)/255.0f;out[3]=(float)(c>>24)/255.0f;
}
static int color(float out[4],const unsigned char *p,unsigned first)
{
    out[3]=number(word(p,first));
    for(unsigned i=0;i<3;i++)out[i]=number(word(p,first+1+i));
    for(unsigned i=0;i<4;i++)if(!finite(out[i]))return 0;
    return 1;
}
static int vertex(struct jp_packet_vertex *v,const unsigned char *p,unsigned type,
                  unsigned offset,const float face[4],const float spec[4])
{
    memset(v,0,sizeof *v);v->base_intensity=v->offset_intensity=1;
    v->x=number(word(p,1));v->y=number(word(p,2));v->inv_w=number(word(p,3));
    if(!finite(v->x)||!finite(v->y)||!finite(v->inv_w))return 0;
    if(type>=3){
        if(type==4||type==6||type==8){
            uint32_t uv=word(p,4);v->u=number(uv&0xffff0000);v->v=number(uv<<16);
        }else{v->u=number(word(p,4));v->v=number(word(p,5));}
        if(!finite(v->u)||!finite(v->v))return 0;
    }
    if(type==0||type==3||type==4){
        packed(v->base,word(p,6));if(type && offset)packed(v->offset,word(p,7));
    }else if(type==1||type==5||type==6){
        if(!color(v->base,p,type==1?4:8))return 0;
        if(type!=1 && offset && !color(v->offset,p,12))return 0;
    }else{
        memcpy(v->base,face,sizeof v->base);v->base_intensity=number(word(p,6));
        if(type>=3 && offset){memcpy(v->offset,spec,sizeof v->offset);v->offset_intensity=number(word(p,7));}
        if(!finite(v->base_intensity)||!finite(v->offset_intensity))return 0;
    }
    return 1;
}
static int increment(size_t *n) { if(*n==SIZE_MAX)return 0;++*n;return 1; }
static int parse(const struct jp_packet_span lists[5],struct counts *count,
    struct jp_packet_plan *plan,struct jp_packet_failure *failure)
{
    static const unsigned list_codes[5]={0,2,4,1,3};
    struct jp_packet_batch *batches=plan?(struct jp_packet_batch *)plan->batches:NULL;
    struct jp_packet_vertex *vertices=plan?(struct jp_packet_vertex *)plan->vertices:NULL;
    struct jp_packet_triangle *triangles=plan?(struct jp_packet_triangle *)plan->triangles:NULL;
    memset(count,0,sizeof *count);
    for(unsigned li=0;li<5;li++){
        const unsigned char *data=lists[li].data;size_t size=lists[li].bytes,at=0,strip=0;
        unsigned type=0,offset=0,have_header=0,have_face=0,have_spec=0;
        float face[4]={0},spec[4]={0};struct jp_packet_batch *batch=NULL;
        if((size && !data)||size%32)return fail(failure,JP_PACKET_ARGUMENT,li,0);
        if(size && li>=3)return fail(failure,JP_PACKET_UNSUPPORTED,li,0);
        while(at<size){
            const unsigned char *p=data+at;uint32_t control=word(p,0);
            if((control&0xe0000000)==0x80000000){
                if(strip)return fail(failure,JP_PACKET_SEQUENCE,li,at);
                if(((control>>24)&7)!=list_codes[li])return fail(failure,JP_PACKET_SEQUENCE,li,at);
                if(control&0x40)return fail(failure,JP_PACKET_UNSUPPORTED,li,at);
                unsigned ct=(control>>4)&3,textured=(control>>3)&1,uv16=control&1;
                offset=(control>>2)&1;
                unsigned length=ct==2 && offset?64:32;
                if(length>size-at)return fail(failure,JP_PACKET_TRUNCATED,li,at);
                if(ct==2){
                    if(!color(face,p,offset?8:4))return fail(failure,JP_PACKET_NONFINITE,li,at);
                    have_face=1;
                    if(offset){if(!color(spec,p,12))return fail(failure,JP_PACKET_NONFINITE,li,at);have_spec=1;}
                }else if(ct==3 && (!have_face || (offset && !have_spec)))
                    return fail(failure,JP_PACKET_SEQUENCE,li,at);
                type=textured?(ct==0?3:ct==1?5:7)+uv16:ct==0?0:ct==1?1:2;
                if(plan){
                    if(count->batches>=plan->batch_count)return fail(failure,JP_PACKET_SEQUENCE,li,at);
                    batch=&batches[count->batches];memset(batch,0,sizeof *batch);
                    for(unsigned i=0;i<length/4;i++)batch->header[i]=word(p,i);
                    batch->header_words=length/4;batch->list=li;batch->vertex_type=type;
                    batch->source_offset=at;batch->first_vertex=count->vertices;batch->first_triangle=count->triangles;
                }
                if(!increment(&count->batches))return fail(failure,JP_PACKET_OVERFLOW,li,at);
                have_header=1;at+=length;
            }else if(control==0xe0000000 || control==0xf0000000){
                if(!have_header)return fail(failure,JP_PACKET_SEQUENCE,li,at);
                unsigned length=(type==5||type==6)?64:32;
                if(length>size-at)return fail(failure,JP_PACKET_TRUNCATED,li,at);
                struct jp_packet_vertex v;
                if(!vertex(&v,p,type,offset,face,spec))return fail(failure,JP_PACKET_NONFINITE,li,at);
                if(plan){
                    if(count->vertices>=plan->vertex_count)return fail(failure,JP_PACKET_SEQUENCE,li,at);
                    vertices[count->vertices]=v;++batch->vertex_count;
                }
                if(strip>=2){
                    if(plan){
                        if(count->triangles>=plan->triangle_count)return fail(failure,JP_PACKET_SEQUENCE,li,at);
                        struct jp_packet_triangle *t=&triangles[count->triangles];
                        t->indices[0]=count->vertices-2+(strip&1);
                        t->indices[1]=count->vertices-1-(strip&1);
                        t->indices[2]=count->vertices;++batch->triangle_count;
                    }
                    if(!increment(&count->triangles))return fail(failure,JP_PACKET_OVERFLOW,li,at);
                }
                if(!increment(&count->vertices)||!increment(&strip))return fail(failure,JP_PACKET_OVERFLOW,li,at);
                if(control==0xf0000000)strip=0;
                at+=length;
            }else if((control&0xe0000000)==0xa0000000 || (control&0xe0000000)==0x20000000)
                return fail(failure,JP_PACKET_UNSUPPORTED,li,at);
            else return fail(failure,JP_PACKET_COMMAND,li,at);
        }
        if(strip)return fail(failure,JP_PACKET_SEQUENCE,li,size);
    }
    return JP_PACKET_OK;
}
static int add_array(size_t *cost,size_t count,size_t width)
{ if(count>(SIZE_MAX-*cost)/width)return 0;*cost+=count*width;return 1; }
int jp_packets_decode(const struct jp_packet_span lists[5],size_t budget,
    struct jp_packet_plan **out,struct jp_packet_failure *failure)
{
    if(!lists||!out)return fail(failure,JP_PACKET_ARGUMENT,0,0);
    struct counts c,actual;int result=parse(lists,&c,NULL,failure);if(result)return result;
    size_t cost=sizeof(struct jp_packet_plan);
    if(!add_array(&cost,c.batches,sizeof(struct jp_packet_batch)) ||
       !add_array(&cost,c.vertices,sizeof(struct jp_packet_vertex)))return fail(failure,JP_PACKET_OVERFLOW,0,0);
    size_t padding=(_Alignof(struct jp_packet_triangle)-cost%_Alignof(struct jp_packet_triangle))%_Alignof(struct jp_packet_triangle);
    if(padding>SIZE_MAX-cost)return fail(failure,JP_PACKET_OVERFLOW,0,0);
    cost+=padding;size_t triangle_offset=cost;
    if(!add_array(&cost,c.triangles,sizeof(struct jp_packet_triangle)))return fail(failure,JP_PACKET_OVERFLOW,0,0);
    if(cost>budget)return fail(failure,JP_PACKET_BUDGET,0,0);
    struct jp_packet_plan *plan=malloc(cost);if(!plan)return fail(failure,JP_PACKET_ALLOCATION,0,0);
    plan->allocation_bytes=cost;plan->batch_count=c.batches;plan->vertex_count=c.vertices;plan->triangle_count=c.triangles;
    plan->batches=(const struct jp_packet_batch *)(plan+1);
    plan->vertices=(const struct jp_packet_vertex *)(plan->batches+c.batches);
    plan->triangles=(const struct jp_packet_triangle *)((const unsigned char *)plan+triangle_offset);
    result=parse(lists,&actual,plan,failure);
    if(result || memcmp(&c,&actual,sizeof c)){
        free(plan);return result?result:fail(failure,JP_PACKET_SEQUENCE,0,0);
    }
    *out=plan;return fail(failure,JP_PACKET_OK,0,0);
}
void jp_packets_destroy(struct jp_packet_plan *plan) { free(plan); }
