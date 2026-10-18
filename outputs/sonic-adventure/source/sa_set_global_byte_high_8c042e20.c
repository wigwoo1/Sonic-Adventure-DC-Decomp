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
void sa_set_global_byte_high_8c042e20(uint8_t *ram, sa_cpu_state *cpu)
{
    uint32_t input_byte = cpu->r[4] & 0xffu;
    uint32_t stack_frame = cpu->r[15] - 4u;
    uint32_t destination = read32(ram, 0x8c042f60u);
    uint32_t value = input_byte << 24;

    write32(ram, stack_frame, value);
    write32(ram, destination, value);
    cpu->r[0] = 0u;
    cpu->r[2] = destination;
    cpu->r[3] = value;
    cpu->r[4] = stack_frame;
    cpu->pc = cpu->pr;
}
