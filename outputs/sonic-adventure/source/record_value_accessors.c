#include "record_value_accessors.h"

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

/* Retail 0x8c65c1c6..0x8c65c1ca: sign-extend object byte +1. */
SA_STAGE_EXPORT int32_t sa_record_signed_byte_get_8c65c1c6(
    const uint8_t *ram, uint32_t object)
{
    return (int8_t)ram[ram_offset(object + 1u)];
}

/* Retail 0x8c667702..0x8c667708: sign-extend object halfword +0x24. */
SA_STAGE_EXPORT int32_t sa_record_signed_halfword_get_8c667702(
    const uint8_t *ram, uint32_t object)
{
    uint32_t offset = ram_offset(object + 0x24u);
    uint16_t value = (uint16_t)ram[offset] | ((uint16_t)ram[offset + 1u] << 8);
    return (int16_t)value;
}

/* Retail 0x8c65bce4..0x8c65bcea: advance a record pointer by 0x50 bytes. */
SA_STAGE_EXPORT uint32_t sa_record_next_record_8c65bce4(uint32_t address)
{
    return address + 0x50u;
}
