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
void sa_masked_global_pair_set_8c608c14(uint8_t *ram, sa_cpu_state *cpu)
{
    cpu->r[3] = read32(ram, 0x8c608ce4u);
    cpu->r[1] = read32(ram, 0x8c608ce8u);
    cpu->r[5] &= 0xfffff800u;
    cpu->sr &= ~1u;
    write32(ram, cpu->r[3], cpu->r[4]);
    write32(ram, cpu->r[1], cpu->r[5]);
    cpu->pc = cpu->pr;
}
