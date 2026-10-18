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

static void write_byte(uint8_t *ram, uint32_t address, uint8_t value)
{
    ram[ram_offset(address)] = value;
}

static uint32_t ascii_upper(uint32_t value)
{
    int8_t character = (int8_t)(uint8_t)value;
    if (character > (int8_t)'`' && character < (int8_t)'{')
        value -= 0x20u;
    return value;
}

/* Retail 0x8c645328 returns the word stored at its data reference 0x8c645344. */
SA_STAGE_EXPORT uint32_t sa_string_table_pointer_get_8c645328(const uint8_t *ram)
{
    return read_word(ram, 0x8c645344u);
}

/* Retail 0x8c645348 uppercases only ASCII a-z, preserving other input bits. */
SA_STAGE_EXPORT uint32_t sa_ascii_upper_8c645348(uint32_t value)
{
    return ascii_upper(value);
}

/* Retail 0x8c64535c copies forward only while the signed length is positive. */
SA_STAGE_EXPORT void sa_forward_copy_positive_8c64535c(
    uint8_t *ram, uint32_t source, uint32_t destination, int32_t length)
{
    while (length > 0) {
        uint8_t value = read_byte(ram, source++);
        write_byte(ram, destination++, value);
        --length;
    }
}

/* Retail 0x8c645374: bounded ASCII-case-insensitive equality. */
SA_STAGE_EXPORT uint32_t sa_case_insensitive_bounded_equal_8c645374(
    const uint8_t *ram, uint32_t left, uint32_t right, int32_t length)
{
    for (;;) {
        if (length < 1)
            return 1;
        uint8_t a = read_byte(ram, left);
        if (a == 0u)
            return read_byte(ram, right) == 0u ? 1u : 0u;
        ++left;
        a = (uint8_t)ascii_upper(a);
        uint8_t b = (uint8_t)ascii_upper(read_byte(ram, right));
        ++right;
        if (a != b)
            return 0;
        --length;
    }
}

/* Retail 0x8c603bfc compares at most length bytes; nonpositive lengths match. */
SA_STAGE_EXPORT uint32_t sa_bounded_bytes_equal_8c603bfc(
    const uint8_t *ram, uint32_t left, uint32_t right, int32_t length)
{
    if (length < 1)
        return 1;
    for (int32_t index = 0; index < length; ++index) {
        if (read_byte(ram, left + (uint32_t)index) !=
            read_byte(ram, right + (uint32_t)index))
            return 0;
    }
    return 1;
}

/* Retail 0x8c6453cc compares signed-char strings through their terminating NUL. */
SA_STAGE_EXPORT uint32_t sa_strings_equal_8c6453cc(
    const uint8_t *ram, uint32_t left, uint32_t right)
{
    for (;;) {
        uint8_t a = read_byte(ram, left);
        uint8_t b = read_byte(ram, right);
        if (a == 0u)
            return b == 0u ? 1u : 0u;
        if (a != b)
            return 0;
        ++left;
        ++right;
    }
}
