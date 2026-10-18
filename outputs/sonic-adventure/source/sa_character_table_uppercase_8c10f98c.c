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

__declspec(dllexport)
void sa_character_table_uppercase_8c10f98c(uint8_t *ram, sa_cpu_state *cpu)
{
    cpu->r[1] = read32(ram, 0x8c10f9a4u);
    cpu->r[0] = cpu->r[4] & 0xffu;
    cpu->r[0] = (uint32_t)(int32_t)(int8_t)ram[ram_offset(cpu->r[1] + cpu->r[0])];
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)((cpu->r[0] & 2u) == 0u);
    cpu->r[0] = cpu->r[4];
    if ((cpu->sr & 1u) == 0u)
        cpu->r[0] -= 0x20u;
    cpu->pc = cpu->pr;
}
