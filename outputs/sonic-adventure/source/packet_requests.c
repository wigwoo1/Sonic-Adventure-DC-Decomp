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

/* Both request entry points share the retail active-context global at 0x8c673450. */
static uint32_t active_context(const uint8_t *ram)
{
    return read_word(ram, 0x8c673450u);
}

/* Retail 0x8c6041d4: schedule a normal packet read. */
SA_STAGE_EXPORT uint32_t sa_packet_read_request_8c6041d4(
    uint8_t *ram, int32_t packet_index, uint32_t callback)
{
    uint32_t context = active_context(ram);
    if (context == 0)
        return 0xfffffff5u;
    if (read_word(ram, context + 0x1cu) != 0)
        return 0xfffffff7u;

    if (packet_index < 0 || packet_index >= (int32_t)read_word(ram, context + 0x14u))
        return 0xfffffffbu;

    write_word(ram, context + 0x1cu, 1u);
    write_word(ram, context + 0x20u, 2u);
    write_word(ram, context + 0x28u, (uint32_t)packet_index);
    write_word(ram, context + 0x30u, callback);
    return 0;
}

/* Retail 0x8c604210: schedule a packet write and clear its retry field. */
SA_STAGE_EXPORT uint32_t sa_packet_write_request_8c604210(
    uint8_t *ram, int32_t packet_index, uint32_t callback)
{
    uint32_t context = active_context(ram);
    if (context == 0)
        return 0xfffffff5u;
    if (read_word(ram, context + 0x1cu) != 0)
        return 0xfffffff7u;

    if (packet_index < 0 || packet_index >= (int32_t)read_word(ram, context + 0x14u))
        return 0xfffffffbu;

    write_word(ram, context + 0x1cu, 2u);
    write_word(ram, context + 0x20u, 2u);
    write_word(ram, context + 0x28u, (uint32_t)packet_index);
    write_word(ram, context + 0x30u, callback);
    write_word(ram, context + 0x24u, 0u);
    return 0;
}

/* Retail 0x8c6042e4: clear the active context pointer. */
SA_STAGE_EXPORT void sa_packet_context_clear_8c6042e4(uint8_t *ram)
{
    write_word(ram, 0x8c673450u, 0u);
}
