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

__declspec(dllexport)
void sa_object_callback_tail_dispatch_8c0663ca(uint8_t *ram, sa_cpu_state *cpu)
{
    uint32_t stack = cpu->r[15] - 4u;
    uint32_t target;

    cpu->r[15] = stack;
    cpu->r[3] = read32(ram, cpu->r[4] + 0x14u);
    cpu->r[2] = cpu->r[3];
    write32(ram, stack, cpu->r[3]);
    target = cpu->r[2];
    cpu->r[15] += 4u;
    cpu->pc = target;
}
