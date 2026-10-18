#include "fpu_cleanup_transfer.h"

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = address & 0x00ffffffu;
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
        ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

/* Retail helper at 8c10e566..8c10e576. Mode 1 terminates the output string. */
SA_STAGE_EXPORT int sa_format_terminate_text_8c10e566(
    uint8_t *ram, struct sa_fpu_banks *fpu, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *context,
    int (*call)(void *, uint32_t, uint8_t *, struct sa_fpu_banks *,
                uint32_t *, uint32_t *, uint32_t *))
{
    (void)fpu;
    (void)pr;
    (void)fpul;
    (void)macl;
    (void)context;
    (void)call;
    r[0] = read32(ram, r[4]);
    const uint32_t terminate = r[0] == 1u;
    *sr = (*sr & ~1u) | terminate;
    if (terminate) {
        r[2] = read32(ram, r[4] + 4u);
        r[3] = 0u;
        ram[r[2] & 0x00ffffffu] = (uint8_t)r[3];
    }
    return 0;
}
