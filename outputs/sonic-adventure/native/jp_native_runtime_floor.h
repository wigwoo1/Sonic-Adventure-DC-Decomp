/* Value-only SH4 conversion bridge for whole()'s finite |x| < 2^32 branch.
 * Fixed FPSCR RM=1, DN=1 policy; no FPSCR flag or trap-state emulation. */
#ifndef JP_NATIVE_RUNTIME_FLOOR_H
#define JP_NATIVE_RUNTIME_FLOOR_H
#include <stdint.h>
__declspec(dllexport) float jp_runtime_trunc_scalar(float value);
/* Integer ABI preserves signaling-NaN payloads across host language bindings.
 * Calls genuine SDK floorf and changes its existing shared SDK errno only. */
__declspec(dllexport) uint32_t jp_runtime_floor_word(uint32_t word);
#endif
