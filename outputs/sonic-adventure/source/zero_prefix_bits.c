#include "zero_prefix_bits.h"

static uint8_t read_byte(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = (address & 0x1fffffffu) - 0x0c000000u;
    return ram[offset];
}

/* Retail 0x8c603c4c..0x8c603c84; param2 is multiplied by 64 bytes. */
SA_STAGE_EXPORT uint32_t sa_count_zero_prefix_bits_8c603c4c(
    const uint8_t *ram, uint32_t data, int32_t block_count)
{
    int32_t remaining = (int32_t)((uint32_t)block_count << 6);
    uint32_t zero_bits = 0;
    for (;;) {
        if (remaining < 1)
            return zero_bits;
        uint8_t value = read_byte(ram, data++);
        int32_t bits = 8;
        do {
            if ((value & 0x80u) != 0u)
                return zero_bits;
            --bits;
            value = (uint8_t)(value << 1);
            ++zero_bits;
        } while (bits > 0);
        --remaining;
    }
}
