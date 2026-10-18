#include "sa_engine_render_buffer_dispatch.h"

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

/* Retail 0x8c6536ec..0x8c65372f. Device/engine targets cross a host ABI. */
SA_STAGE_EXPORT int sa_engine_render_buffer_dispatch(
    uint8_t *memory, struct sa_fpu_banks *fpu, uint32_t *sr,
    uint32_t *r, uint32_t *pr, uint32_t *fpul, uint32_t *macl,
    void *context, sa_render_dispatch_call_fn call)
{
    (void)fpul;
    (void)macl;

    r[15] -= 4;
    write_word(memory, r[15], r[14]);
    r[15] -= 4;
    write_word(memory, r[15], r[13]);
    r[15] -= 4;
    write_word(memory, r[15], *pr);

    uint32_t descriptor = r[4];
    r[4] = descriptor;
    *pr = 0x8c6536f6u;
    int service_error = call(context, 0x8c653f08u, memory, fpu, sr, r, pr);
    if (service_error != 0)
        goto host_error;

    r[14] = r[4];
    r[4] = r[0];
    int result = (int32_t)r[4];
    if (result == 0) {
        if ((read_word(memory, r[14]) & 0xffffu) == 3u) {
            r[4] = r[14];
            *pr = 0x8c65370eu;
            service_error = call(context, 0x8c663712u,
                                 memory, fpu, sr, r, pr);
        } else {
            r[4] = read_word(memory, r[14] + 0x14u);
            r[5] = r[14];
            *pr = 0x8c65371au;
            service_error = call(context, 0x8c663540u,
                                 memory, fpu, sr, r, pr);
        }
        if (service_error != 0)
            goto host_error;

        result = (int32_t)r[0];
        r[13] = r[0];
        if (result == 3) {
            r[4] = 3;
            *pr = 0x8c653726u;
            service_error = call(context, 0x8c6524d2u,
                                 memory, fpu, sr, r, pr);
            if (service_error != 0)
                goto host_error;
        }
        r[0] = r[13];
    }

    *pr = read_word(memory, r[15]);
    r[15] += 4;
    r[13] = read_word(memory, r[15]);
    r[15] += 4;
    r[14] = read_word(memory, r[15]);
    r[15] += 4;
    return (int32_t)r[0];

host_error:
    r[0] = (uint32_t)service_error;
    *pr = read_word(memory, r[15]);
    r[15] += 4;
    r[13] = read_word(memory, r[15]);
    r[15] += 4;
    r[14] = read_word(memory, r[15]);
    r[15] += 4;
    return service_error;
}
