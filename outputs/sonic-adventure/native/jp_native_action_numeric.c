#include "jp_native_action_numeric.h"
#include <stdint.h>
#include <string.h>
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"
float jp_action_fipr_self(float x,float y,float z,float w)
{
    uint32_t v[4],word;float values[4]={x,y,z,w},out;
    memcpy(v,values,sizeof v);word=sh4_fipr(v,v,1);memcpy(&out,&word,4);return out;
}
float jp_action_fsrra(float value)
{
    uint32_t word;float out;memcpy(&word,&value,4);word=sh4_fsrra(word,1);memcpy(&out,&word,4);return out;
}
