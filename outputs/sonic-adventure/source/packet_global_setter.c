#include "memory_positive_loops.h"

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
    ram[offset] = (uint8_t)value;
    ram[offset + 1u] = (uint8_t)(value >> 8);
    ram[offset + 2u] = (uint8_t)(value >> 16);
    ram[offset + 3u] = (uint8_t)(value >> 24);
}

/* Retail 0x8c604510 stores the input at the pointer held in its literal pool. */
SA_STAGE_EXPORT void sa_packet_global_value_set_8c604510(uint8_t *ram, uint32_t value)
{
    uint32_t destination = read_word(ram, 0x8c60461cu);
    write_word(ram, destination, value);
}

typedef void (*sa_native_callback_dispatch)(uint8_t *ram, uint32_t sh4_target);

/* Retail 0x8c604546 resets two globals and tail-dispatches an optional callback.
   The callback resolver is explicit here because SH-4 addresses are not host pointers. */
SA_STAGE_EXPORT void sa_packet_callback_reset_8c604546(
    uint8_t *ram, sa_native_callback_dispatch dispatch)
{
    write_word(ram, 0x8c88f7d0u, 0u);
    write_word(ram, 0x8c88f7d8u, 1u);
    uint32_t callback = read_word(ram, 0x8c673478u);
    if (callback != 0u && dispatch != 0)
        dispatch(ram, callback);
}
