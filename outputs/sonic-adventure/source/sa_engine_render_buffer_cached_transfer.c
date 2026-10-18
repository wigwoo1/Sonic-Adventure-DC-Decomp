#include "fpu_cleanup_transfer.h"
#include "input_io.h"

int sa_engine_word_copy(uint8_t *, struct sa_fpu_banks *, uint32_t *,
                        uint32_t *, uint32_t *, uint32_t *, uint32_t *,
                        const struct sa_input_io *, void *,
                        int (*)(void *, uint32_t, uint8_t *,
                                struct sa_fpu_banks *, uint32_t *,
                                uint32_t *, uint32_t *));
int sa_engine_cache_writeback_range(
    uint8_t *, struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *,
    uint32_t *, uint32_t *, void *, int (*)(void *, uint32_t));

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

/* Recovered 8c6535c2..8c6535e5 transfer: read via A4 aperture,
 * copy to RAM, then write back destination cache lines. */
SA_STAGE_EXPORT int sa_engine_render_buffer_cached_transfer(
    uint8_t *memory, struct sa_fpu_banks *fpu, uint32_t *sr,
    uint32_t *r, uint32_t *pr, uint32_t *fpul, uint32_t *macl,
    const struct sa_input_io *io, void *context,
    int (*cache_writeback)(void *, uint32_t))
{
    r[15] -= 4;
    write_word(memory, r[15], *pr);
    r[15] -= 12;
    write_word(memory, r[15], r[4]);
    write_word(memory, r[15] + 8, r[5]);
    write_word(memory, r[15] + 4, r[6]);
    r[3] = read_word(memory, 0x8c653620u);
    r[2] = read_word(memory, 0x8c653624u);
    r[5] = read_word(memory, r[15] + 8) + r[3];
    r[4] = read_word(memory, r[15]);
    *pr = 0x8c6535d8u;
    int status = sa_engine_word_copy(memory, fpu, sr, r, pr, fpul, macl,
                                     io, context, 0);
    if (status != 0)
        return status;

    r[4] = read_word(memory, r[15]);
    r[5] = read_word(memory, r[15] + 4);
    *pr = 0x8c6535deu;
    status = sa_engine_cache_writeback_range(
        memory, fpu, sr, r, pr, fpul, macl, context, cache_writeback);
    if (status != 0)
        return status;

    r[15] += 12;
    *pr = read_word(memory, r[15]);
    r[15] += 4;
    r[0] = 1;
    return 0;
}
