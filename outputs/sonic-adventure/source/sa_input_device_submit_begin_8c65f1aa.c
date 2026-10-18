#include "fpu_cleanup_transfer.h"

typedef int (*sa_service_call)(void *, uint32_t, uint8_t *,
    struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *);

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = address & 0x00ffffffu;
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
        ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static int32_t read_signed16(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = address & 0x00ffffffu;
    const uint16_t value = (uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8);
    return (int16_t)value;
}

static uint8_t read8(const uint8_t *ram, uint32_t address)
{
    return ram[address & 0x00ffffffu];
}

static void write8(uint8_t *ram, uint32_t address, uint8_t value)
{
    ram[address & 0x00ffffffu] = value;
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = address & 0x00ffffffu;
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

/* Static recovery of retail input-submit preflight 8c65f1aa..8c65f1ec.
 * It checks request status and device flags, claims the shared lock, and reports
 * a busy-lock error through the caller-supplied service dispatcher. */
SA_STAGE_EXPORT int sa_input_device_submit_begin_8c65f1aa(
    uint8_t *ram, struct sa_fpu_banks *fpu, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *context,
    sa_service_call call)
{
    (void)fpul;
    (void)macl;
    uint32_t target;
    int status;

    /* 8c65f1aa..8c65f1b6: inspect request status and device flag word. */
    r[3] = read32(ram, 0x8c65f290u);
    r[0] = (uint32_t)read_signed16(ram, 0x8c65f286u);
    r[15] -= 4u;
    write32(ram, r[15], *pr);
    r[4] = read32(ram, r[3]);
    r[2] = read32(ram, r[0] + r[4]);
    *sr = (*sr & ~1u) | (uint32_t)(r[2] == 0u);
    if ((*sr & 1u) == 0u) {
        r[0] = 0xfffffffdu;
        goto return_with_pr;
    }

    r[0] = (uint32_t)read_signed16(ram, 0x8c65f288u);
    r[3] = read32(ram, r[0] + r[4]);
    r[0] = read32(ram, r[3] + 0x18u);
    *sr = (*sr & ~1u) | (uint32_t)((r[0] & 1u) == 0u);
    if ((*sr & 1u) == 0u) {
        r[0] = 0xffffffffu;
        goto return_with_pr;
    }

    /* 8c65f1ce..8c65f1d4: atomically set the lock's busy bit. */
    r[3] = read32(ram, 0x8c65f28cu);
    r[0] = read32(ram, r[3]);
    {
        const uint8_t old = read8(ram, r[0]);
        write8(ram, r[0], (uint8_t)(old | 0x80u));
        *sr = (*sr & ~1u) | (uint32_t)(old == 0u);
    }
    if ((*sr & 1u) != 0u) {
        r[0] = 0u;
        goto return_with_pr;
    }

    /* 8c65f1d6..8c65f1e2: report the busy semaphore and return -2. */
    r[1] = read32(ram, 0x8c65f294u);
    *pr = 0x8c65f1dcu;
    r[15] -= 4u;
    write32(ram, r[15], r[1]);
    target = 0x8c65f348u;
    status = call(context, target, ram, fpu, sr, r, pr);
    if (status != 0)
        return status;
    r[15] += 4u;
    *pr = read32(ram, r[15]);
    r[15] += 4u;
    r[0] = 0xfffffffeu;
    return 0;

return_with_pr:
    /* Shared early-return epilogue at 8c65f1e4..8c65f1ec. */
    *pr = read32(ram, r[15]);
    r[15] += 4u;
    return 0;
}
