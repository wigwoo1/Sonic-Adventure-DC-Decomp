#include "memory_positive_loops.h"

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint8_t read_byte(const uint8_t *ram, uint32_t address)
{
    return ram[ram_offset(address)];
}

static uint16_t read_halfword(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = ram_offset(address);
    return (uint16_t)((uint16_t)ram[offset] | ((uint16_t)ram[offset + 1u] << 8));
}

static uint32_t read_word(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = ram_offset(address);
    return (uint32_t)ram[offset] | ((uint32_t)ram[offset + 1u] << 8) |
           ((uint32_t)ram[offset + 2u] << 16) | ((uint32_t)ram[offset + 3u] << 24);
}

/* Retail 0x8c64bda0: CRC-16 table routine used by the packet validator. */
SA_STAGE_EXPORT uint32_t sa_crc16_packet_8c64bda0(
    const uint8_t *ram, int32_t length, uint32_t data)
{
    uint32_t count = (uint32_t)length;
    uint32_t crc = read_halfword(ram, 0x8c64bdd4u);
    do {
        --count;
        if ((int32_t)count < 0)
            break;
        uint8_t value = read_byte(ram, data++);
        uint32_t table_index = ((crc & 0xffffu) >> 8) ^ value;
        int32_t table_entry = (int16_t)read_halfword(
            ram, 0x8c66d3bcu + table_index * 2u);
        crc = ((crc & 0xffffu) << 8) ^ (uint32_t)table_entry;
    } while (1);
    return (~(crc & 0xffffu)) & 0xffffu;
}

/* Retail 0x8c603cd2: validate an FF-padded 64-byte packet, then consume it. */
SA_STAGE_EXPORT uint32_t sa_packet_validate_8c603cd2(
    uint8_t *ram, uint32_t context, uint32_t packet)
{
    (void)context;
    uint32_t index = 0;
    while (index < 60u) {
        if (read_byte(ram, packet + 2u + index) != 0xffu)
            return 0;
        ++index;
    }

    uint32_t checksum = sa_crc16_packet_8c64bda0(ram, 0x3e, packet);
    if (read_halfword(ram, packet + 0x3eu) != (uint16_t)checksum)
        return 0;

    for (index = 0; index < 0x40u; ++index)
        ram[ram_offset(packet + index)] = 0xffu;
    return 1;
}

/* Retail 0x8c603f8c: copy the pending 60-byte payload into its packet slot,
   then accept the 0xffff sentinel or a matching packet CRC. */
SA_STAGE_EXPORT int32_t sa_packet_buffer_check_8c603f8c(
    uint8_t *ram, uint32_t context)
{
    if ((int16_t)read_halfword(ram, context + 2u) != 0)
        return -6;

    uint32_t packet = read_word(ram, context + 0x0cu) +
                      ((read_word(ram, context + 0x28u) + 1u) << 6);
    uint32_t source = read_word(ram, context + 0x30u);
    for (uint32_t index = 0; index < 0x3cu; ++index)
        ram[ram_offset(packet + 2u + index)] = read_byte(ram, source + index);

    if (read_halfword(ram, packet) == 0xffffu ||
        read_halfword(ram, packet + 0x3eu) ==
            (uint16_t)sa_crc16_packet_8c64bda0(ram, 0x3e, packet))
        return 0;
    return -4;
}
