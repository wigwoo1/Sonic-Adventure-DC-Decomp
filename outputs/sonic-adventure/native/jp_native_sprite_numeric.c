#include "jp_native_sprite_numeric.h"
#include "jp_ninja_scalar32_rz.h"
#include "jp_ninja_shared_state.h"
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"
#include <string.h>
static uint32_t bits(float x){uint32_t u;memcpy(&u,&x,4);return u;}
static float value(uint32_t u){float x;memcpy(&x,&u,4);return x;}
float jp_sprite_add(float a,float b){return value(jp_rz_scalar32_add(bits(a),bits(b)));}
float jp_sprite_subtract(float a,float b){return value(jp_rz_scalar32_subtract(bits(a),bits(b)));}
float jp_sprite_multiply(float a,float b){return value(jp_rz_scalar32_multiply(bits(a),bits(b)));}
float jp_sprite_divide(float a,float b){return value(jp_rz_scalar32_divide(bits(a),bits(b)));}
float jp_sprite_negate(float a){return value(bits(a)^0x80000000u);}
float jp_sprite_add_assign(float *a,float b){return *a=jp_sprite_add(*a,b);}
float jp_sprite_subtract_assign(float *a,float b){return *a=jp_sprite_subtract(*a,b);}
float jp_sprite_multiply_assign(float *a,float b){return *a=jp_sprite_multiply(*a,b);}
float jp_sprite_divide_assign(float *a,float b){return *a=jp_sprite_divide(*a,b);}
static uint32_t flush(uint32_t u){return (u&0x7fffffffu)<0x800000u?u&0x80000000u:u;}
static int compare(float a,float b)
{
    uint32_t x=flush(bits(a)),y=flush(bits(b));
    if((x&0x7fffffffu)>0x7f800000u || (y&0x7fffffffu)>0x7f800000u)return 2;
    if(x==y || !((x|y)&0x7fffffffu))return 0;
    if((x^y)&0x80000000u)return x&0x80000000u?-1:1;
    return x&0x80000000u ? (x>y?-1:1) : (x<y?-1:1);
}
int jp_sprite_eq(float a,float b){return compare(a,b)==0;}
int jp_sprite_ne(float a,float b){return compare(a,b)!=0;}
int jp_sprite_lt(float a,float b){return compare(a,b)==-1;}
int jp_sprite_le(float a,float b){int c=compare(a,b);return c==-1||c==0;}
int jp_sprite_gt(float a,float b){return compare(a,b)==1;}
int jp_sprite_ge(float a,float b){int c=compare(a,b);return c==1||c==0;}
void jp_sprite_fsca(int32_t angle,float *sine,float *cosine)
{
    uint32_t s,c;sh4_fsca((uint32_t)angle,1,&s,&c);memcpy(sine,&s,4);memcpy(cosine,&c,4);
}
void jp_sprite_transform_point(const float *point,float *result)
{
    uint32_t matrix[16],v[4];memcpy(matrix,jp_ninja_current_matrix_storage(),64);
    memcpy(v,point,12);v[3]=0x3f800000u;sh4_ftrv(matrix,v,1);memcpy(result,v,12);
}
