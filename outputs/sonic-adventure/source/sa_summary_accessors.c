#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu;

#define RAM_BASE 0x0c000000u

static uint32_t offset(uint32_t address)
{
    return (address & 0x1fffffffu) - RAM_BASE;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static uint16_t read16(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint16_t)((uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8));
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

__declspec(dllexport)
void sa_summary_set_byte_pointer_to_one_8c901640(uint8_t *ram, sa_cpu *cpu)
{
    uint32_t pointer = read32(ram, cpu->r[4] + 0x20u);
    cpu->r[3] = pointer;
    cpu->r[2] = 1u;
    ram[offset(pointer)] = 1u;
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_summary_get_nested_signed_halfword_8c901ba0(uint8_t *ram, sa_cpu *cpu)
{
    cpu->r[3] = read32(ram, cpu->r[4] + 0x20u);
    cpu->r[4] = read32(ram, cpu->r[3] + 0x10u);
    cpu->r[0] = read16(ram, cpu->r[4] + 2u);
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_summary_get_nested_word_plus8_8c901baa(uint8_t *ram, sa_cpu *cpu)
{
    cpu->r[4] = read32(ram, cpu->r[4] + 0x2cu);
    cpu->r[0] = read32(ram, cpu->r[4] + 8u);
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_summary_set_nested_word_plus12_8c901bb6(uint8_t *ram, sa_cpu *cpu)
{
    cpu->r[4] = read32(ram, cpu->r[4] + 0x2cu);
    write32(ram, cpu->r[4] + 0x0cu, cpu->r[5]);
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_summary_get_signed_byte_pointer_8c901bbc(uint8_t *ram, sa_cpu *cpu)
{
    uint32_t pointer = read32(ram, cpu->r[4] + 0x20u);
    cpu->r[0] = (uint32_t)(int32_t)(int8_t)ram[offset(pointer)];
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_summary_set_byte_pointer_8c901bc2(uint8_t *ram, sa_cpu *cpu)
{
    uint32_t pointer = read32(ram, cpu->r[4] + 0x20u);
    cpu->r[3] = pointer;
    ram[offset(pointer)] = (uint8_t)cpu->r[5];
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_summary_get_nested_word_plus4_8c901bc8(uint8_t *ram, sa_cpu *cpu)
{
    cpu->r[4] = read32(ram, cpu->r[4] + 0x2cu);
    cpu->r[0] = read32(ram, cpu->r[4] + 4u);
    cpu->pc = cpu->pr;
}
