#include "memory_positive_loops.h"

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint8_t read_byte(const uint8_t *ram, uint32_t address)
{
    return ram[ram_offset(address)];
}

static uint32_t read_word(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = ram_offset(address);
    return (uint32_t)ram[offset] | ((uint32_t)ram[offset + 1u] << 8) |
           ((uint32_t)ram[offset + 2u] << 16) | ((uint32_t)ram[offset + 3u] << 24);
}

static uint32_t ascii_upper(uint32_t value)
{
    int8_t character = (int8_t)(uint8_t)value;
    if (character > (int8_t)'`' && character < (int8_t)'{')
        value -= 0x20u;
    return value;
}

static uint32_t name_hash(const uint8_t *ram, uint32_t name)
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

static int equal_names(const uint8_t *ram, uint32_t left, uint32_t right, int case_sensitive)
{
    for (;;) {
        uint8_t a = read_byte(ram, left++);
        uint8_t b = read_byte(ram, right++);
        if (!case_sensitive) {
            a = (uint8_t)ascii_upper(a);
            b = (uint8_t)ascii_upper(b);
        }
        if (a != b)
            return 0;
        if (a == 0u)
            return 1;
    }
}

/* Retail 0x8c6455ce searches a 44-byte-per-entry table, using the cached name
   hash before exact or ASCII-case-insensitive string comparison. Returns 1-based index. */
SA_STAGE_EXPORT int32_t sa_directory_name_find_8c6455ce(
    const uint8_t *ram, uint32_t name, uint32_t table, int32_t case_sensitive)
{
    int32_t count = (int32_t)read_word(ram, table);
    uint32_t record = table + 16u;
    uint32_t hash = name_hash(ram, name);
    for (int32_t index = 0; index < count; ++index, record += 44u) {
        if (read_byte(ram, record + 9u) == (uint8_t)hash &&
            equal_names(ram, name, record + 10u, case_sensitive == 1))
            return index + 1;
    }
    return 0;
}
