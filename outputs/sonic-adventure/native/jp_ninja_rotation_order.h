#ifndef JP_NINJA_ROTATION_ORDER_H
#define JP_NINJA_ROTATION_ORDER_H
#include "jp_ninja_math.h"
/* Same current/saved matrix and RZ/DN1 arithmetic. Zero axes are skipped as
 * in the original combined routines; nonzero full angles retain FSCA wrapping. */
JP_MATH_API void njRotateXYZ(jp_matrix *,int32_t,int32_t,int32_t);
JP_MATH_API void njRotateZXY(jp_matrix *,int32_t,int32_t,int32_t);
#endif
