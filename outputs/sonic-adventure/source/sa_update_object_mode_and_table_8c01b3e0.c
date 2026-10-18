#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

static uint32_t ram_offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

__declspec(dllexport)
void sa_update_object_mode_and_table_8c01b3e0(uint8_t *ram, sa_cpu_state *cpu)
{
    uint32_t first_literal = read32(ram, 0x8c01b4e0u);
    uint32_t table_global = read32(ram, 0x8c01b4e4u);
    uint32_t first_object = read32(ram, first_literal);
    uint32_t mode = ram[ram_offset(first_object + 9u)];
    int32_t signed_mode = mode < 0x80u ? (int32_t)mode : (int32_t)mode - 0x100;
    uint32_t table_base = read32(ram, table_global);
    uint32_t second_literal;
    uint32_t second_object;
    uint32_t table_entry = table_base + (uint32_t)(signed_mode * 8) + 4u;
    uint32_t table_value;

    ram[ram_offset(first_object + 9u)] = (uint8_t)cpu->r[4];
    mode = ram[ram_offset(first_object + 9u)];
    signed_mode = mode < 0x80u ? (int32_t)mode : (int32_t)mode - 0x100;
    table_entry = table_base + (uint32_t)(signed_mode * 8) + 4u;
    table_value = read32(ram, table_entry);
    write32(ram, first_object + 0x10u, table_value);

    second_literal = read32(ram, 0x8c01b4e8u);
    second_object = read32(ram, second_literal);
    write32(ram, second_object + 0x14u, 0u);

    cpu->r[0] = second_literal;
    cpu->r[1] = table_global;
    cpu->r[2] = 0u;
    cpu->r[3] = second_object;
    cpu->r[5] = first_literal;
    cpu->r[2] = 0u;
    if ((uint8_t)cpu->r[4] >= 0x80u)
        cpu->sr |= 1u;
    else
        cpu->sr &= ~1u;
    cpu->pc = cpu->pr;
}
