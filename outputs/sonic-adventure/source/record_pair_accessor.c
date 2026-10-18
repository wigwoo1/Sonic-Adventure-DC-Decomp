#include "record_pair_accessor.h"

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static void write_word(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint8_t *p = ram + ram_offset(address);
    for (unsigned i = 0; i < 4; ++i)
        p[i] = (uint8_t)(value >> (i * 8));
}

static uint32_t read_word(const uint8_t *ram, uint32_t address)
{
    const uint8_t *p = ram + ram_offset(address);
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

/* Retail 0x8c65a5b4..0x8c65a5c8. */
SA_STAGE_EXPORT uint32_t sa_record_fields10_14_set_8c65a5b4(
    uint8_t *ram, uint32_t object, uint32_t first, uint32_t second)
{
    if (object == 0u)
        return read_word(ram, 0x8c65a5c8u);
    write_word(ram, object + 0x10u, first);
    write_word(ram, object + 0x14u, second);
    return 0u;
}
