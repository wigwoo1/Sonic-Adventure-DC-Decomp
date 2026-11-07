/* Value bridges for the original Ninja motion source's SH-4 inline bodies. */
#ifndef JP_NATIVE_SDK_MOTION_NUMERIC_H
#define JP_NATIVE_SDK_MOTION_NUMERIC_H
#include "jp_native_action_numeric.h"
#include <stdint.h>
__declspec(dllexport) float jp_sdk_motion_from_int(int32_t);
__declspec(dllexport) int32_t jp_sdk_motion_to_int(float);
__declspec(dllexport) void jp_sdk_motion_sum(float *,float *,float *,float *,
                                           float,float,float,float,float *);
__declspec(dllexport) void jp_sdk_motion_sum_angles(void *,void *,void *,void *,
                                                  float,float,float,float,void *);
static void weighted_sum(Float *a,Float *b,Float *c,Float *d,
                         Float wa,Float wb,Float wc,Float wd,Float *out)
{jp_sdk_motion_sum(a,b,c,d,wa,wb,wc,wd,out);}
static void weighted_sum_angles(Angle *a,Angle *b,Angle *c,Angle *d,
                                Float wa,Float wb,Float wc,Float wd,Angle *out)
{jp_sdk_motion_sum_angles(a,b,c,d,wa,wb,wc,wd,out);}
static Float squared_length(Float x,Float y,Float z,Float w)
{return jp_action_fipr_self(x,y,z,w);}
#define _builtin_fsrra jp_action_fsrra
#define ALWAYS_INLINE static inline __attribute__((always_inline))
#endif
