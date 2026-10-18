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

/* Retail 8c10d400..8c10d422 wrapper. It saves incoming values into its
 * outgoing stack frame, installs the fixed mode registers, and dispatches
 * through the runtime formatter pointer. */
SA_STAGE_EXPORT int sa_debug_log_forward_8c10d400(
    uint8_t *ram, struct sa_fpu_banks *fpu, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *context,
    sa_service_call call)
{
    (void)fpul;
    (void)macl;
    r[15] -= 4u;
    write32(ram, r[15], *pr);
    r[15] -= 12u;
    write32(ram, r[15], r[4]);
    r[3] = r[6];
    write32(ram, r[15] + 4u, r[5]);
    r[5] = 0u;
    write32(ram, r[15] + 8u, r[6]);
    r[15] -= 4u;
    write32(ram, r[15], r[6]);

    r[3] = read32(ram, 0x8c10d424u);
    r[6] = read32(ram, r[15] + 4u);
    r[7] = read32(ram, r[15] + 8u);
    r[4] = 1u;
    *pr = 0x8c10d41au;
    int status = call(context, r[3], ram, fpu, sr, r, pr);
    if (status != 0)
        return status;

    r[15] += 16u;
    *pr = read32(ram, r[15]);
    r[15] += 4u;
    return 0;
}
