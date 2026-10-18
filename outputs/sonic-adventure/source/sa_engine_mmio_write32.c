#include "fpu_cleanup_transfer.h"

typedef int (*sa_mmio_write32_fn)(void *context, uint32_t address,
                                  uint32_t value);

/* Recovered retail register writer 8c653444..8c65344d.
 * The Dreamcast hardware register window is supplied by the host platform. */
SA_STAGE_EXPORT int sa_engine_mmio_write32(
    uint8_t *memory, struct sa_fpu_banks *fpu, uint32_t *sr,
    uint32_t *r, uint32_t *pr, uint32_t *fpul, uint32_t *macl,
    void *context, sa_mmio_write32_fn write_register)
{
    (void)memory;
    (void)fpu;
    (void)sr;
    (void)pr;
    (void)fpul;
    (void)macl;

    r[3] = 0xa05f8000u;
    r[4] += r[3];
    if (write_register != 0)
        (void)write_register(context, r[4], r[5]);
    r[0] = 1;
    return 0;
}
