#include "memory_positive_loops.h"

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint8_t read_byte(const uint8_t *ram, uint32_t address)
{
    return ram[ram_offset(address)];
}

static uint32_t ascii_upper(uint32_t value)
{
    int8_t character = (int8_t)(uint8_t)value;
    if (character > (int8_t)'`' && character < (int8_t)'{')
        value -= 0x20u;
    return value;
}

static void copy_forward(uint8_t *ram, uint32_t source, uint32_t destination, int32_t length)
{
    while (length > 0) {
        uint8_t value = read_byte(ram, source++);
        ram[ram_offset(destination++)] = value;
        --length;
    }
}

/* Retail 0x8c64541c returns the first left-string index containing a right-set byte,
   or the left string's length if there is no match. */
SA_STAGE_EXPORT int32_t sa_string_set_find_8c64541c(
    const uint8_t *ram, uint32_t left, uint32_t set)
{
    int32_t index = 0;
    for (;;) {
        uint8_t value = read_byte(ram, left);
        if (value == 0u)
            return index;
        uint32_t scan = set;
        while (read_byte(ram, scan) != 0u) {
            if (value == read_byte(ram, scan))
                return index;
            ++scan;
        }
        ++index;
        ++left;
    }
}

/* Retail 0x8c645442: seeded, case-insensitive byte sum reduced to eight bits. */
SA_STAGE_EXPORT uint32_t sa_ascii_name_hash_8c645442(
    const uint8_t *ram, uint32_t name)
{
    uint32_t sum = read_byte(ram, 0x8c64554eu);
    for (;;) {
        uint8_t value = read_byte(ram, name++);
        if (value == 0u)
            break;
        sum += ascii_upper(value) & 0xffu;
    }
    return (sum >> 2) & 0xffu;
}

/* Retail 0x8c645468 parses a 0x22-byte-or-larger directory record into a compact
   10-byte metadata prefix plus a NUL-terminated name and its case-insensitive hash. */
SA_STAGE_EXPORT uint8_t sa_directory_entry_parse_8c645468(
    uint8_t *ram, uint32_t record, uint32_t output)
{
    uint8_t record_length = read_byte(ram, record);
    if (record_length < 0x22u)
        return 0;

    copy_forward(ram, record + 2u, output, 4);
    copy_forward(ram, record + 10u, output + 4u, 4);
    uint8_t flags = read_byte(ram, record + 0x19u);
    ram[ram_offset(output + 8u)] = flags;

    uint32_t name_length = read_byte(ram, record + 0x20u);
    if ((flags & 2u) == 0u)
        name_length -= 2u;
    copy_forward(ram, record + 0x21u, output + 10u, (int32_t)name_length);
    ram[ram_offset(output + 10u + name_length)] = 0;

    if ((flags & 2u) != 0u) {
        uint32_t first_name_byte = output + 10u;
        if (read_byte(ram, first_name_byte) == 0u) {
            ram[ram_offset(first_name_byte)] = '.';
        } else if (read_byte(ram, first_name_byte) == 1u) {
            ram[ram_offset(first_name_byte)] = '.';
            ram[ram_offset(first_name_byte + 1u)] = '.';
            ram[ram_offset(first_name_byte + 2u)] = 0;
        }
    }

    ram[ram_offset(output + 9u)] = (uint8_t)sa_ascii_name_hash_8c645442(ram, output + 10u);
    return record_length;
}
