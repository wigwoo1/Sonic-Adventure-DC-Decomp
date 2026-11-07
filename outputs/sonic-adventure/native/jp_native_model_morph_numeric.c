#include "jp_native_model_morph_numeric.h"
#include "jp_ninja_fmac_rz.h"
#include <string.h>
float jp_model_morph_fmac(float x,float y,float accumulator)
{
    uint32_t a,b,c,out;float value;
    memcpy(&a,&x,4);memcpy(&b,&y,4);memcpy(&c,&accumulator,4);
    out=jp_rz_scalar32_fmac(a,b,c);memcpy(&value,&out,4);return value;
}
