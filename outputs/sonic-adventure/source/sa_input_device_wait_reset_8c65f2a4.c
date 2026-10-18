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

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = address & 0x00ffffffu;
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static void set_t(uint32_t *sr, uint32_t value)
{
    *sr = (*sr & ~1u) | (value & 1u);
}

/* Retail SH-4 recovery for 8c65f2a4..8c65f348. The two small time helpers
 * remain explicit dispatches and are independently recovered elsewhere. */
SA_STAGE_EXPORT int sa_input_device_wait_reset_8c65f2a4(
    uint8_t *ram, struct sa_fpu_banks *fpu, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *context,
    sa_service_call call)
{
    (void)fpul;
    (void)macl;

    const uint32_t saved_r14 = r[14];
    const uint32_t saved_r13 = r[13];
    const uint32_t saved_r12 = r[12];
    const uint32_t saved_r11 = r[11];
    const uint32_t saved_r10 = r[10];
    const uint32_t saved_r9 = r[9];
    const uint32_t saved_pr = *pr;

    r[15] -= 4u; write32(ram, r[15], saved_r14);
    r[15] -= 4u; write32(ram, r[15], saved_r13);
    r[15] -= 4u; write32(ram, r[15], saved_r12);
    r[15] -= 4u; write32(ram, r[15], saved_r11);
    r[15] -= 4u; write32(ram, r[15], saved_r10);
    r[15] -= 4u; write32(ram, r[15], saved_r9);
    r[15] -= 4u; write32(ram, r[15], saved_pr);

    /* Start time, then wait while the device state has bit zero set. */
    r[13] = 0x8c60260eu;
    *pr = 0x8c65f2b8u;
    int status = call(context, r[13], ram, fpu, sr, r, pr);
    if (status != 0)
        return status;
    r[9] = r[0];
    r[11] = 0x8c602618u;
    r[14] = read32(ram, 0x8c65f3d8u);
    r[10] = read32(ram, 0x8c65f3d0u);
    const uint32_t field_offset = (uint32_t)(int32_t)(int16_t)
        read16(ram, 0x8c65f3c0u);

    for (;;) {
        r[12] = 1u;
        r[0] = field_offset;
        r[2] = read32(ram, r[14]);
        r[3] = read32(ram, r[2] + r[0]);
        r[1] = read32(ram, r[3] + 0x18u);
        const uint32_t state_clear = (r[1] & r[12]) == 0u;
        set_t(sr, state_clear);
        if (state_clear)
            break;

        *pr = 0x8c65f2c8u;
        status = call(context, r[13], ram, fpu, sr, r, pr);
        if (status != 0)
            return status;
        r[5] = r[0];
        r[4] = r[9];
        *pr = 0x8c65f2ceu;
        status = call(context, r[11], ram, fpu, sr, r, pr);
        if (status != 0)
            return status;
        const uint32_t timed_out = (int32_t)r[0] > (int32_t)r[10];
        set_t(sr, timed_out);
        if (timed_out)
            break;
    }

    /* Reset the device state record and install its three retail constants. */
    r[0] = field_offset;
    r[4] = 0u;
    r[2] = read32(ram, r[14]);
    r[3] = read32(ram, r[2] + r[0]);
    write32(ram, r[3] + 0x18u, r[4]);
    r[3] = read32(ram, r[14]);
    r[2] = read32(ram, r[3] + r[0]);
    write32(ram, r[2] + 0x14u, r[4]);
    r[3] = read32(ram, r[14]);
    r[2] = read32(ram, r[3] + r[0]);
    write32(ram, r[2] + 0x10u, r[4]);
    r[3] = read32(ram, r[14]);
    r[1] = read32(ram, 0x8c65f3dcu);
    r[2] = read32(ram, r[3] + r[0]);
    r[0] = (uint32_t)(int32_t)(int16_t)read16(ram, 0x8c65f3c2u);
    write32(ram, r[2] + r[0], r[1]);
    r[0] = field_offset;
    r[3] = read32(ram, r[14]);
    r[1] = read32(ram, 0x8c65f3e0u);
    r[2] = read32(ram, r[3] + r[0]);
    r[0] = (uint32_t)(int32_t)(int16_t)read16(ram, 0x8c65f3c4u);
    write32(ram, r[2] + r[0], r[1]);
    r[0] = field_offset;
    r[3] = read32(ram, r[14]);
    r[1] = read32(ram, 0x8c65f3e4u);
    r[2] = read32(ram, r[3] + r[0]);
    write32(ram, r[2] + 4u, r[1]);

    *pr = 0x8c65f316u;
    status = call(context, r[13], ram, fpu, sr, r, pr);
    if (status != 0)
        return status;
    r[9] = r[0];
    r[10] = (uint32_t)(int32_t)(int16_t)read16(ram, 0x8c65f3c6u);

    for (;;) {
        r[0] = field_offset;
        r[2] = read32(ram, r[14]);
        r[3] = read32(ram, r[2] + r[0]);
        r[1] = read32(ram, r[3] + 0x18u);
        const uint32_t state_clear = (r[1] & 1u) == 0u;
        set_t(sr, state_clear);
        if (state_clear)
            break;

        *pr = 0x8c65f320u;
        status = call(context, r[13], ram, fpu, sr, r, pr);
        if (status != 0)
            return status;
        r[5] = r[0];
        r[4] = r[9];
        *pr = 0x8c65f326u;
        status = call(context, r[11], ram, fpu, sr, r, pr);
        if (status != 0)
            return status;
        const uint32_t timed_out = (int32_t)r[0] > (int32_t)r[10];
        set_t(sr, timed_out);
        if (timed_out)
            break;
    }

    /* Retail epilogue: restore PR and the six callee-saved registers. */
    *pr = read32(ram, r[15]); r[15] += 4u;
    r[9] = read32(ram, r[15]); r[15] += 4u;
    r[10] = read32(ram, r[15]); r[15] += 4u;
    r[11] = read32(ram, r[15]); r[15] += 4u;
    r[12] = read32(ram, r[15]); r[15] += 4u;
    r[13] = read32(ram, r[15]); r[15] += 4u;
    r[14] = read32(ram, r[15]); r[15] += 4u;
    (void)saved_r14; (void)saved_r13; (void)saved_r12;
    (void)saved_r11; (void)saved_r10; (void)saved_r9; (void)saved_pr;
    r[0] = 0u;
    return 0;
}
