#include "record_pair_result.h"

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

/* Retail 0x8c65c200..0x8c65c208. */
SA_STAGE_EXPORT uint32_t sa_record_fields18_1c_set_8c65c200(
    uint8_t *ram, uint32_t object, uint32_t field1c, uint32_t field18)
{
    write_word(ram, object + 0x1cu, field1c);
    write_word(ram, object + 0x18u, field18);
    return 1u;
}
