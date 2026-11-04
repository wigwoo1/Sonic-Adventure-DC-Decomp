/* Authored original SDK value/state contract. No original game body is linked. */
#include "jp_ninja_camera.h"
#include "jp_ninja_scalar32_rz.h"
#include "jp_ninja_fmac_rz.h"
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"
#include <string.h>
static uint32_t bits(float x) { uint32_t u;memcpy(&u,&x,4);return u; }
static float number(uint32_t u) { float x;memcpy(&x,&u,4);return x; }
static int32_t angle_word(uint32_t x) { int32_t i;memcpy(&i,&x,4);return i; }
static int less_zero(uint32_t x)
{
    uint32_t absolute=x&0x7fffffffu;
    return (x&0x80000000u) && absolute>=0x800000u && absolute<=0x7f800000u;
}
static uint32_t truncate_word(uint32_t x)
{
    uint32_t absolute=x&0x7fffffffu;
    if (absolute>0x7f800000u) return 0x80000000u;
    if (absolute>=0x4f000000u) return x&0x80000000u ? 0x80000000u : 0x7fffffffu;
    int exponent=(int)((x>>23)&255u)-127;
    if (exponent<0) return 0;
    uint32_t sig=(x&0x7fffffu)|0x800000u;
    uint32_t value=exponent>=23 ? sig<<(exponent-23) : sig>>(23-exponent);
    return x&0x80000000u ? 0u-value : value;
}
void njTranslate(jp_matrix *m,float x,float y,float z)
{
    jp_vector vector={x,y,z};
    njTranslateV(m,&vector);
}
void njSetView(jp_view *view)
{
    njUnitMatrix(&view->m);
    njRotateZ(&view->m,angle_word(0u-(uint32_t)view->roll));
    uint32_t x=bits(view->vx),y=bits(view->vy),z=bits(view->vz);
    uint32_t h=jp_rz_camera_sqrt(jp_rz_scalar32_fmac(z,z,jp_rz_scalar32_multiply(x,x)));
    uint32_t squared_length=jp_rz_scalar32_fmac(h,h,jp_rz_scalar32_multiply(y,y));
    uint32_t pitch=jp_rz_camera_acos(jp_rz_scalar32_multiply(h,sh4_fsrra(squared_length,1)));
    uint32_t magnitude=truncate_word(jp_rz_scalar32_multiply(0x4622f986u,pitch))&0xffffu;
    njRotateX(&view->m,angle_word(less_zero(y) ? 0u-magnitude : magnitude));
    uint32_t yaw=jp_rz_camera_acos(jp_rz_scalar32_divide(z,h));
    magnitude=truncate_word(jp_rz_scalar32_multiply(0x4622f986u,yaw))&0xffffu;
    njRotateY(&view->m,angle_word(less_zero(x) ? magnitude : 0u-magnitude));
    njTranslate(&view->m,number(bits(view->px)^0x80000000u),number(bits(view->py)^0x80000000u),number(bits(view->pz)^0x80000000u));
    _nj_view_=view;
    njClearMatrix();
}
