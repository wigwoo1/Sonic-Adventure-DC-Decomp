#include "fpu_cleanup_transfer.h"

typedef void (*sa_value_observer)(void *, uint32_t, const uint8_t *,
    const struct sa_fpu_banks *, const uint32_t *, const uint32_t *, const uint32_t *);

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

/* Retail 8c07c9d2..8c07c9fa: 20 integer instructions, no external calls.
 * Names/field roles are inferred. Literal pointers and table entries remain
 * guest-memory reads. Writes and the stack reload retain their retail order,
 * including when guest objects alias the local stack word or pointer tables.
 * All integer arithmetic wraps; SR, PR and FPU state are untouched.
 */
SA_STAGE_EXPORT void sa_main_dispatch_value_write_8c07c9d2(uint8_t *ram, uint32_t *r)
{
    r[15] -= 4u;
    r[4] &= 0xffu;
    r[0] = read_value(ram, 0x8c07cb00u, 4);
    r[4] <<= 2;
    write_value(ram, r[15], r[4], 4);
    r[4] = read_value(ram, r[0] + r[4], 4);
    r[0] = read_value(ram, 0x8c07cb04u, 4);
    r[3] = read_value(ram, r[15], 4);
    r[2] = read_value(ram, r[0] + r[3], 4);
    r[3] = 0x4000u;
    write_value(ram, r[15], r[2], 4);
    r[3] -= r[6];
    write_value(ram, r[4] + 0x14u, r[5], 4);
    write_value(ram, r[4] + 0x18u, r[3], 4);
    r[2] = read_value(ram, r[15], 4);
    write_value(ram, r[2] + 0x20u, r[3], 4);
    write_value(ram, r[4] + 0x1cu, r[7], 4);
    r[15] += 4u;
}

/* Retail 8c07c298..8c07c2d0: 28 integer instructions plus the native leaf.
 * Preserve byte argument truncation/sign extension, PR/frame writes, and the
 * post-call table reload before clearing the pending byte at pointee +24.
 * There is no external-service dependency on this path. Observer is optional
 * read-only host instrumentation, not original game code.
 */
SA_STAGE_EXPORT void sa_main_dispatch_value_apply_8c07c298(
    uint8_t *ram, struct sa_fpu_banks *fpu, uint32_t *sr, uint32_t *r,
    uint32_t *pr, void *context, sa_value_observer observe)
{
    r[15] -= 4u;
    write_value(ram, r[15], *pr, 4);
    r[15] -= 0x10u;
    r[3] = read_value(ram, 0x8c07c3a0u, 4);
    write_value(ram, r[15], r[4], 1);
    write_value(ram, r[15] + 4u, r[5], 4);
    r[7] = read_value(ram, r[15], 1);
    r[7] <<= 2;
    r[7] += r[3];
    write_value(ram, r[15] + 0xcu, r[7], 4);
    r[7] = read_value(ram, r[7], 4);
    write_value(ram, r[15] + 8u, r[7], 4);
    r[5] = read_value(ram, r[15] + 8u, 4);
    r[6] = read_value(ram, r[15] + 4u, 4);
    r[5] = read_value(ram, r[5] + 0x14u, 4);
    r[7] = read_value(ram, r[7] + 0x1cu, 4);
    *pr = 0x8c07c2bcu;
    r[4] = (uint32_t)(int32_t)(int8_t)read_value(ram, r[15], 1);
    if (observe)
        observe(context, 0x8c07c9d2u, ram, fpu, sr, r, pr);
    sa_main_dispatch_value_write_8c07c9d2(ram, r);
    r[3] = read_value(ram, r[15] + 0xcu, 4);
    r[0] = 0x24u;
    r[2] = read_value(ram, r[3], 4);
    r[3] = 0u;
    r[1] = read_value(ram, r[2] + 0x3cu, 4);
    write_value(ram, r[1] + r[0], r[3], 1);
    r[15] += 0x10u;
    *pr = read_value(ram, r[15], 4);
    r[15] += 4u;
}
