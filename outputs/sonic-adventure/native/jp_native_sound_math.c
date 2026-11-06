/* Authored native counterparts of the pinned Japanese instruction contracts. */
#include "jp_native_sound_math.h"
#include <string.h>
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"
__declspec(dllimport) uint32_t jp_rz_camera_sqrt(uint32_t);
static float number(uint32_t word)
{float value;memcpy(&value,&word,4);return value;}
float njSin(int32_t angle)
{
    uint32_t sine,cosine;sh4_fsca((uint32_t)angle&UINT32_C(0xffff),1,&sine,&cosine);
    return number(sine);
}
float njCos(int32_t angle)
{
    uint32_t sine,cosine;sh4_fsca((uint32_t)angle&UINT32_C(0xffff),1,&sine,&cosine);
    return number(cosine);
}
float njSqrt(float value)
{
    uint32_t word;memcpy(&word,&value,4);uint32_t absolute=word&UINT32_C(0x7fffffff);
    /* Original 0>n comparison treats DN subnormals as zero and NaNs as
     * unordered. Only negative normal/inf enters abs/sqrt/negate. */
    if((word&UINT32_C(0x80000000))&&absolute>=UINT32_C(0x800000)&&absolute<=UINT32_C(0x7f800000))
        return number(jp_rz_camera_sqrt(absolute)^UINT32_C(0x80000000));
    return number(jp_rz_camera_sqrt(word));
}
float vector_length_squared(float x,float y,float z)
{
    uint32_t vector[4];memcpy(vector,&x,4);memcpy(vector+1,&y,4);memcpy(vector+2,&z,4);vector[3]=0;
    return number(sh4_fipr(vector,vector,1));
}
__declspec(dllexport) uintptr_t peer_sound_sqrt_import(void)
{return (uintptr_t)&jp_rz_camera_sqrt;}
