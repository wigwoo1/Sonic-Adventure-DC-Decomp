#include "fpu_cleanup_transfer.h"

static uint32_t read_word(const uint8_t *memory, uint32_t address)
{
    uint32_t offset = address & 0x00ffffffu;
    return (uint32_t)memory[offset] |
           ((uint32_t)memory[offset + 1] << 8) |
           ((uint32_t)memory[offset + 2] << 16) |
           ((uint32_t)memory[offset + 3] << 24);
}

static uint32_t read_signed_halfword(const uint8_t *memory, uint32_t address)
{
    uint32_t offset = address & 0x00ffffffu;
    uint16_t value = (uint16_t)memory[offset] |
                     ((uint16_t)memory[offset + 1] << 8);
    return (uint32_t)(int32_t)(int16_t)value;
}

static void write_word(uint8_t *memory, uint32_t address, uint32_t value)
{
    uint32_t offset = address & 0x00ffffffu;
    memory[offset] = (uint8_t)value;
    memory[offset + 1] = (uint8_t)(value >> 8);
    memory[offset + 2] = (uint8_t)(value >> 16);
    memory[offset + 3] = (uint8_t)(value >> 24);
}

/* Recovered 1ST_READ.BIN routine 8c652e80..8c652ea0.
 * The external option dispatcher remains a service boundary. */
SA_STAGE_EXPORT int sa_engine_dispatch_option(
    uint8_t *memory, struct sa_fpu_banks *fpu, uint32_t *sr,
    uint32_t *r, uint32_t *pr, uint32_t *fpul, uint32_t *macl,
    void *context,
    int (*call)(void *, uint32_t, uint8_t *, struct sa_fpu_banks *,
                uint32_t *, uint32_t *, uint32_t *))
{
    (void)fpu;
    (void)fpul;
    (void)macl;

    uint32_t original_pr = *pr;
    r[15] -= 4;
    write_word(memory, r[15], r[14]);
    r[3] = 0xfffffffcu;
    r[15] -= 4;
    write_word(memory, r[15], original_pr);
    r[14] = r[4];
    *sr = (*sr & ~1u) | (((r[14] & r[3]) == 0) ? 1u : 0u);

    if (*sr & 1u) {
        r[2] = read_word(memory, 0x8c652f08u);
        write_word(memory, r[2], r[14]);
        r[3] = read_word(memory, 0x8c652f0cu);
        r[4] = read_signed_halfword(memory, 0x8c652f00u);
        r[5] = r[14];
        *pr = 0x8c652e98u;
        if (call != 0)
            (void)call(context, r[3], memory, fpu, sr, r, pr);
    }

    *pr = read_word(memory, r[15]);
    r[15] += 4;
    r[0] = 0;
    r[14] = read_word(memory, r[15]);
    r[15] += 4;
    return 0;
}
