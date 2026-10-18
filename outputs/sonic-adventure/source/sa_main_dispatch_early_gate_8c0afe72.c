#include "fpu_cleanup_transfer.h"

typedef int (*sa_gate_service)(void *, uint32_t, uint8_t *,
    struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *);

static uint32_t read_value(const uint8_t *ram, uint32_t address, unsigned width)
{
    uint32_t value = 0;
    const uint32_t at = address & 0x00ffffffu;
    for (unsigned i = 0; i < width; ++i)
        value |= (uint32_t)ram[at + i] << (8u * i);
    return value;
}

static void write_value(uint8_t *ram, uint32_t address, uint32_t value, unsigned width)
{
    const uint32_t at = address & 0x00ffffffu;
    for (unsigned i = 0; i < width; ++i)
        ram[at + i] = (uint8_t)(value >> (8u * i));
}

/* Retail 8c0afe72..8c0aff1a, 55 instructions excluding the literal-pool gap.
 * Descriptive name/field roles are inferred, not original source symbols.
 * Modes 1..4 set bit 0x4000; modes 5/6 call the external literal target;
 * other modes clear bit 0x4000. Integer arithmetic wraps at 32 bits.
 * RAM/GPR/SR/PR are explicit to preserve the retail ABI and stack writes.
 * Host status 0 means completion, with retail 0/1 result in R0. A missing
 * service returns -2 without mutation; a service error preserves partial
 * guest state at its return boundary. No FPU instructions occur here.
 */
SA_STAGE_EXPORT int sa_main_dispatch_early_gate_8c0afe72(
    uint8_t *ram, struct sa_fpu_banks *fpu, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *context,
    sa_gate_service call)
{
    (void)fpul;
    (void)macl;
    if (!call)
        return -2;

    /* 8c0afe72..8c0afe80: save R14/R13/PR, then read the unsigned mode. */
    r[15] -= 4u;
    write_value(ram, r[15], r[14], 4);
    r[14] = r[4];
    r[15] -= 4u;
    write_value(ram, r[15], r[13], 4);
    r[0] = 0x24u;
    r[15] -= 4u;
    write_value(ram, r[15], *pr, 4);
    r[13] = read_value(ram, r[14] + 0x3cu, 4);
    r[0] = read_value(ram, r[13] + r[0], 1);

    /* 8c0afe82..8c0afe98 compare in this exact short-circuit order. */
    *sr = (*sr & ~1u) | (r[0] == 2u);
    if (!(*sr & 1u)) {
        *sr = (*sr & ~1u) | (r[0] == 3u);
        if (!(*sr & 1u)) {
            *sr = (*sr & ~1u) | (r[0] == 4u);
            if (!(*sr & 1u))
                *sr = (*sr & ~1u) | (r[0] == 1u);
        }
    }
    if (*sr & 1u) {
        /* 8c0afe9e..8c0afea8: MOV.W sign extension is visible in R0. */
        r[0] = (uint32_t)(int32_t)(int16_t)read_value(ram, r[14] + 4u, 2);
        r[3] = 0x4000u;
        r[0] |= r[3];
        write_value(ram, r[14] + 4u, r[0], 2);
        r[0] = 1u;
    } else {
        *sr = (*sr & ~1u) | (r[0] == 6u);
        if (*sr & 1u) {
            /* BRA delay slot at 8c0afeac supplies the unchanged value. */
            r[5] = read_value(ram, r[13] + 0x2cu, 4);
        } else {
            *sr = (*sr & ~1u) | (r[0] == 5u);
            if (!(*sr & 1u)) {
                /* 8c0aff00..8c0aff0e: clear bit, return retail zero. */
                r[3] = 0xffffbfffu;
                r[0] = (uint32_t)(int32_t)(int16_t)read_value(ram, r[14] + 4u, 2);
                r[0] &= r[3];
                write_value(ram, r[14] + 4u, r[0], 2);
                r[0] = 0u;
                goto restore;
            }
            /* 8c0afeae..8c0afeb8: mode five uses a wrapping difference. */
            r[5] = read_value(ram, r[13] + 0x2cu, 4);
            r[2] = 0x4000u;
            r[3] = read_value(ram, r[14] + 0x18u, 4);
            r[5] = r[5] - r[3] + r[2];
        }
        /* 8c0afeba..8c0afec0: target captured before the JSR delay slot. */
        r[3] = read_value(ram, 0x8c0afef8u, 4);
        r[0] = (uint32_t)(int32_t)(int8_t)read_value(ram, r[14] + 8u, 1);
        const uint32_t target = r[3];
        *pr = 0x8c0afec2u;
        r[4] = r[0];
        int status = call(context, target, ram, fpu, sr, r, pr);
        if (status != 0)
            return status;
        r[0] = 1u;
    }

restore:
    /* 8c0aff12..8c0aff18, including the RTS delay-slot R14 restore. */
    *pr = read_value(ram, r[15], 4);
    r[15] += 4u;
    r[13] = read_value(ram, r[15], 4);
    r[15] += 4u;
    r[14] = read_value(ram, r[15], 4);
    r[15] += 4u;
    return 0;
}
