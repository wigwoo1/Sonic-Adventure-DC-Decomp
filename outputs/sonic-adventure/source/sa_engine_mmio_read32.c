#include "fpu_cleanup_transfer.h"

typedef uint32_t (*sa_mmio_read32_fn)(void *context, uint32_t address);

/* Recovered retail register reader 8c653476..8c65347b.
 * The load in the RTS delay slot is modeled through the host MMIO service. */
SA_STAGE_EXPORT int sa_engine_mmio_read32(
    uint8_t *memory, struct sa_fpu_banks *fpu, uint32_t *sr,
    uint32_t *r, uint32_t *pr, uint32_t *fpul, uint32_t *macl,
    void *context, sa_mmio_read32_fn read_register)
{
    (void)memory;
    (void)fpu;
    (void)sr;
    (void)pr;
    (void)fpul;
    (void)macl;

    r[0] = 0xa05f8000u + r[4];
    if (read_register != 0)
        r[0] = read_register(context, r[0]);
    else
        r[0] = 0;
    return 0;
}
