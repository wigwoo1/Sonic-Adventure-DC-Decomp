/* Authored ABI for the three view-direction/roll services used by presentation. */
#ifndef JP_NINJA_VIEW_ROTATION_H
#define JP_NINJA_VIEW_ROTATION_H
#include "jp_ninja_view_stack.h"
JP_MATH_API void njRotateViewXAbsolute(jp_view *,int32_t);
JP_MATH_API void njRotateViewYRelative(jp_view *,int32_t);
JP_MATH_API void njRotateViewZAbsolute(jp_view *,int32_t);
#endif
