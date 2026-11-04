/* Float-shaped ABI over exact integer RZ/DN1 value services. No FPSCR emulation. */
#ifndef JP_NATIVE_SPRITE_NUMERIC_H
#define JP_NATIVE_SPRITE_NUMERIC_H
#include <stdint.h>
#define JP_SPRITE_API __declspec(dllexport)
JP_SPRITE_API float jp_sprite_add(float,float);
JP_SPRITE_API float jp_sprite_subtract(float,float);
JP_SPRITE_API float jp_sprite_multiply(float,float);
JP_SPRITE_API float jp_sprite_divide(float,float);
JP_SPRITE_API float jp_sprite_negate(float);
JP_SPRITE_API float jp_sprite_add_assign(float *,float);
JP_SPRITE_API float jp_sprite_subtract_assign(float *,float);
JP_SPRITE_API float jp_sprite_multiply_assign(float *,float);
JP_SPRITE_API float jp_sprite_divide_assign(float *,float);
JP_SPRITE_API int jp_sprite_eq(float,float);
JP_SPRITE_API int jp_sprite_ne(float,float);
JP_SPRITE_API int jp_sprite_lt(float,float);
JP_SPRITE_API int jp_sprite_le(float,float);
JP_SPRITE_API int jp_sprite_gt(float,float);
JP_SPRITE_API int jp_sprite_ge(float,float);
JP_SPRITE_API void jp_sprite_fsca(int32_t,float *,float *);
JP_SPRITE_API void jp_sprite_transform_point(const float *,float *);
#endif
