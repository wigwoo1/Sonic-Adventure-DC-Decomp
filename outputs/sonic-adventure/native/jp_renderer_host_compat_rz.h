/* Value-only RZ/DN1 boundary for the complete pinned GCC model producer.
 * Uses the existing Ninja current matrix; introduces no matrix owner.
 * SH4ZAM remains an external dependency for types and non-arithmetic helpers.
 */
#ifndef JP_RENDERER_HOST_COMPAT_RZ_H
#define JP_RENDERER_HOST_COMPAT_RZ_H
#include "jp_renderer_host_compat_v2.h"
#include <sh4zam/shz_xmtrx.h>
#include "jp_native_primitive_numeric.h"
#include <string.h>
/* Only this storage accessor crosses the ABI. The full authored math header
 * declares ABI adapters that deliberately have other types than SDK headers. */
float *jp_ninja_current_matrix_storage(void);
static inline Float jp_model_fipr(shz_vec4_t a,shz_vec4_t b)
{
    uint32_t x[4],y[4],word;Float out;
    const Float av[4]={a.x,a.y,a.z,a.w},bv[4]={b.x,b.y,b.z,b.w};
    memcpy(x,av,16);memcpy(y,bv,16);word=sh4_fipr(x,y,1);
    memcpy(&out,&word,4);return out;
}
static inline shz_vec4_t jp_model_transform4(shz_vec4_t v)
{
    uint32_t m[16],p[4];const Float a[4]={v.x,v.y,v.z,v.w};Float out[4];
    memcpy(m,jp_ninja_current_matrix_storage(),64);memcpy(p,a,16);
    sh4_ftrv(m,p,1);memcpy(out,p,16);
    return shz_vec4_init(out[0],out[1],out[2],out[3]);
}
static inline shz_vec3_t jp_model_transform3(shz_vec3_t v)
{
    shz_vec4_t r=jp_model_transform4(shz_vec4_init(v.x,v.y,v.z,0.0F));
    return shz_vec3_init(r.x,r.y,r.z);
}
static inline shz_vec3_t jp_model_point3(shz_vec3_t v)
{
    shz_vec4_t r=jp_model_transform4(shz_vec4_init(v.x,v.y,v.z,1.0F));
    return shz_vec3_init(r.x,r.y,r.z);
}
static inline shz_vec4_t jp_model_read_col(unsigned int index)
{
    Float v[4];memcpy(v,jp_ninja_current_matrix_storage()+4*index,16);
    return shz_vec4_init(v[0],v[1],v[2],v[3]);
}
static inline shz_vec2_t jp_model_dot2(shz_vec4_t a,shz_vec4_t b,shz_vec4_t c)
{return shz_vec2_init(jp_model_fipr(a,b),jp_model_fipr(a,c));}
static inline shz_vec3_t jp_model_dot3(shz_vec4_t a,shz_vec4_t b,shz_vec4_t c,shz_vec4_t d)
{return shz_vec3_init(jp_model_fipr(a,b),jp_model_fipr(a,c),jp_model_fipr(a,d));}
#undef shz_xmtrx_transform_vec4
#undef shz_xmtrx_transform_vec3
#undef shz_xmtrx_transform_point3
#undef shz_xmtrx_read_col
#undef shz_vec4_dot2
#undef shz_vec4_dot3
#define shz_xmtrx_transform_vec4 jp_model_transform4
#define shz_xmtrx_transform_vec3 jp_model_transform3
#define shz_xmtrx_transform_point3 jp_model_point3
#define shz_xmtrx_read_col jp_model_read_col
#define shz_vec4_dot2 jp_model_dot2
#define shz_vec4_dot3 jp_model_dot3
#define zam_fipr jp_model_fipr
#endif
