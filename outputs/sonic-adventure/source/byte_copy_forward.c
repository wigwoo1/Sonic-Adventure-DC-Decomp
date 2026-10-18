#include "byte_copy_forward.h"

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

/* Retail 0x8c604500..0x8c60450e; copy forward once before testing the count. */
SA_STAGE_EXPORT void sa_byte_copy_forward_8c604500(
    uint8_t *ram, uint32_t destination, uint32_t source, uint32_t length)
{
    do {
        ram[ram_offset(destination)] = ram[ram_offset(source)];
        ++destination;
        ++source;
        --length;
    } while (length != 0u);
}
