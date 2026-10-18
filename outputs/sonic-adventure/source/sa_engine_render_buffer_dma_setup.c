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

typedef int (*sa_dma_service_fn)(void *, uint32_t, uint8_t *,
                                 struct sa_fpu_banks *, uint32_t *,
                                 uint32_t *, uint32_t *);
typedef int (*sa_cache_writeback_fn)(void *, uint32_t);

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

/* Recovered renderer setup/DMA routine 8c6534e0..8c6535c1.
 * DMA queueing, device completion and cache operations remain host services. */
SA_STAGE_EXPORT int sa_engine_render_buffer_dma_setup(
    uint8_t *memory, struct sa_fpu_banks *fpu, uint32_t *sr,
    uint32_t *r, uint32_t *pr, uint32_t *fpul, uint32_t *macl,
    const struct sa_input_io *io, void *context,
    sa_dma_service_fn service, sa_cache_writeback_fn cache_writeback)
{
    r[15] -= 4; write_word(memory, r[15], r[14]);
    r[15] -= 4; write_word(memory, r[15], r[13]);
    r[15] -= 4; write_word(memory, r[15], r[12]);
    r[15] -= 4; write_word(memory, r[15], r[11]);
    r[15] -= 4; write_word(memory, r[15], r[10]);
    r[15] -= 4; write_word(memory, r[15], r[9]);
    r[15] -= 4; write_word(memory, r[15], r[8]);
    r[15] -= 4; write_word(memory, r[15], *pr);
    r[15] -= 4;

    r[0] = *sr;
    r[12] = r[4];
    r[10] = 0x1fu;
    r[3] = r[12];
    r[11] = r[6];
    r[0] >>= 2;
    r[14] = r[5];
    r[0] >>= 2;
    *sr = (*sr & ~1u) | ((r[3] & r[10]) == 0);
    r[0] &= 0xfu;

    if (!(*sr & 1u))
        goto fallback;
    r[4] = r[0];
    r[1] = r[14];
    *sr = (*sr & ~1u) | ((r[1] & r[10]) == 0);
    if (!(*sr & 1u))
        goto fallback;
    *sr = (*sr & ~1u) | ((r[4] & r[4]) == 0);
    if (!(*sr & 1u))
        goto fallback;

    r[5] = r[11];
    *pr = 0x8c65351au;
    r[4] = r[14];
    {
        int status = sa_engine_cache_writeback_range(
            memory, fpu, sr, r, pr, fpul, macl, context, cache_writeback);
        if (status != 0)
            return status;
    }
    r[4] = r[14];
    r[13] = 0xffffffe0u;
    r[13] &= r[11];
    *sr = (*sr & ~1u) | (r[13] == 0);
    if (*sr & 1u) {
        r[3] = 0x8c8a3178u;
        r[0] = 0;
        write_word(memory, r[3], r[0]);
        goto remainder;
    }

    r[8] = 0x11000000u;
    r[9] = 0x100u;
    r[8] += r[12];
    r[3] = 0x8c650bc8u;
    r[2] = r[15];
    r[5] = r[14];
    r[6] = r[13];
    for (;;) {
        r[15] -= 4;
        write_word(memory, r[15], r[2]);
        r[7] = 0x18u;
        *pr = 0x8c653538u;
        r[4] = r[8];
        if (service != 0) {
            int status = service(context, r[3], memory, fpu, sr, r, pr);
            if (status != 0)
                return status;
        }
        *sr = (*sr & ~1u) | (r[0] == r[9]);
        r[15] += 4;
        if (!(*sr & 1u))
            break;
    }
    r[3] = read_word(memory, r[15]);
    r[1] = 0x8c8a3178u;
    r[2] = read_word(memory, r[3]);
    write_word(memory, r[1], r[2]);

remainder:
    r[6] = r[11] & r[10];
    *sr = (*sr & ~1u) | (r[6] == 0);
    if (!(*sr & 1u)) {
        r[4] = 0xa4000000u;
        r[14] += r[13];
        r[3] = 0x8c65253eu;
        r[12] += r[13];
        r[5] = r[14];
        *pr = 0x8c653564u;
        r[4] += r[12];
        {
            int status = sa_engine_word_copy(memory, fpu, sr, r, pr,
                                              fpul, macl, io, context, 0);
            if (status != 0)
                return status;
        }
    }
    r[2] = 0x8c8a317cu;
    r[0] = read_word(memory, r[2]);
    *sr = (*sr & ~1u) | (r[0] == 1u);
    if (*sr & 1u) {
        r[3] = 0x8c651000u;
        r[1] = 0x8c8a3178u;
        *pr = 0x8c653574u;
        r[4] = read_word(memory, r[1]);
        if (service != 0) {
            int status = service(context, r[3], memory, fpu, sr, r, pr);
            if (status != 0)
                return status;
        }
    }
    goto finish;

fallback:
    r[4] = 0xa4000000u;
    r[6] = r[11];
    r[3] = 0x8c65253eu;
    r[5] = r[14];
    *pr = 0x8c6535acu;
    r[4] += r[12];
    {
        int status = sa_engine_word_copy(memory, fpu, sr, r, pr,
                                          fpul, macl, io, context, 0);
        if (status != 0)
            return status;
    }

finish:
    r[0] = 1;
    r[15] += 4;
    *pr = read_word(memory, r[15]); r[15] += 4;
    r[8] = read_word(memory, r[15]); r[15] += 4;
    r[9] = read_word(memory, r[15]); r[15] += 4;
    r[10] = read_word(memory, r[15]); r[15] += 4;
    r[11] = read_word(memory, r[15]); r[15] += 4;
    r[12] = read_word(memory, r[15]); r[15] += 4;
    r[13] = read_word(memory, r[15]); r[15] += 4;
    r[14] = read_word(memory, r[15]); r[15] += 4;
    return 0;
}
