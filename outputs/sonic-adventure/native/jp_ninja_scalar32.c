#include "jp_ninja_scalar32.h"
#include "../source/sh4_add_sub.h"
#include "../source/sh4_multiply_extended.h"
#include "../source/sh4_divide.h"
#define NAN_WORD 0x7fbfffffu
#define INFINITY_WORD 0x7f800000u
static uint32_t checked(int status, uint32_t value)
{
    /* The fixed policy and class dispatch below cover all operand words.
     * An unexpected core rejection is an implementation error, not a value. */
    if (status != 0) __builtin_trap();
    return value;
}
static uint32_t addition(uint32_t a, uint32_t b, int subtract)
{
    uint32_t aa = a & 0x7fffffffu, bb = b & 0x7fffffffu;
    if (aa > INFINITY_WORD || bb > INFINITY_WORD) return NAN_WORD;
    if (aa == INFINITY_WORD || bb == INFINITY_WORD) {
        uint32_t right = b ^ (subtract ? 0x80000000u : 0);
        if (aa == INFINITY_WORD && bb == INFINITY_WORD)
            return (a ^ right) & 0x80000000u ? NAN_WORD : a;
        return aa == INFINITY_WORD ? a : right;
    }
    uint32_t policy = 0x40000u, value = 0;
    int status = sa_sh4_add_sub(&a, &b, &policy, &value, subtract);
    return checked(status, value);
}
uint32_t jp_scalar32_add(uint32_t a, uint32_t b) { return addition(a, b, 0); }
uint32_t jp_scalar32_subtract(uint32_t a, uint32_t b) { return addition(a, b, 1); }
uint32_t jp_scalar32_multiply(uint32_t a, uint32_t b)
{
    uint32_t policy = 0x40000u, value = 0;
    int status = sa_sh4_multiply_extended(&a, &b, &policy, &value);
    return checked(status, value);
}
uint32_t jp_scalar32_divide(uint32_t a, uint32_t b)
{
    uint32_t aa = a & 0x7fffffffu, bb = b & 0x7fffffffu;
    uint32_t sign = (a ^ b) & 0x80000000u;
    if (aa > INFINITY_WORD || bb > INFINITY_WORD) return NAN_WORD;
    if (aa == INFINITY_WORD || bb == INFINITY_WORD) {
        if (aa == INFINITY_WORD && bb == INFINITY_WORD) return NAN_WORD;
        return aa == INFINITY_WORD ? sign | INFINITY_WORD : sign;
    }
    uint32_t policy = 0x40000u, value = 0;
    int status = sa_sh4_divide(&a, &b, &policy, &value);
    return checked(status, value);
}
