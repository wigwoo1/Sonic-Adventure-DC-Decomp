/* Authored value implementation of the pinned SDK camera-math behavior.
 * Exact scalar services preserve original ordering. Mathematical constants
 * are recorded in jp_camera_external_inputs.json; game bodies stay external. */
#include "jp_ninja_camera_math.h"
#include "jp_ninja_scalar32_rz.h"
#include "jp_ninja_fmac_rz.h"
extern int sa_sh4_sqrt(uint32_t,uint32_t *,uint32_t *);
#define ONE 0x3f800000u
#define HALF 0x3f000000u
#define PIO2 0x3fc90fdbu
#define PIO4 0x3f490fdbu
#define RAW_NAN 0x7f800001u
volatile int32_t jp_sdk_math_errno;
uint32_t jp_rz_camera_sqrt(uint32_t x)
{
    uint32_t absolute=x&0x7fffffffu;
    if (absolute<0x800000u) return x&0x80000000u;
    if ((x&0x80000000u) || absolute>0x7f800000u) return 0x7fbfffffu;
    if (absolute==0x7f800000u) return x;
    uint32_t policy=0x40001u,result=0;
    if (sa_sh4_sqrt(x,&policy,&result)!=0) __builtin_trap();
    return result;
}
static uint32_t scale_power(uint32_t x,int shift)
{
    uint32_t sign=x&0x80000000u,exponent=(x>>23)&255u;
    if (exponent==255u) return x&0x7fffffu ? RAW_NAN : x;
    if (exponent==0u) return sign;
    int adjusted=(int)exponent+shift;
    if (adjusted<=0 || adjusted>=255) return shift>=0 ? sign|0x7f800000u : sign;
    return sign|((uint32_t)adjusted<<23)|(x&0x7fffffu);
}
static uint32_t small_atan(uint32_t x)
{
    static const uint32_t coefficient[6]={3183095273u,1038323256u,3188869413u,1045220557u,3198855851u,1065353216u};
    uint32_t square=jp_rz_scalar32_multiply(x,x),sum=coefficient[0];
    for (int i=1;i<6;++i) sum=jp_rz_scalar32_fmac(square,sum,coefficient[i]);
    return jp_rz_scalar32_multiply(x,sum);
}
static uint32_t atan_value(uint32_t x)
{
    uint32_t sign=x&0x80000000u,magnitude=x&0x7fffffffu;
    if (magnitude>0x7f800000u) return RAW_NAN;
    if (magnitude>=0x4f800000u) return PIO2|sign;
    if (magnitude<0x800000u) return x;
    if (magnitude==ONE) return PIO4|sign;
    uint32_t offset=0,reduced=magnitude,result;
    if (magnitude>ONE) {
        offset=PIO4;
        reduced=jp_rz_scalar32_divide(jp_rz_scalar32_add(0xbf800000u,magnitude),jp_rz_scalar32_add(ONE,magnitude));
    }
    /* reduced is positive and below one. Its eighth-bin quotient is exact
     * integer truncation of reduced*8; the unused original remainder is not
     * a camera result. DN makes an exponent-zero input an integer zero. */
    int exponent=(int)((reduced>>23)&255u)-127;
    uint32_t quotient=exponent<-3 ? 0u : ((reduced&0x7fffffu)|0x800000u)>>(20-exponent);
    uint32_t ratio;
    if (quotient==0u) result=jp_rz_scalar32_add(offset,small_atan(reduced));
    else if (quotient<=2u) {
        ratio=jp_rz_scalar32_divide(jp_rz_scalar32_add(0xbe800000u,reduced),jp_rz_scalar32_add(ONE,scale_power(reduced,-2)));
        result=jp_rz_scalar32_add(jp_rz_scalar32_add(offset,1048239024u),small_atan(ratio));
    } else if (quotient<=4u) {
        ratio=jp_rz_scalar32_divide(jp_rz_scalar32_add(0xbf000000u,reduced),jp_rz_scalar32_add(ONE,scale_power(reduced,-1)));
        result=jp_rz_scalar32_add(jp_rz_scalar32_add(offset,1055744824u),small_atan(ratio));
    } else if (quotient<=6u) {
        ratio=jp_rz_scalar32_divide(jp_rz_scalar32_add(reduced,0xbf400000u),jp_rz_scalar32_fmac(reduced,0x3f400000u,ONE));
        result=jp_rz_scalar32_add(jp_rz_scalar32_add(offset,1059372157u),small_atan(ratio));
    } else {
        ratio=jp_rz_scalar32_divide(jp_rz_scalar32_add(0xbf800000u,reduced),jp_rz_scalar32_add(ONE,reduced));
        result=jp_rz_scalar32_add(jp_rz_scalar32_add(offset,PIO4),small_atan(ratio));
    }
    return result^sign;
}
static uint32_t asin_value(uint32_t x)
{
    uint32_t magnitude=x&0x7fffffffu;
    if (magnitude>ONE) return RAW_NAN;
    if (magnitude==ONE) return PIO2|(x&0x80000000u);
    if (magnitude<0x800000u) return x;
    uint32_t root=jp_rz_camera_sqrt(jp_rz_scalar32_subtract(ONE,jp_rz_scalar32_multiply(x,x)));
    uint32_t ratio=magnitude<HALF ? jp_rz_scalar32_divide(x,jp_rz_scalar32_add(ONE,root))
                                 : jp_rz_scalar32_divide(jp_rz_scalar32_subtract(ONE,root),x);
    return scale_power(atan_value(ratio),1);
}
uint32_t jp_rz_camera_acos(uint32_t x)
{
    uint32_t result=jp_rz_scalar32_subtract(PIO2,asin_value(x));
    if ((result&0x7fffffffu)>=0x7f800000u) jp_sdk_math_errno=(result&0x7fffffu) ? 1101 : 1100;
    return result;
}
