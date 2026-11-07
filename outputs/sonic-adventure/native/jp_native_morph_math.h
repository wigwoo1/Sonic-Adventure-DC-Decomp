#ifndef JP_NATIVE_MORPH_MATH_H
#define JP_NATIVE_MORPH_MATH_H
#include <stdint.h>
#ifndef JP_MORPH_MATH_API
#define JP_MORPH_MATH_API __declspec(dllexport)
#endif
JP_MORPH_MATH_API float njSin(int32_t angle);
/* Original valid-input contract: three writable floats, creating game thread. */
JP_MORPH_MATH_API float njUnitVector(void *vector);
#endif
