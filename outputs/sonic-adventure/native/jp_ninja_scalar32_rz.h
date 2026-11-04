/* Authored value-only binary32 boundary for toward-zero, DN=1 contexts.
 * Integer inputs/results avoid dependence on the host floating environment.
 * This is a fixed numerical policy, not an FPSCR register/trap emulator. */
#ifndef JP_NINJA_SCALAR32_RZ_H
#define JP_NINJA_SCALAR32_RZ_H
#include "jp_ninja_math.h"
JP_MATH_API uint32_t jp_rz_scalar32_add(uint32_t, uint32_t);
JP_MATH_API uint32_t jp_rz_scalar32_subtract(uint32_t, uint32_t);
JP_MATH_API uint32_t jp_rz_scalar32_multiply(uint32_t, uint32_t);
JP_MATH_API uint32_t jp_rz_scalar32_divide(uint32_t, uint32_t);
#endif
