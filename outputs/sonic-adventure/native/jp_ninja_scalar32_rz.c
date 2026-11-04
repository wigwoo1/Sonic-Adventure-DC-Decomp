/* The nearest-mode implementation stays preserved for its historical proofs.
 * Both policies reuse the same existing authored integer arithmetic cores. */
#include "jp_ninja_scalar32_rz.h"
#include "../source/sh4_add_sub.h"
#include "../source/sh4_multiply_extended.h"
#include "../source/sh4_divide.h"
#define NAN_WORD 0x7fbfffffu
#define INFINITY_WORD 0x7f800000u
#define POLICY 0x40001u
static uint32_t checked(int status, uint32_t value)
{
    if (status != 0) __builtin_trap();
    return value;
}
static uint32_t addition(uint32_t a, uint32_t b, int subtract)
{
    uint32_t aa=a&0x7fffffffu, bb=b&0x7fffffffu;
    if (aa>INFINITY_WORD || bb>INFINITY_WORD) return NAN_WORD;
    if (aa==INFINITY_WORD || bb==INFINITY_WORD) {
        uint32_t right=b^(subtract ? 0x80000000u : 0);
        if (aa==INFINITY_WORD && bb==INFINITY_WORD)
            return (a^right)&0x80000000u ? NAN_WORD : a;
        return aa==INFINITY_WORD ? a : right;
    }
    uint32_t policy=POLICY, value=0;
    int status=sa_sh4_add_sub(&a,&b,&policy,&value,subtract);
    return checked(status,value);
}
uint32_t jp_rz_scalar32_add(uint32_t a,uint32_t b) { return addition(a,b,0); }
uint32_t jp_rz_scalar32_subtract(uint32_t a,uint32_t b) { return addition(a,b,1); }
uint32_t jp_rz_scalar32_multiply(uint32_t a,uint32_t b)
{
    uint32_t policy=POLICY, value=0;
    int status=sa_sh4_multiply_extended(&a,&b,&policy,&value);
    return checked(status,value);
}
uint32_t jp_rz_scalar32_divide(uint32_t a,uint32_t b)
{
    uint32_t aa=a&0x7fffffffu, bb=b&0x7fffffffu, sign=(a^b)&0x80000000u;
    if (aa>INFINITY_WORD || bb>INFINITY_WORD) return NAN_WORD;
    if (aa==INFINITY_WORD || bb==INFINITY_WORD) {
        if (aa==INFINITY_WORD && bb==INFINITY_WORD) return NAN_WORD;
        return aa==INFINITY_WORD ? sign|INFINITY_WORD : sign;
    }
    uint32_t policy=POLICY, value=0;
    int status=sa_sh4_divide(&a,&b,&policy,&value);
    return checked(status,value);
}
