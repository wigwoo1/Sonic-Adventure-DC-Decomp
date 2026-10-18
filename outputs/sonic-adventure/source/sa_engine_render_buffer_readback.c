#include "fpu_cleanup_transfer.h"

typedef int (*sa_dispatch_call_fn)(
    void *, uint32_t, uint8_t *, struct sa_fpu_banks *, uint32_t *,
    uint32_t *, uint32_t *);

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

/* Recovered wrapper 8c653492..8c6534a7 for VRAM-to-RAM transfers. */
SA_STAGE_EXPORT int sa_engine_render_buffer_readback(
    uint8_t *memory, struct sa_fpu_banks *fpu, uint32_t *sr,
    uint32_t *r, uint32_t *pr, uint32_t *fpul, uint32_t *macl,
    void *context, sa_dispatch_call_fn call)
{
    (void)fpul;
    (void)macl;

    r[15] -= 4;
    write_word(memory, r[15], *pr);
    r[15] -= 4;
    r[2] = read_word(memory, 0x8c6534ccu);
    r[3] = read_word(memory, 0x8c6534c8u);
    write_word(memory, r[15], r[5]);
    *pr = 0x8c65349eu;
    r[5] += r[3];
    if (call != 0) {
        int status = call(context, r[2], memory, fpu, sr, r, pr);
        if (status != 0)
            return status;
    }
    r[15] += 4;
    *pr = read_word(memory, r[15]);
    r[15] += 4;
    r[0] = 1;
    return 0;
}
