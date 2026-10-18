#include "object_accessors.h"

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

/* Retail 0x8c644500..0x8c644504: signed byte at object offset 1. */
SA_STAGE_EXPORT int32_t sa_object_byte_at_1_get_8c644500(
    const uint8_t *ram, uint32_t object)
{
    return (int8_t)ram[ram_offset(object + 1u)];
}

/* Retail 0x8c644602..0x8c644606: store a word at object offset 0x38. */
SA_STAGE_EXPORT void sa_object_field38_set_8c644602(
    uint8_t *ram, uint32_t object, uint32_t value)
{
    uint8_t *p = ram + ram_offset(object + 0x38u);
    for (unsigned i = 0; i < 4; ++i)
        p[i] = (uint8_t)(value >> (i * 8));
}
