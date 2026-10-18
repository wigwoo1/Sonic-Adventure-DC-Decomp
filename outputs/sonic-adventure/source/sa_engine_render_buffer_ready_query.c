#include "sa_engine_render_buffer_ready_query.h"

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

/* Retail 0x8c6533e0..0x8c653403. */
SA_STAGE_EXPORT int sa_engine_render_buffer_ready_query(
    uint8_t *memory, struct sa_fpu_banks *fpu, uint32_t *sr,
    uint32_t *r, uint32_t *pr, uint32_t *fpul, uint32_t *macl,
    void *context, sa_render_dispatch_call_fn call)
{
    (void)fpul;
    (void)macl;

    r[15] -= 4;
    write_word(memory, r[15], *pr);
    r[3] = 0x8c8a317cu;
    r[0] = read_word(memory, r[3]);
    int result = 0;
    if (r[0] == 1u) {
        r[0] = 0;
        *sr = (*sr & ~1u) | 1u;
    } else {
        r[2] = 0x8c650f78u;
        r[3] = 0x8c8a3178u;
        r[4] = read_word(memory, r[3]);
        *pr = 0x8c6533f0u;
        int service_error = call(context, r[2],
                                 memory, fpu, sr, r, pr);
        if (service_error != 0) {
            result = service_error;
            r[0] = (uint32_t)service_error;
        } else {
            result = (int32_t)r[0] == 1 ? 0 : 0x11;
            r[0] = (uint32_t)result;
            *sr = (*sr & ~1u) | (result == 0 ? 1u : 0u);
        }
    }
    *pr = read_word(memory, r[15]);
    r[15] += 4;
    return result;
}
