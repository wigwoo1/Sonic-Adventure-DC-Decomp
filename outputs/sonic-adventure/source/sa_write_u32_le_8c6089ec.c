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

__declspec(dllexport)
void sa_write_u32_le_8c6089ec(uint8_t *ram, sa_cpu_state *cpu)
{
    uint32_t value = cpu->r[4];
    uint32_t destination = cpu->r[5];
    cpu->r[0] = value >> 8;
    ram[ram_offset(destination)] = (uint8_t)value;
    ram[ram_offset(destination + 1u)] = (uint8_t)cpu->r[0];
    cpu->r[0] = value;
    cpu->r[4] = value >> 16;
    cpu->r[0] >>= 16;
    cpu->r[4] >>= 8;
    destination = ram_offset(destination + 2u);
    ram[destination] = (uint8_t)cpu->r[0];
    ram[destination + 1u] = (uint8_t)(cpu->r[0] >> 8);
    cpu->r[0] = cpu->r[4];
    ram[ram_offset(cpu->r[5] + 3u)] = (uint8_t)cpu->r[0];
    cpu->pc = cpu->pr;
}
