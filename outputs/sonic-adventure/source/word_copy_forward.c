#include "word_copy_forward.h"

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read_word(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = ram_offset(address);
    return (uint32_t)ram[offset] | ((uint32_t)ram[offset + 1u] << 8) |
           ((uint32_t)ram[offset + 2u] << 16) | ((uint32_t)ram[offset + 3u] << 24);
}

static void write_word(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t offset = ram_offset(address);
    for (unsigned i = 0; i < 4; ++i)
        ram[offset + i] = (uint8_t)(value >> (i * 8));
}

/* Retail 0x8c6044e0..0x8c6044ee; count is in 32-bit words and the loop runs once. */
SA_STAGE_EXPORT void sa_word_copy_forward_8c6044e0(
    uint8_t *ram, uint32_t destination, uint32_t source, uint32_t count)
{
    do {
        uint32_t value = read_word(ram, source);
        write_word(ram, destination, value);
        destination += 4u;
        source += 4u;
        --count;
    } while (count != 0u);
}
