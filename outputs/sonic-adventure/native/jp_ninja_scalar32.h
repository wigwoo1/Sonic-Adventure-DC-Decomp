/* Authored value-only binary32 boundary: RN, DN=1, exceptions disabled.
 * Inputs/outputs are raw words, independent of the host floating environment.
 * Uses the project's existing exact scalar cores. Does not emulate FPSCR. */
#ifndef JP_NINJA_SCALAR32_H
#define JP_NINJA_SCALAR32_H
#include "jp_ninja_math.h"
JP_MATH_API uint32_t jp_scalar32_add(uint32_t, uint32_t);
JP_MATH_API uint32_t jp_scalar32_subtract(uint32_t, uint32_t);
JP_MATH_API uint32_t jp_scalar32_multiply(uint32_t, uint32_t);
JP_MATH_API uint32_t jp_scalar32_divide(uint32_t, uint32_t);
#endif
