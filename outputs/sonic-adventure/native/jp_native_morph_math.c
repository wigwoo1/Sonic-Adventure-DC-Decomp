#include "jp_native_morph_math.h"
#include <string.h>
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"
uint32_t jp_rz_scalar32_multiply(uint32_t,uint32_t);
static float number(uint32_t word)
{float value;memcpy(&value,&word,4);return value;}
float njSin(int32_t angle)
{
    uint32_t sine,cosine;
    sh4_fsca((uint32_t)angle&UINT32_C(0xffff),1,&sine,&cosine);
    return number(sine);
}
/* Same numerical sequence as the already qualified sound normalizer. Move
 * that provider to Frames; the sound DLL forwards its existing export here. */
float njUnitVector(void *vector)
{
    uint32_t original[4], normalized[3];
    memcpy(original,vector,12);original[3]=0;
    uint32_t squared=sh4_fipr(original,original,1);
    uint32_t reciprocal=sh4_fsrra(squared,1);
    for(int i=0;i<3;i++)
        normalized[i]=jp_rz_scalar32_multiply(original[i],reciprocal);
    memcpy(vector,normalized,12);
    return number(jp_rz_scalar32_multiply(squared,reciprocal));
}
