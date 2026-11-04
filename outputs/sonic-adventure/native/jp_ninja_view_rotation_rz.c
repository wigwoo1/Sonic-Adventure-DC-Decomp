/* Authored from the pinned original SDK contracts; original bodies stay external.
 * Shares the actual native current-view pointer, never a copied view instance. */
#include "jp_ninja_view_rotation.h"
#include "jp_ninja_scalar32_rz.h"
#include "jp_ninja_fmac_rz.h"
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"
#include <string.h>
static uint32_t bits(float x) { uint32_t u;memcpy(&u,&x,4);return u; }
static float number(uint32_t u) { float x;memcpy(&x,&u,4);return x; }
void njRotateViewXAbsolute(jp_view *view,int32_t angle)
{
    jp_view *v=view ? view : _nj_view_;
    uint32_t sine,cosine;
    sh4_fsca((uint32_t)angle,1,&sine,&cosine);
    uint32_t x=bits(v->avx),y=bits(v->avy),z=bits(v->avz);
    uint32_t new_z=jp_rz_scalar32_fmac(sine,y,jp_rz_scalar32_multiply(z,cosine));
    uint32_t new_y=jp_rz_scalar32_fmac(sine^0x80000000u,z,jp_rz_scalar32_multiply(y,cosine));
    v->vx=number(x);v->vz=number(new_z);v->vy=number(new_y);
}
void njRotateViewYRelative(jp_view *view,int32_t angle)
{
    jp_view *v=view ? view : _nj_view_;
    uint32_t sine,cosine;
    sh4_fsca((uint32_t)angle,1,&sine,&cosine);
    uint32_t x=bits(v->vx),z=bits(v->vz);
    uint32_t new_x=jp_rz_scalar32_fmac(sine,z,jp_rz_scalar32_multiply(x,cosine));
    uint32_t new_z=jp_rz_scalar32_fmac(sine^0x80000000u,x,jp_rz_scalar32_multiply(z,cosine));
    v->vx=number(new_x);v->vz=number(new_z);
}
void njRotateViewZAbsolute(jp_view *view,int32_t angle)
{
    jp_view *v=view ? view : _nj_view_;
    uint32_t base;memcpy(&base,&v->aroll,4);
    uint32_t result=base+(uint32_t)angle;
    memcpy(&v->roll,&result,4);
}
