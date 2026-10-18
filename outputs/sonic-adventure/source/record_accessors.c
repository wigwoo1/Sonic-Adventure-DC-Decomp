#include "record_accessors.h"

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

/* Retail 0x8c65086c..0x8c650870. */
SA_STAGE_EXPORT void sa_record_field1c_set_8c65086c(
    uint8_t *ram, uint32_t object, uint32_t value)
{
    write_word(ram, object + 0x1cu, value);
}

/* Retail 0x8c65a5ac..0x8c65a5b2. */
SA_STAGE_EXPORT uint32_t sa_record_field8_set_8c65a5ac(
    uint8_t *ram, uint32_t object, uint32_t value)
{
    write_word(ram, object + 8u, value);
    return 0u;
}

/* Retail 0x8c65a5cc..0x8c65a5d2. */
SA_STAGE_EXPORT uint32_t sa_record_field4_set_8c65a5cc(
    uint8_t *ram, uint32_t object, uint32_t value)
{
    write_word(ram, object + 4u, value);
    return 0u;
}

/* Retail 0x8c65a5d4..0x8c65a5da. */
SA_STAGE_EXPORT uint32_t sa_record_fieldc_set_8c65a5d4(
    uint8_t *ram, uint32_t object, uint32_t value)
{
    write_word(ram, object + 0xcu, value);
    return 0u;
}

/* Retail 0x8c65bfbc..0x8c65bfc2. */
SA_STAGE_EXPORT void sa_record_fields38_3c_set_8c65bfbc(
    uint8_t *ram, uint32_t object, uint32_t first, uint32_t second)
{
    write_word(ram, object + 0x38u, first);
    write_word(ram, object + 0x3cu, second);
}
