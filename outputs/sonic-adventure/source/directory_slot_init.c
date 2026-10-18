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

static uint8_t read_byte(const uint8_t *ram, uint32_t address)
{
    return ram[ram_offset(address)];
}

/* Retail 0x8c645898 clears a two-word descriptor, selects an eight-byte global
   slot, and copies the source descriptor's eight identifying bytes into it. */
SA_STAGE_EXPORT void sa_directory_slot_initialize_8c645898(
    uint8_t *ram, int32_t slot_index, uint32_t output, uint32_t object, int32_t value)
{
    uint32_t source = read_word(ram, object + 8u);
    uint32_t slot_table = read_word(ram, 0x8c6458f4u);
    uint32_t slot = slot_table + (uint32_t)slot_index * 8u;

    write_word(ram, output + 4u, 0u);
    write_word(ram, output, 0u);
    write_word(ram, output + 36u, (uint32_t)value + 0x14u);
    write_word(ram, output + 40u, slot);

    ram[ram_offset(slot)] = read_byte(ram, source + 8u);
    ram[ram_offset(slot + 1u)] = read_byte(ram, source + 9u);
    for (uint32_t index = 0; index < 6u; ++index)
        ram[ram_offset(slot + 2u + index)] = read_byte(ram, source + 10u + index);
}
