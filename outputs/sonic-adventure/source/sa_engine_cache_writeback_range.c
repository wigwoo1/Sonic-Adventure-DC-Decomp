#include "fpu_cleanup_transfer.h"

typedef int (*sa_cache_writeback_fn)(void *context, uint32_t address);

static uint32_t read_word(const uint8_t *memory, uint32_t address)
{
    uint32_t offset = address & 0x00ffffffu;
    return (uint32_t)memory[offset] |
           ((uint32_t)memory[offset + 1] << 8) |
           ((uint32_t)memory[offset + 2] << 16) |
           ((uint32_t)memory[offset + 3] << 24);
}

static void write_word(uint8_t *memory, uint32_t address, uint32_t value)
{
    uint32_t offset = address & 0x00ffffffu;
    memory[offset] = (uint8_t)value;
    memory[offset + 1] = (uint8_t)(value >> 8);
    memory[offset + 2] = (uint8_t)(value >> 16);
    memory[offset + 3] = (uint8_t)(value >> 24);
}

/* Recovered range wrapper 8c65344e..8c653475.
 * The Dreamcast cache instruction is represented by a host service callback. */
SA_STAGE_EXPORT int sa_engine_cache_writeback_range(
    uint8_t *memory, struct sa_fpu_banks *fpu, uint32_t *sr,
    uint32_t *r, uint32_t *pr, uint32_t *fpul, uint32_t *macl,
    void *context, sa_cache_writeback_fn writeback)
{
    (void)fpu;
    (void)fpul;
    (void)macl;

    r[15] -= 4;
    write_word(memory, r[15], r[14]);
    r[14] = r[4];
    r[15] -= 4;
    write_word(memory, r[15], r[13]);
    r[13] = r[4];
    r[15] -= 4;
    write_word(memory, r[15], r[12]);
    r[13] += r[5];
    r[15] -= 4;
    write_word(memory, r[15], *pr);
    r[12] = read_word(memory, 0x8c6534c4u);

    *sr = (*sr & ~1u) | (r[14] > r[13]);
    if (!(*sr & 1u)) {
        for (;;) {
            r[4] = r[14];
            *pr = 0x8c653466u;
            if (writeback != 0)
                (void)writeback(context, r[14]);
            r[14] += 0x20u;
            *sr = (*sr & ~1u) | (r[14] > r[13]);
            if (*sr & 1u)
                break;
        }
    }

    *pr = read_word(memory, r[15]);
    r[15] += 4;
    r[12] = read_word(memory, r[15]);
    r[15] += 4;
    r[13] = read_word(memory, r[15]);
    r[15] += 4;
    r[14] = read_word(memory, r[15]);
    r[15] += 4;
    return 0;
}
