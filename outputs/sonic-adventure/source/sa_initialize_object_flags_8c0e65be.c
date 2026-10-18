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

static void write16(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
}

__declspec(dllexport)
void sa_initialize_object_flags_8c0e65be(uint8_t *ram, sa_cpu_state *cpu)
{
    cpu->r[3] = 1u;
    cpu->r[0] = 0x49u;
    ram[ram_offset(cpu->r[4])] = (uint8_t)cpu->r[3];
    cpu->r[0] <<= 2;
    cpu->r[4] = 0u;
    write16(ram, cpu->r[6] + cpu->r[0], cpu->r[4]);
    cpu->r[0] = cpu->r[4];
    write16(ram, cpu->r[6] + 0x0eu, cpu->r[0]);
    cpu->pc = cpu->pr;
}
