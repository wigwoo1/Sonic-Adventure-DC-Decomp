/* Authored value adapters for original ADX arithmetic and runtime cosine. */
#include "jp_native_adx_numeric.h"
#include <string.h>
#include "adx_runtime_constants.h"
__declspec(dllimport) uint32_t jp_rz_scalar32_add(uint32_t,uint32_t);
__declspec(dllimport) uint32_t jp_rz_scalar32_subtract(uint32_t,uint32_t);
__declspec(dllimport) uint32_t jp_rz_scalar32_multiply(uint32_t,uint32_t);
__declspec(dllimport) uint32_t jp_rz_scalar32_divide(uint32_t,uint32_t);
__declspec(dllimport) uint32_t jp_rz_scalar32_fmac(uint32_t,uint32_t,uint32_t);
__declspec(dllimport) uint32_t jp_rz_camera_sqrt(uint32_t);
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
static uint32_t bits(float f){uint32_t u;memcpy(&u,&f,4);return u;}
static float number(uint32_t u){float f;memcpy(&f,&u,4);return f;}
static uint32_t from_integer(uint32_t word)
{uint32_t policy=0x40001,result=0;if(sa_sh4_float_i32(word,&policy,&result))__builtin_trap();return result;}
static uint32_t to_integer(uint32_t word)
{uint32_t policy=0x40001,result=0;if(sa_sh4_truncate_i32(word,&policy,&result))__builtin_trap();return result;}
static uint32_t from_unsigned(uint32_t word)
{return word<0x80000000u?from_integer(word):jp_rz_scalar32_add(from_integer(word-0x80000000u),0x4f000000u);}
static uint32_t to_unsigned(uint32_t word)
{return word>=0x4f000000u?to_integer(jp_rz_scalar32_subtract(word,0x4f000000u))+0x80000000u:to_integer(word);}
static uint32_t cosine(uint32_t word)
{
    uint32_t x=word&0x7fffffffu;if(x>=0x7f800000u)return 0x7f800001u;
    uint32_t quotient_word=jp_rz_scalar32_divide(x,0x3f490fdbu);
    /* Original whole() includes FTRC/FLOAT below 2^32, followed by the
     * original unsigned conversion. Preserve that intermediate rounding. */
    if((quotient_word&0x7f800000u)<0x4f800000u)quotient_word=from_integer(to_integer(quotient_word));
    uint32_t quotient=to_unsigned(quotient_word);
    x=jp_rz_scalar32_subtract(x,jp_rz_scalar32_multiply(from_unsigned(quotient),0x3f490fdbu));
    unsigned octant=quotient&7u;
    if(octant&1u)x=jp_rz_scalar32_add(0xbf490fdbu,x);
    int use_cosine=octant==0||octant==3||octant==4||octant==7;
    const uint32_t *coeff=use_cosine?adx_cos_coeff:adx_sin_coeff;
    uint32_t square=jp_rz_scalar32_multiply(x,x),sum=coeff[0];
    for(int i=1;i<7;i++)sum=jp_rz_scalar32_fmac(sum,square,coeff[i]);
    if(!use_cosine)sum=jp_rz_scalar32_multiply(x,sum);
    if(octant>=1&&octant<=4)sum^=0x80000000u;
    return sum;
}
__declspec(dllexport) void ADX_GetCoefficient(int32_t cutoff,int32_t rate,int16_t *k0,int16_t *k1)
{
    uint32_t root=jp_rz_camera_sqrt(0x40000000u);
    uint32_t angle=jp_rz_scalar32_divide(jp_rz_scalar32_multiply(0x40c90fdbu,from_integer((uint32_t)cutoff)),from_integer((uint32_t)rate));
    uint32_t a=jp_rz_scalar32_subtract(root,cosine(angle));
    uint32_t b=jp_rz_scalar32_subtract(root,0x3f800000u);
    uint32_t product=jp_rz_scalar32_multiply(jp_rz_scalar32_add(a,b),jp_rz_scalar32_subtract(a,b));
    uint32_t c=jp_rz_scalar32_divide(jp_rz_scalar32_subtract(a,jp_rz_camera_sqrt(product)),b);
    uint16_t first=(uint16_t)to_integer(jp_rz_scalar32_multiply(c,0x46000000u));
    uint16_t second=(uint16_t)to_integer(jp_rz_scalar32_multiply(jp_rz_scalar32_multiply(c,c),0x45800000u)^0x80000000u);
    memcpy(k0,&first,2);memcpy(k1,&second,2);
}
float jp_adx_madd(float a,float b,float c)
{return number(jp_rz_scalar32_fmac(bits(a),bits(b),bits(c)));}
float jp_adx_multiply(float a,float b)
{return number(jp_rz_scalar32_multiply(bits(a),bits(b)));}
int32_t jp_adx_truncate(float f)
{uint32_t u=to_integer(bits(f));int32_t v;memcpy(&v,&u,4);return v;}
int32_t jp_adx_shift(int32_t value,unsigned shift)
{uint32_t u=(uint32_t)value<<shift;int32_t v;memcpy(&v,&u,4);return v;}
int32_t jp_adx_asr(int32_t value,unsigned shift)
{uint32_t u=(uint32_t)value;u=(u>>shift)|((u&0x80000000u)?~(UINT32_MAX>>shift):0);int32_t v;memcpy(&v,&u,4);return v;}
uint32_t jp_adx_bswap32(uint32_t u)
{return (u>>24)|((u>>8)&0xff00u)|((u<<8)&0xff0000u)|(u<<24);}
