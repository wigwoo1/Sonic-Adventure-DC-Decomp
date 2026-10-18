#include "fpu_cleanup_transfer.h"

typedef int (*sa_service_call)(void *, uint32_t, uint8_t *,
    struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *);

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = address & 0x00ffffffu;
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
        ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = address & 0x00ffffffu;
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

/* Retail wrapper at 8c038dfc..8c038e0c. The first target is called with a
 * saved return address; after restoring PR and SP, the second target is a
 * tail transfer and returns directly to the original caller. */
SA_STAGE_EXPORT int sa_callback_pair_tail_8c038dfc(
    uint8_t *ram, struct sa_fpu_banks *fpu, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *context,
    sa_service_call call)
{
    (void)fpul;
    (void)macl;
    r[15] -= 4u;
    write32(ram, r[15], *pr);

    r[0] = read32(ram, 0x8c038e0cu);
    *pr = 0x8c038e04u;
    int status = call(context, r[0], ram, fpu, sr, r, pr);
    if (status != 0)
        return status;

    *pr = read32(ram, r[15]);
    r[15] += 4u;
    r[0] = read32(ram, 0x8c038e10u);
    return call(context, r[0], ram, fpu, sr, r, pr);
}
