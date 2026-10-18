#include "fpu_cleanup_transfer.h"

typedef int (*sa_service_call)(void *, uint32_t, uint8_t *,
    struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *);

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = address & 0x00ffffffu;
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
        ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static uint16_t read16(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = address & 0x00ffffffu;
    return (uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8);
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

/* Static SH-4 recovery of retail input service entry 8c647198..8c6471f4.
 * Calls at 8c6604c0, 8c646f68, 8c646654 and the error callback are dispatched
 * through the supplied service interface so callers can use recovered helpers. */
SA_STAGE_EXPORT int sa_input_service_entry_8c647198(
    uint8_t *ram, struct sa_fpu_banks *fpu, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *context,
    sa_service_call call)
{
    (void)fpul;
    (void)macl;
    uint32_t target;
    int status;

    /* 8c647198..8c6471a6: save caller state and test/set the semaphore. */
    r[15] -= 4u;
    write32(ram, r[15], r[14]);
    r[15] -= 4u;
    write32(ram, r[15], *pr);
    r[15] -= 8u;
    r[3] = read32(ram, 0x8c64727cu);
    write32(ram, r[15], r[4]);
    write32(ram, r[15] + 4u, r[7]);
    r[0] = read32(ram, r[3]);
    {
        const uint8_t old = read8(ram, r[0]);
        write8(ram, r[0], (uint8_t)(old | 0x80u));
        *sr = (*sr & ~1u) | (uint32_t)(old == 0u);
    }

    if ((*sr & 1u) == 0u) {
        /* 8c6471aa..8c6471b0: report a busy semaphore, passing the message
         * in the retail stack argument slot before the indirect call. */
        r[2] = read32(ram, 0x8c647284u);
        r[1] = read32(ram, 0x8c647280u);
        target = r[2];
        *pr = 0x8c6471b2u;
        r[15] -= 4u;
        write32(ram, r[15], r[1]);
        status = call(context, target, ram, fpu, sr, r, pr);
        if (status != 0)
            return status;
        r[15] += 4u;
        r[0] = 0xfffffffeu;
        goto epilogue;
    }

    /* 8c6471be..8c6471c4: invoke the configured device service. */
    r[3] = read32(ram, 0x8c647288u);
    target = r[3];
    *pr = 0x8c6471c4u;
    r[4] = read32(ram, r[15]);
    status = call(context, target, ram, fpu, sr, r, pr);
    if (status != 0)
        return status;

    /* 8c6471c4..8c6471d0: preserve the result and store positive requests. */
    r[14] = r[0];
    *sr = (*sr & ~1u) | (uint32_t)((int32_t)r[14] > 0);
    if ((*sr & 1u) != 0u) {
        r[5] = read32(ram, r[15] + 4u);
        r[6] = r[14];
        *pr = 0x8c6471d2u;
        r[4] = read32(ram, r[15]);
        status = call(context, 0x8c646f68u, ram, fpu, sr, r, pr);
        if (status != 0)
            return status;
    }

    /* 8c6471d2..8c6471e2: release the semaphore and read the cleanup flag. */
    r[3] = read32(ram, 0x8c64727cu);
    r[1] = 0u;
    r[0] = read32(ram, 0x8c64728cu);
    r[2] = read32(ram, r[3]);
    write8(ram, r[2], (uint8_t)r[1]);
    r[2] = read32(ram, r[0]);
    r[0] = (uint32_t)(int32_t)(int16_t)read16(ram, 0x8c647278u);
    r[1] = read32(ram, r[0] + r[2]);
    *sr = (*sr & ~1u) | (uint32_t)(r[1] == 0u);
    if ((*sr & 1u) == 0u) {
        /* 8c6471e6..8c6471e8: optional state cleanup. */
        *pr = 0x8c6471eau;
        r[0] = 0u;
        status = call(context, 0x8c646654u, ram, fpu, sr, r, pr);
        if (status != 0)
            return status;
    }

    r[0] = r[14];

epilogue:
    /* Retail epilogues restore PR and R14 from the saved stack slots. */
    r[15] += 8u;
    *pr = read32(ram, r[15]);
    r[15] += 4u;
    r[14] = read32(ram, r[15]);
    r[15] += 4u;
    return 0;
}
