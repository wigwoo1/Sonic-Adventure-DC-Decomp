#include "fpu_cleanup_transfer.h"

typedef int (*sa_service_call)(void *, uint32_t, uint8_t *,
    struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *);

int sa_byte_move(uint8_t *, struct sa_fpu_banks *, uint32_t *, uint32_t *,
    uint32_t *, uint32_t *, uint32_t *, void *, sa_service_call);

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = address & 0x00ffffffu;
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
        ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static int32_t read_s8(const uint8_t *ram, uint32_t address)
{
    return (int32_t)(int8_t)ram[address & 0x00ffffffu];
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

/* Retail text-output helper at 8c10e50e..8c10e566. Bulk mode uses the
 * independently recovered byte-move routine; callback mode emits one byte
 * at a time and stops when its callback returns -1. */
SA_STAGE_EXPORT int sa_format_emit_text_8c10e50e(
    uint8_t *ram, struct sa_fpu_banks *fpu, uint32_t *sr, uint32_t *r,
    uint32_t *pr, uint32_t *fpul, uint32_t *macl, void *context,
    sa_service_call call)
{
    const uint32_t saved_r14 = r[14];
    const uint32_t saved_r13 = r[13];
    const uint32_t saved_r12 = r[12];
    const uint32_t saved_r11 = r[11];
    const uint32_t saved_pr = *pr;
    r[15] -= 4u; write32(ram, r[15], saved_r14);
    r[15] -= 4u; write32(ram, r[15], saved_r13);
    r[15] -= 4u; write32(ram, r[15], saved_r12);
    r[15] -= 4u; write32(ram, r[15], saved_r11);
    r[15] -= 4u; write32(ram, r[15], saved_pr);
    r[14] = r[6];
    r[11] = r[4];
    r[0] = read32(ram, r[14]);
    const uint32_t bulk_mode = r[0] == 1u;
    set_t(sr, bulk_mode);
    r[13] = r[5];

    int status = 0;
    if (bulk_mode) {
        r[2] = read32(ram, 0x8c10e58cu);
        r[5] = r[11];
        r[6] = r[13];
        r[4] = read32(ram, r[14] + 4u);
        *pr = 0x8c10e52eu;
        status = sa_byte_move(ram, fpu, sr, r, pr, fpul, macl,
                              context, call);
        if (status != 0)
            return status;
        r[3] = read32(ram, r[14] + 4u);
        r[3] += r[13];
        write32(ram, r[14] + 4u, r[3]);
        r[2] = read32(ram, r[14] + 8u);
        r[2] += r[13];
        write32(ram, r[14] + 8u, r[2]);
    } else {
        r[12] = 0u;
        const uint32_t positive_length = (int32_t)r[13] > 0;
        set_t(sr, positive_length);
        if (positive_length) {
            for (;;) {
                r[5] = read32(ram, r[14] + 4u);
                r[0] = read32(ram, r[14] + 12u);
                r[4] = (uint32_t)read_s8(ram, r[11]);
                r[11] += 1u;
                *pr = 0x8c10e54au;
                status = call(context, r[0], ram, fpu, sr, r, pr);
                if (status != 0)
                    return status;
                const uint32_t failed = r[0] == 0xffffffffu;
                set_t(sr, failed);
                if (failed)
                    break;
                r[2] = read32(ram, r[14] + 8u);
                r[12] += 1u;
                const uint32_t complete = (int32_t)r[12] >= (int32_t)r[13];
                set_t(sr, complete);
                r[2] += 1u;
                write32(ram, r[14] + 8u, r[2]);
                if (complete)
                    break;
            }
        }
    }

    *pr = read32(ram, r[15]); r[15] += 4u;
    r[11] = read32(ram, r[15]); r[15] += 4u;
    r[12] = read32(ram, r[15]); r[15] += 4u;
    r[13] = read32(ram, r[15]); r[15] += 4u;
    r[14] = read32(ram, r[15]); r[15] += 4u;
    (void)saved_r14; (void)saved_r13; (void)saved_r12;
    (void)saved_r11; (void)saved_pr;
    return 0;
}
