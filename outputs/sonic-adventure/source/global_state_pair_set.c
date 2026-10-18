#include "global_state_pair_set.h"

static uint32_t read_word(const uint8_t *ram, uint32_t address)
{
    const uint8_t *p = ram + ((address & 0x1fffffffu) - 0x0c000000u);
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static void write_word(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint8_t *p = ram + ((address & 0x1fffffffu) - 0x0c000000u);
    for (unsigned i = 0; i < 4; ++i)
        p[i] = (uint8_t)(value >> (i * 8));
}

/* Retail 0x8c65b468..0x8c65b472; both destinations come from retail pointer literals. */
SA_STAGE_EXPORT void sa_global_state_pair_set_8c65b468(
    uint8_t *ram, uint32_t first, uint32_t second)
{
    uint32_t first_address = read_word(ram, 0x8c65b4bcu);
    uint32_t second_address = read_word(ram, 0x8c65b4c0u);
    write_word(ram, first_address, first);
    write_word(ram, second_address, second);
}
