#include "jp_native_runtime_floor.h"
#include <string.h>
extern float floorf(float value);
float jp_runtime_trunc_scalar(float value)
{
    uint32_t word, exponent, result;
    __builtin_memcpy(&word, &value, sizeof word);
    exponent = (word >> 23) & 255u;
    /* The original whole() tests this bound before calling the bridge. */
    if (exponent >= 159u) __builtin_trap();
    if (exponent < 127u) {
        result = 0u;
    } else if (exponent == 158u) {
        /* FTRC saturates; FLOAT converts INT32_MAX toward zero. */
        result = (word & 0x80000000u) ? 0xcf000000u : 0x4effffffu;
    } else if (exponent >= 150u) {
        result = word;
    } else {
        result = word & ~((1u << (150u - exponent)) - 1u);
    }
    __builtin_memcpy(&value, &result, sizeof value);
    return value;
}
uint32_t jp_runtime_floor_word(uint32_t word)
{
    float value;
    __builtin_memcpy(&value, &word, sizeof value);
    value = floorf(value);
    __builtin_memcpy(&word, &value, sizeof word);
    return word;
}
