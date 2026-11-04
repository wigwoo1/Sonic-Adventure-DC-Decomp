/* Actual sprite-module settings ownership, separated for the host port.
 * The wide-frame owner still supplies anchor/cover. No fallback globals. */
#include "jp_ninja_polygon2d.h"
#include "jp_ninja_scalar32_rz.h"
#include <string.h>
jp_sprite_record_fn port_sprite_record;
float port_draw_aspect = 1.0f;
int32_t port_pillarbox;
static uint32_t bits(float x) { uint32_t u; memcpy(&u,&x,4); return u; }
static float value(uint32_t u) { float x; memcpy(&x,&u,4); return x; }
static float compare_value(float x)
{
    uint32_t u=bits(x);
    return (u&0x7fffffffu)<0x800000u ? value(u&0x80000000u) : x;
}
float port_wide_x(float x)
{
    uint32_t at=port_pillarbox ? 0x43a00000u : jp_rz_scalar32_add(0x43a00000u,bits(port_wide_anchor));
    return value(jp_rz_scalar32_add(at,jp_rz_scalar32_multiply(jp_rz_scalar32_subtract(bits(x),at),bits(port_draw_aspect))));
}
int32_t port_wide_squeezes(void)
{
    return compare_value(port_draw_aspect)!=1.0f && (port_pillarbox || !port_wide_cover);
}
int32_t port_wide_covers(const float *x,int32_t count,int32_t stride)
{
    int left=0,right=0;
    if(port_pillarbox)return 0;
    for(;count>0;--count,x+=stride) {
        float f=compare_value(*x);
        if(f>=-1.0f && f<=1.0f)left=1;
        else if(f>=639.0f && f<=641.0f)right=1;
        else return 0;
    }
    return left && right;
}
