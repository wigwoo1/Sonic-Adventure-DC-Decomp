#include "memory_positive_loops.h"

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

/* Retail 0x8c603c84..0x8c603cd2 pads bitstream blocks with zero bits,
   then a one-bit suffix. The final partial byte retains the block-count seed. */
SA_STAGE_EXPORT uint32_t sa_bitstream_pad_8c603c84(
    uint8_t *ram, int32_t valid_bits, uint32_t destination, uint32_t block_count)
{
    int32_t remaining = (int32_t)(block_count << 6);
    uint32_t seed = block_count;

    while (remaining > 0) {
        uint8_t value;
        if (valid_bits < 8) {
            if (valid_bits < 1) {
                value = 0xffu;
            } else {
                for (int bit = 0; bit < 8; ++bit) {
                    seed <<= 1;
                    if (valid_bits < 1)
                        seed |= 1u;
                    else
                        --valid_bits;
                }
                value = (uint8_t)seed;
            }
        } else {
            value = 0;
            valid_bits -= 8;
        }
        ram[ram_offset(destination)] = value;
        ++destination;
        --remaining;
    }
    return 0;
}
