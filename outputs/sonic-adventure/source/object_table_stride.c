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

/* Retail 0x8c646856 returns table_base + index * 0x18, modulo 32 bits. */
SA_STAGE_EXPORT uint32_t sa_object_table_stride_8c646856(const uint8_t *ram, uint32_t index)
{
    return read_word(ram, 0x8c6468b0u) + index * 0x18u;
}
