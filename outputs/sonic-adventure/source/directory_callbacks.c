#include "memory_positive_loops.h"

typedef void (*sa_sh4_callback_dispatch)(uint8_t *ram, uint32_t target, uint32_t argument);

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

/* Retail 0x8c645afa dispatches each linked callback in the event's 16-slot group.
   SH-4 code pointers are resolved by the native port's dispatcher. */
SA_STAGE_EXPORT void sa_directory_callback_dispatch_8c645afa(
    uint8_t *ram, int16_t event, sa_sh4_callback_dispatch dispatch)
{
    int32_t group = (int32_t)event >> 4;
    uint32_t table = read_word(ram, 0x8c645b60u);
    uint32_t node = read_word(ram, table + (uint32_t)(group * 4));
    while (node != 0u) {
        uint32_t target = read_word(ram, node);
        uint32_t argument = read_word(ram, node + 8u);
        dispatch(ram, target, argument);
        node = read_word(ram, node + 4u);
    }
}
