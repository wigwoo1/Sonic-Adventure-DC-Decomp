#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

static uint32_t offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static void dispatch(uint8_t *ram, sa_cpu_state *cpu, uint32_t delay_destination)
{
    uint32_t stack = cpu->r[15] - 4u;
    uint32_t table_base;
    uint32_t target;
    uint32_t delay_address;
    uint32_t delay_value;

    write32(ram, stack, cpu->r[3]);
    cpu->r[15] = stack;
    table_base = read32(ram, 0x8c10cd50u);
    cpu->r[3] = table_base;
    cpu->r[3] = read32(ram, cpu->r[0] + cpu->r[3]);
    cpu->r[0] -= 4u;
    target = cpu->r[3];
    delay_address = cpu->r[0] + cpu->r[2];
    delay_value = read32(ram, delay_address);
    if (delay_destination == 0u)
        cpu->r[0] = delay_value;
    else
        cpu->r[3] = delay_value;
    cpu->pc = target;
}

__declspec(dllexport)
void sa_indirect_indexed_dispatch_8c10cd38(uint8_t *ram, sa_cpu_state *cpu)
{ dispatch(ram, cpu, 0u); }

__declspec(dllexport)
void sa_indirect_indexed_dispatch_8c10cd44(uint8_t *ram, sa_cpu_state *cpu)
{ dispatch(ram, cpu, 3u); }
