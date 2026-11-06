#ifndef JP_NATIVE_SOUND_MATH_H
#define JP_NATIVE_SOUND_MATH_H
#include <stdint.h>
/* Original internal value ABI: fixed RZ/DN1, exceptions disabled. */
#define JP_SOUND_MATH_API __declspec(dllexport)
JP_SOUND_MATH_API float njSin(int32_t angle);
JP_SOUND_MATH_API float njCos(int32_t angle);
JP_SOUND_MATH_API float njSqrt(float value);
JP_SOUND_MATH_API float vector_length_squared(float x,float y,float z);
#endif
