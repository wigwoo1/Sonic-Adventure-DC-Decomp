/* SH-4 FIPR/FLOAT/FTRC value semantics; no second floating-state owner. */
#include <stdint.h>
#include <string.h>
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
static uint32_t from_integer(uint32_t word)
{
    uint32_t policy=0x40001,result=0;
    if(sa_sh4_float_i32(word,&policy,&result))__builtin_trap();
    return result;
}
static uint32_t to_integer(uint32_t word)
{
    uint32_t policy=0x40001,result=0;
    if(sa_sh4_truncate_i32(word,&policy,&result))__builtin_trap();
    return result;
}
__declspec(dllexport) float jp_sdk_motion_from_int(int32_t number)
{uint32_t word=from_integer((uint32_t)number);float value;memcpy(&value,&word,4);return value;}
__declspec(dllexport) int32_t jp_sdk_motion_to_int(float value)
{uint32_t word;int32_t number;memcpy(&word,&value,4);word=to_integer(word);memcpy(&number,&word,4);return number;}
__declspec(dllexport) void jp_sdk_motion_sum(float *a,float *b,float *c,float *d,
                                            float wa,float wb,float wc,float wd,float *out)
{
    float weights[4]={wa,wb,wc,wd};uint32_t w[4],result[3];
    memcpy(w,weights,16);
    for(unsigned i=0;i<3;i++){
        float v[4]={a[i],b[i],c[i],d[i]};uint32_t words[4];
        memcpy(words,v,16);result[i]=sh4_fipr(w,words,1);
    }
    /* Original assembly stores z,y,x after computing all three values. */
    memcpy(out+2,result+2,4);memcpy(out+1,result+1,4);memcpy(out,result,4);
}
__declspec(dllexport) void jp_sdk_motion_sum_angles(void *a,void *b,void *c,void *d,
                                                   float wa,float wb,float wc,float wd,void *out)
{
    float weights[4]={wa,wb,wc,wd};uint32_t w[4],result[3];
    memcpy(w,weights,16);
    for(unsigned i=0;i<3;i++){
        uint32_t v[4],word;void *arrays[4]={a,b,c,d};
        for(unsigned j=0;j<4;j++){
            memcpy(&word,(char *)arrays[j]+i*4,4);v[j]=from_integer(word);
        }
        result[i]=sh4_fipr(w,v,1);
    }
    for(int i=2;i>=0;i--){uint32_t word=to_integer(result[i]);memcpy((char *)out+i*4,&word,4);}
}
