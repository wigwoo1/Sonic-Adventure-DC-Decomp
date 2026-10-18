#include "fpu_cleanup_transfer.h"

typedef int (*sa_event_call_fn)(void *, uint32_t, uint8_t *,
                                struct sa_fpu_banks *, uint32_t *,
                                uint32_t *, uint32_t *);
typedef int (*sa_event_mmio_write_fn)(void *, uint32_t, uint32_t);

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

/* Retail 0x8c6521ca..0x8c6521f1. */
SA_STAGE_EXPORT int sa_engine_event_count_callback(
    uint8_t *memory, struct sa_fpu_banks *fpu, uint32_t *sr,
    uint32_t *r, uint32_t *pr, uint32_t *fpul, uint32_t *macl,
    void *context, sa_event_mmio_write_fn write_mmio,
    sa_event_call_fn call)
{
    (void)fpul;
    (void)macl;

    r[15] -= 4;
    write_word(memory, r[15], *pr);
    r[1] = 0x8c8a31f8u;
    r[3] = read_word(memory, r[1]);
    uint32_t state = r[3];
    r[2] = 0xa05f6908u;
    if (write_mmio != 0)
        (void)write_mmio(context, r[2], 8u);

    r[4] = 0x8c8a323cu;
    uint32_t counter = r[4] + 0x1cu;
    r[3] = read_word(memory, counter) + 1u;
    write_word(memory, counter, r[3]);
    if (state == 1u) {
        counter = r[4] + 0x10u;
        write_word(memory, counter, read_word(memory, counter) + 1u);
        r[4] = 1;
        *pr = 0x8c6521ecu;
        if (call != 0) {
            int service_error = call(context, 0x8c652034u, memory, fpu,
                                     sr, r, pr);
            if (service_error != 0) {
                r[0] = (uint32_t)service_error;
                goto finish;
            }
        }
    }
    r[0] = 0;
    *sr = (*sr & ~1u) | (state == 1u ? 1u : 0u);
finish:
    *pr = read_word(memory, r[15]);
    r[15] += 4;
    return (int32_t)r[0];
}
