#ifndef JP_NATIVE_ADX_NUMERIC_H
#define JP_NATIVE_ADX_NUMERIC_H
#include <stdint.h>
/* Internal SDK value helpers, fixed RZ/DN1; callers own valid buffers. */
float jp_adx_madd(float,float,float);
float jp_adx_multiply(float,float);
int32_t jp_adx_truncate(float);
int32_t jp_adx_shift(int32_t,unsigned);
int32_t jp_adx_asr(int32_t,unsigned);
uint32_t jp_adx_bswap32(uint32_t);
#endif
