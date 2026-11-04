/* Complete 2D polygon packet producer: native pointers, bounded queue writes,
 * exact RZ/DN1 scalar values, and the GCC replay/aspect entry boundary.
 * Original undefined packet words become zero. This does not rasterize. */
#include "jp_ninja_polygon2d.h"
#include "jp_native_queue.h"
#include "jp_ninja_scalar32_rz.h"
#include "jp_ninja_camera_math.h"
#include <string.h>
extern void present_record(enum jp_present_kind,const void *,int32_t,int32_t,float,uint32_t);
extern void njGetkmPointer(void),njSetkmPointer(void),njSetkmColorMode(uint32_t);
extern void *njStartVertexNonTexOpaque(void),*njStartVertexNonTexTrans(void);
extern void *njStartVertexTexOpaque(void),*njStartVertexTexTrans(void);
extern void njEndVertex(uint32_t *),_builtin_prefetch(void *);
static uint32_t bits(float x) { uint32_t u;memcpy(&u,&x,4);return u; }
static uint32_t uv(int16_t x) { return jp_rz_scalar32_multiply(bits((float)x),0x3b800000u); }
static uint32_t *emit(uint32_t *q,const jp_point2col *p,int i,uint32_t x,uint32_t y,uint32_t z,int texture,int end)
{
    memset(q,0,32);q[0]=end?0xf0000000u:0xe0000000u;q[1]=x;q[2]=y;q[3]=z;
    if(texture){q[4]=uv(p->tex[i].tex.u);q[5]=uv(p->tex[i].tex.v);}
    q[6]=p->col[i].color;_builtin_prefetch(q);return q+8;
}
static uint32_t *edge(uint32_t *q,const jp_point2col *p,int a,int b,uint32_t z,int texture)
{
    uint32_t ax=bits(p->p[a].x),ay=bits(p->p[a].y),bx=bits(p->p[b].x),by=bits(p->p[b].y);
    uint32_t dx=jp_rz_scalar32_subtract(bx,ax),dy=jp_rz_scalar32_subtract(by,ay);
    uint32_t length=jp_rz_camera_sqrt(jp_rz_scalar32_add(jp_rz_scalar32_multiply(dx,dx),jp_rz_scalar32_multiply(dy,dy)));
    uint32_t inv=jp_rz_scalar32_divide(0x3f800000u,length);
    uint32_t sx=jp_rz_scalar32_multiply(jp_rz_scalar32_multiply(dy,inv),0x3f066666u);
    uint32_t sy=jp_rz_scalar32_multiply(jp_rz_scalar32_multiply(dx,inv),0x3f066666u)^0x80000000u;
    q=emit(q,p,a,jp_rz_scalar32_add(ax,sx),jp_rz_scalar32_add(ay,sy),z,texture,0);
    q=emit(q,p,a,jp_rz_scalar32_subtract(ax,sx),jp_rz_scalar32_subtract(ay,sy),z,texture,0);
    q=emit(q,p,b,jp_rz_scalar32_add(bx,sx),jp_rz_scalar32_add(by,sy),z,texture,0);
    return emit(q,p,b,jp_rz_scalar32_subtract(bx,sx),jp_rz_scalar32_subtract(by,sy),z,texture,1);
}
int jp_polygon2d_draw(jp_point2col *p,int32_t n,float pri,uint32_t attr)
{
    int filled=!!(attr&0x20u),texture=!!(attr&0x80000000u),trans=!!(attr&0x40u);
    if(!p || !p->p || !p->col || (texture && !p->tex) || n<(filled?3:2) || (filled && n>32))return JP_POLYGON_ARGUMENT;
    /* The real GCC replay owner counts 32-bit flat words in a signed Int. */
    if(n>INT32_MAX/(texture?4:3))return JP_POLYGON_ARGUMENT;
    if(!filled && (uint32_t)n>(UINT32_MAX-1u)/4u)return JP_POLYGON_ARGUMENT;
    uint32_t blocks=1u+(filled?(uint32_t)n:4u*(uint32_t)n);
    if(jp_queue_reserve(trans?JP_QUEUE_TRANSLUCENT:JP_QUEUE_OPAQUE,blocks))return jp_queue_status();
    present_record(JP_PRESENT_POLYGON_2D,p,n,0,pri,attr);
    jp_point2 points[32];jp_point2col copy;
    if(n<=32 && port_wide_squeezes() && (texture || !port_wide_covers(&p->p->x,n,2))) {
        for(int i=0;i<n;++i){points[i].x=port_wide_x(p->p[i].x);points[i].y=p->p[i].y;}
        copy=*p;copy.p=points;p=&copy;
    }
    njGetkmPointer();
    if(jp_queue_status())return jp_queue_status();
    njSetkmColorMode((trans?0x02100002u:0x00080002u)|(texture?8u:0u));
    uint32_t *q=texture ? (trans?njStartVertexTexTrans():njStartVertexTexOpaque()) : (trans?njStartVertexNonTexTrans():njStartVertexNonTexOpaque());
    if(!q)return jp_queue_status();
    uint32_t priority=bits(pri),magnitude=priority&0x7fffffffu;
    uint32_t depth=(priority&0x80000000u) && magnitude>0x3f800000u && magnitude<=0x7f800000u ? jp_rz_scalar32_divide(0xbf800000u,priority) : 0x3f800000u;
    if(filled) {
        for(int i=0;i<n;++i) {
            int k=(i&1)?n-1-i/2:i/2;
            q=emit(q,p,k,bits(p->p[k].x),bits(p->p[k].y),depth,texture,i==n-1);
        }
    } else {
        for(int i=1;i<n;++i)q=edge(q,p,(i&1)?i-1:i,(i&1)?i:i-1,depth,texture);
        q=edge(q,p,n-1,0,depth,texture);
    }
    njEndVertex(q);njSetkmPointer();return jp_queue_status();
}
void njDrawPolygon2D(jp_point2col *p,int32_t n,float pri,uint32_t attr) { (void)jp_polygon2d_draw(p,n,pri,attr); }
