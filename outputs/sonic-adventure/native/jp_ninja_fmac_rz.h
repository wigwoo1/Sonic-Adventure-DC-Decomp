/* Authored binary32 value boundary for fused a*b+c, RZ/DN1, traps disabled.
 * Does not emulate FPSCR flags, register aliases or hardware exception entry. */
#ifndef JP_NINJA_FMAC_RZ_H
#define JP_NINJA_FMAC_RZ_H
#include "jp_ninja_math.h"
JP_MATH_API uint32_t jp_rz_scalar32_fmac(uint32_t,uint32_t,uint32_t);
#endif
