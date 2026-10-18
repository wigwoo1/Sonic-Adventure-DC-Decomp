#include "memory_positive_loops.h"

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

/* Retail 0x8c603c2e..0x8c603c3e: forward copy while signed length is positive. */
SA_STAGE_EXPORT void sa_byte_copy_positive_8c603c2e(
    uint8_t *ram, uint32_t destination, uint32_t source, int32_t length)
{
    while (length > 0) {
        ram[ram_offset(destination)] = ram[ram_offset(source)];
        ++destination;
        ++source;
        --length;
    }
}

/* Retail 0x8c603c38..0x8c603c4c: write 0xff while signed length is positive. */
SA_STAGE_EXPORT void sa_byte_fill_ff_positive_8c603c38(
    uint8_t *ram, uint32_t destination, int32_t length)
{
    while (length > 0) {
        ram[ram_offset(destination)] = 0xffu;
        ++destination;
        --length;
    }
}
