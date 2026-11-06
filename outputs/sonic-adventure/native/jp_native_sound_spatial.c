#include "jp_native_sound_spatial.h"
#include <string.h>
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"

/* Import the actual existing frame DLL's matrix and arithmetic providers.
 * No matrix arena, FPU register bank or duplicate scalar implementation here.
 */
__declspec(dllimport) void njCalcPoint(void *, void *, void *);
__declspec(dllimport) uint32_t jp_rz_scalar32_add(uint32_t, uint32_t);
__declspec(dllimport) uint32_t jp_rz_scalar32_multiply(uint32_t, uint32_t);

static uint32_t bits(float value)
{uint32_t word;memcpy(&word,&value,4);return word;}
static float number(uint32_t word)
{float value;memcpy(&value,&word,4);return value;}

void matrix_calc_point(void *point)
{
    /* The null-matrix in-place path executes the same current-matrix FTRV. */
    njCalcPoint(NULL,point,point);
}

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

float jp_qsound_position_side(float x)
{
    uint32_t sum=jp_rz_scalar32_add(bits(x),UINT32_C(0x3f800000));
    return number(jp_rz_scalar32_multiply(sum,UINT32_C(0x41780000)));
}

int32_t jp_qsound_position_channel(float side)
{
    uint32_t word=bits(side),absolute=word&UINT32_C(0x7fffffff),integer;
    int negative=(int)(word>>31);
    if(absolute>(negative?UINT32_C(0x4f000000):UINT32_C(0x4effffff)))
        integer=(!negative&&absolute<=UINT32_C(0x7f800000))?
                UINT32_C(0x7fffffff):UINT32_C(0x80000000);
    else if(absolute<UINT32_C(0x3f800000))integer=0;
    else{
        int exponent=(int)(absolute>>23)-127;
        uint32_t significand=(absolute&UINT32_C(0x7fffff))|UINT32_C(0x800000);
        uint32_t magnitude=exponent>=23?significand<<(exponent-23):significand>>(23-exponent);
        integer=negative?0u-magnitude:magnitude;
    }
    /* Original FTRC followed by SHLL2: explicit unsigned wrapping, including
     * invalid/overflow conversion results, rather than an undefined C cast. */
    integer<<=2;
    int32_t result;memcpy(&result,&integer,4);return result;
}

__declspec(dllexport) int peer_spatial_import_addresses(uintptr_t *out)
{
    if(!out)return 1;
    out[0]=(uintptr_t)&njCalcPoint;
    out[1]=(uintptr_t)&jp_rz_scalar32_add;
    out[2]=(uintptr_t)&jp_rz_scalar32_multiply;
    return 0;
}
