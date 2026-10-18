#include "fpu_cleanup_transfer.h"

typedef int (*sa_service_call)(void *, uint32_t, uint8_t *,
    struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *);

int sa_debug_log_forward_8c10d400(uint8_t *, struct sa_fpu_banks *, uint32_t *,
    uint32_t *, uint32_t *, uint32_t *, uint32_t *, void *, sa_service_call);

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

/* Retail callback at 8c65f348..8c65f38e. Its report/notify services are
 * dispatched explicitly because the retail targets are external to this body. */
SA_STAGE_EXPORT int sa_input_device_report_callback_8c65f348(
    uint8_t *ram, struct sa_fpu_banks *fpu, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *context,
    sa_service_call call)
{
    (void)fpul;
    (void)macl;
    const uint32_t saved_r14 = r[14];
    const uint32_t saved_pr = *pr;
    r[2] = read32(ram, 0x8c65f3d8u);
    r[0] = (uint32_t)(int32_t)(int16_t)read16(ram, 0x8c65f3c8u);
    r[15] -= 4u; write32(ram, r[15], saved_r14);
    r[15] -= 4u; write32(ram, r[15], saved_pr);
    r[3] = read32(ram, r[2]);
    r[1] = read32(ram, r[3] + r[0]);
    set_t(sr, r[1] == 0u);

    if (r[1] != 0u) {
        r[0] = r[15] + 8u;
        r[5] = 3u;
        r[0] += 4u;
        const uint32_t aligned = (r[0] & r[5]) == 0u;
        set_t(sr, aligned);
        if (aligned) {
            r[5] = r[15] + 8u;
            r[5] += 4u;
        } else {
            /* The short branch's delay slot adds another eight bytes. */
            r[5] = r[15] + 16u;
        }
        r[3] = read32(ram, 0x8c65f3ecu);
        r[6] = r[5];
        r[4] = read32(ram, 0x8c65f3e8u);
        r[5] = read32(ram, r[15] + 8u);
        *pr = 0x8c65f37cu;
        int status = sa_debug_log_forward_8c10d400(
            ram, fpu, sr, r, pr, fpul, macl, context, call);
        if (status != 0)
            return status;

        r[2] = read32(ram, 0x8c65f3f0u);
        r[14] = read32(ram, r[2]);
        set_t(sr, r[14] == 0u);
        if (r[14] != 0u) {
            r[4] = read32(ram, 0x8c65f3e8u);
            *pr = 0x8c65f38au;
            status = call(context, r[14], ram, fpu, sr, r, pr);
            if (status != 0)
                return status;
        }
    }

    *pr = read32(ram, r[15]); r[15] += 4u;
    r[14] = read32(ram, r[15]); r[15] += 4u;
    (void)saved_r14;
    (void)saved_pr;
    return 0;
}
