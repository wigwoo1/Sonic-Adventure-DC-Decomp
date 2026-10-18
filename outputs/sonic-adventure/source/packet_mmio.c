#include "memory_positive_loops.h"

typedef uint32_t (*sa_mmio_read32_fn)(void *context, uint32_t address);

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static void write_word(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t offset = ram_offset(address);
    ram[offset] = (uint8_t)value;
    ram[offset + 1u] = (uint8_t)(value >> 8);
    ram[offset + 2u] = (uint8_t)(value >> 16);
    ram[offset + 3u] = (uint8_t)(value >> 24);
}

/* Retail 0x8c604594: wait for a masked MMIO value to clear and then set. */
SA_STAGE_EXPORT void sa_mmio_transition_wait_8c604594(
    sa_mmio_read32_fn read32, void *context)
{
    const uint32_t address = 0xa05f810cu;
    const uint32_t mask = 0xff0f01ffu;
    do {
    } while ((read32(context, address) & mask) != 0u);
    do {
    } while ((read32(context, address) & mask) == 0u);
}

/* Retail 0x8c6045a8: write one while the controller's busy bit is clear. */
SA_STAGE_EXPORT void sa_mmio_idle_flag_store_8c6045a8(
    uint8_t *ram, uint32_t destination,
    sa_mmio_read32_fn read32, void *context)
{
    uint32_t value = read32(context, 0xa05f688cu);
    write_word(ram, destination, (value & 0x20u) == 0u ? 1u : 0u);
}
