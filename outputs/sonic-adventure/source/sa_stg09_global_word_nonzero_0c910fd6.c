#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu;

#define RAM_BASE 0x0c000000u
#define POINTER_LITERAL 0x0c911018u

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

__declspec(dllexport)
void sa_stg09_global_word_nonzero_0c910fd6(uint8_t *ram, sa_cpu *cpu)
{
    uint32_t global = read32(ram, POINTER_LITERAL);
    uint32_t value = read32(ram, global);
    cpu->r[2] = global;
    cpu->r[3] = value;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(value == 0u);
    cpu->r[0] = (value == 0u) ? 0u : 1u;
    cpu->pc = cpu->pr;
}
