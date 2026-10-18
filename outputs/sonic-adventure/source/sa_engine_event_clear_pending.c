#include "fpu_cleanup_transfer.h"

typedef int (*sa_event_mmio_write_fn)(void *, uint32_t, uint32_t);

/* Retail 0x8c6521f2..0x8c65220f. */
SA_STAGE_EXPORT int sa_engine_event_clear_pending(
    uint8_t *memory, struct sa_fpu_banks *fpu, uint32_t *sr,
    uint32_t *r, uint32_t *pr, uint32_t *fpul, uint32_t *macl,
    void *context, sa_event_mmio_write_fn write_mmio)
{
    (void)fpu;
    (void)pr;
    (void)fpul;
    (void)macl;

    r[2] = 0xa05f6908u;
    if (write_mmio != 0)
        (void)write_mmio(context, r[2], 2u);

    r[5] = 0x8c8a3290u;
    r[3] = (uint32_t)memory[r[5] & 0x00ffffffu] |
           ((uint32_t)memory[(r[5] + 1u) & 0x00ffffffu] << 8) |
           ((uint32_t)memory[(r[5] + 2u) & 0x00ffffffu] << 16) |
           ((uint32_t)memory[(r[5] + 3u) & 0x00ffffffu] << 24);
    if (r[3] == 0) {
        r[3] = 0x8c8a2ffcu;
        uint32_t offset = r[3] & 0x00ffffffu;
        r[4] = (uint32_t)memory[offset] |
               ((uint32_t)memory[offset + 1] << 8) |
               ((uint32_t)memory[offset + 2] << 16) |
               ((uint32_t)memory[offset + 3] << 24);
        r[4] |= 0x10u;
        memory[offset] = (uint8_t)r[4];
        memory[offset + 1] = (uint8_t)(r[4] >> 8);
        memory[offset + 2] = (uint8_t)(r[4] >> 16);
        memory[offset + 3] = (uint8_t)(r[4] >> 24);
        r[4] = 0;
        *sr |= 1u;
    } else {
        r[4] = 0;
        *sr &= ~1u;
    }
    memory[0x8a3290] = 0;
    memory[0x8a3291] = 0;
    memory[0x8a3292] = 0;
    memory[0x8a3293] = 0;
    r[0] = 0;
    return 0;
}
