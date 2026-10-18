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

static void copy_bytes(uint8_t *ram, uint32_t source, uint32_t destination, uint32_t length)
{
    for (uint32_t index = 0; index < length; ++index)
        ram[ram_offset(destination + index)] = ram[ram_offset(source + index)];
}

/* Retail 0x8c6461ae compacts 24 records of stride 0x2c to stride 0x18,
   retaining source byte ranges [0,4), [8,12), [16,24). */
SA_STAGE_EXPORT void sa_object_table_compact_8c6461ae(uint8_t *ram)
{
    uint32_t destination = read_word(ram, 0x8c6462b0u);
    uint32_t source = read_word(ram, 0x8c8a83b4u) + 0x28u;
    for (uint32_t row = 0; row < 0x18u; ++row) {
        uint32_t src = source + row * 0x2cu;
        uint32_t dst = destination + row * 0x18u;
        copy_bytes(ram, src, dst, 4u);
        copy_bytes(ram, src + 8u, dst + 4u, 4u);
        copy_bytes(ram, src + 0x10u, dst + 8u, 8u);
    }
}
