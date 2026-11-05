/* Authored value-only boundary for the original FLOAT/FMUL/FMUL/FTRC chain.
 * The genuine SDK rand/srand/state and game shuffle bodies compile separately
 * from pinned external sources. No host CRT RNG or native FP arithmetic.
 */
#include "jp_native_random_pick.h"
#include "jp_ninja_scalar32_rz.h"
#include <limits.h>
static int shuffle_error;
static int invalid_arguments(void){shuffle_error=1;return 0;}
uint32_t jp_random_positive_float_word(uint32_t integer)
{
    if(!integer)return 0;
    unsigned power=0;for(uint32_t n=integer;n>1;n>>=1)++power;
    uint32_t significand=power>23?integer>>(power-23):integer<<(23-power);
    return ((power+127u)<<23)|(significand&0x7fffffu);
}
int32_t jp_random_pick(uint32_t draw,int32_t left)
{
    if(draw>32767 || left<1)return -1;
    uint32_t value=jp_rz_scalar32_multiply(jp_random_positive_float_word(draw),jp_random_positive_float_word((uint32_t)left));
    value=jp_rz_scalar32_multiply(value,0x38000000u); /* exactly 2^-15 */
    unsigned exponent=(value>>23)&255u;
    if(exponent<127)return 0;
    uint32_t significand=(value&0x7fffffu)|0x800000u;
    unsigned power=exponent-127;
    /* For this complete valid input domain 0 <= value < left <= INT_MAX.
     * No exceptional conversion or host floating environment is involved.
     */
    return (int32_t)(power>=23?significand<<(power-23):significand>>(23-power));
}
int jp_shuffle_arguments(const void *array,int count,int size)
{
    shuffle_error=0;
    if(count<0 || size<0 || (count>1 && size && !array))return invalid_arguments();
    if(count && (size_t)size>SIZE_MAX/(size_t)count)return invalid_arguments();
    size_t total=(size_t)count*(size_t)size;
    if(total>INT_MAX || (array && total>UINTPTR_MAX-(uintptr_t)array))return invalid_arguments();
    return 1;
}
int jp_shuffle_status(void){return shuffle_error;}
