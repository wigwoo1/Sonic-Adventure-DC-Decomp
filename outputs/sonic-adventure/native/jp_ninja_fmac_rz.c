/* Reuses the preserved authored exact-product/one-rounding FMAC core.
 * Reference: Renesas SH-4 Software Manual rev6, section 9.36, special cases.
 * Numerical flags/traps and full nonfinite hardware equivalence are unqualified. */
#include "jp_ninja_fmac_rz.h"
#include "../source/sh4_fmac.h"
static uint32_t flush(uint32_t x)
{
    return (x&0x7fffffffu)<0x800000u ? x&0x80000000u : x;
}
uint32_t jp_rz_scalar32_fmac(uint32_t a,uint32_t b,uint32_t c)
{
    a=flush(a);b=flush(b);c=flush(c);
    uint32_t aa=a&0x7fffffffu,bb=b&0x7fffffffu,cc=c&0x7fffffffu;
    if (aa>0x7f800000u || bb>0x7f800000u || cc>0x7f800000u) return 0x7fbfffffu;
    if (aa==0x7f800000u || bb==0x7f800000u) {
        uint32_t sign=(a^b)&0x80000000u;
        if (aa==0 || bb==0) return 0x7fbfffffu;
        if (cc==0x7f800000u && ((c^sign)&0x80000000u)) return 0x7fbfffffu;
        return sign|0x7f800000u;
    }
    if (cc==0x7f800000u) return c;
    uint32_t policy=0x40001u,result=0;
    if (sa_sh4_fmac(&a,&b,&c,&policy,&result)!=0) __builtin_trap();
    return result;
}
