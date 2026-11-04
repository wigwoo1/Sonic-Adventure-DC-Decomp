/* Authored word-valued camera math, fixed RZ/DN1 and disabled FPU traps.
 * SDK error state is kept separate from the host CRT's _errno() function.
 * Native SDK consumers must bind their original _errno data to this adapter. */
#ifndef JP_NINJA_CAMERA_MATH_H
#define JP_NINJA_CAMERA_MATH_H
#include "jp_ninja_math.h"
JP_MATH_API extern volatile int32_t jp_sdk_math_errno;
JP_MATH_API uint32_t jp_rz_camera_sqrt(uint32_t);
JP_MATH_API uint32_t jp_rz_camera_acos(uint32_t);
#endif
