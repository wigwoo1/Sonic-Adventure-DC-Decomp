#include "fpu_cleanup_transfer.h"

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = address - 0x8c000000u;
    return (uint32_t)ram[offset] | ((uint32_t)ram[offset + 1u] << 8) |
           ((uint32_t)ram[offset + 2u] << 16) |
           ((uint32_t)ram[offset + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t offset = address - 0x8c000000u;
    ram[offset] = (uint8_t)value;
    ram[offset + 1u] = (uint8_t)(value >> 8);
    ram[offset + 2u] = (uint8_t)(value >> 16);
    ram[offset + 3u] = (uint8_t)(value >> 24);
}

static void div1_step(uint32_t divisor, uint32_t *remainder, uint32_t *sr)
{
    uint32_t q = *remainder >> 31;
    uint32_t m = (*sr >> 9) & 1u;
    uint32_t old_q = (*sr >> 8) & 1u;
    uint32_t before = (*remainder << 1) | (*sr & 1u);
    uint32_t result;
    uint32_t carry;

    if (old_q == m) {
        result = before - divisor;
        carry = result > before;
    } else {
        result = before + divisor;
        carry = result < before;
    }
    q = m ? (q ? carry : !carry) : (q ? !carry : carry);
    *remainder = result;
    *sr = (*sr & ~0x101u) | (q << 8) | (uint32_t)(q == m);
}

SA_STAGE_EXPORT int sa_unsigned_remainder_8c10cc80(
        uint8_t *ram, struct sa_fpu_banks *fpu, uint32_t *sr,
        uint32_t *r, uint32_t *pr, uint32_t *fpul, uint32_t *macl,
        void *context,
        int (*call)(void *, uint32_t, uint8_t *, struct sa_fpu_banks *,
                    uint32_t *, uint32_t *, uint32_t *))
{
    (void)fpu;
    (void)pr;
    (void)fpul;
    (void)macl;
    (void)context;
    (void)call;

    *sr = (*sr & ~1u) | (uint32_t)(r[0] == 0u);
    if (*sr & 1u) {
        r[15] -= 4u;
        write32(ram, r[15], r[2]);
        r[1] = read32(ram, 0x8c10cd30u);
        r[2] = read32(ram, 0x8c10cd34u);
        r[0] = 0u;
        write32(ram, r[1], r[2]);
        r[2] = read32(ram, r[15]);
        r[15] += 4u;
        return 0;
    }

    r[15] -= 4u;
    write32(ram, r[15], r[3]);
    r[3] = 0u;
    r[15] -= 4u;
    write32(ram, r[15], r[4]);
    r[4] = r[0];
    *sr &= ~0x301u; /* DIV0U */

    for (unsigned bit = 0; bit < 32u; ++bit) {
        uint32_t incoming = *sr & 1u;
        *sr = (*sr & ~1u) | (r[1] >> 31);
        r[1] = (r[1] << 1) | incoming; /* ROTCL r1 */
        div1_step(r[4], &r[3], sr);
    }

    if (*sr & 1u)
        r[0] = r[3];
    else
        r[0] += r[3];

    r[4] = read32(ram, r[15]);
    r[15] += 4u;
    r[3] = read32(ram, r[15]);
    r[15] += 4u;
    return 0;
}
