#include "byte_fill_backward.h"

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

/* Retail 0x8c0869a4..0x8c0869b0; fill backward and test after each byte. */
SA_STAGE_EXPORT void sa_byte_fill_backward_8c0869a4(
    uint8_t *ram, uint32_t destination, uint32_t value, uint32_t length)
{
    uint32_t cursor = destination + length;
    do {
        --length;
        --cursor;
        ram[ram_offset(cursor)] = (uint8_t)value;
    } while (length != 0u);
}
