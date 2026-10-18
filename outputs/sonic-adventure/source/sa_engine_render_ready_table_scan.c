#include "fpu_cleanup_transfer.h"

static uint16_t read_halfword(const uint8_t *memory, uint32_t address)
{
    uint32_t offset = address & 0x00ffffffu;
    return (uint16_t)((uint16_t)memory[offset] |
                      ((uint16_t)memory[offset + 1] << 8));
}

static uint32_t read_word(const uint8_t *memory, uint32_t address)
{
    uint32_t offset = address & 0x00ffffffu;
    return (uint32_t)memory[offset] |
           ((uint32_t)memory[offset + 1] << 8) |
           ((uint32_t)memory[offset + 2] << 16) |
           ((uint32_t)memory[offset + 3] << 24);
}

/* Retail 0x8c650f78..0x8c650fd3: scan eight 32-byte entries per window. */
SA_STAGE_EXPORT int sa_engine_render_ready_table_scan(
    uint8_t *memory, struct sa_fpu_banks *fpu, uint32_t *sr,
    uint32_t *r, uint32_t *pr, uint32_t *fpul, uint32_t *macl)
{
    (void)fpu;
    (void)pr;
    (void)fpul;
    (void)macl;

    uint32_t window_start = read_word(memory, 0x8c650fe4u);
    uint32_t window_end = window_start + read_halfword(memory, 0x8c650fe0u);
    uint32_t step = read_halfword(memory, 0x8c650fe2u);
    uint32_t cursor = window_start;
    uint32_t key = r[4];
    r[5] = window_start;
    r[6] = window_start;
    r[7] = window_start;

    for (;;) {
        if ((int32_t)window_end <= (int32_t)cursor) {
            r[0] = 1;
            *sr = (*sr & ~1u) | 1u;
            return 1;
        }
        r[2] = 0;
        for (uint32_t slot = 0; slot < 8; ++slot) {
            uint32_t entry = cursor + slot * 32u;
            r[5] = entry;
            r[6] = entry;
            if (read_word(memory, entry) == key) {
                uint32_t flags = read_word(memory, entry + 24u);
                r[1] = flags;
                if ((flags & 3u) != 0) {
                    r[2] = key;
                    r[0] = 0;
                    *sr &= ~1u;
                    return 0;
                }
            }
        }
        cursor += step;
        r[5] = cursor - step + 256u;
        r[6] = r[5];
        r[7] = cursor;
    }
}
