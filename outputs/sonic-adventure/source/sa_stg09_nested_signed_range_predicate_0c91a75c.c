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

__declspec(dllexport)
void sa_stg09_nested_signed_range_predicate_0c91a75c(const uint8_t *ram, sa_cpu *cpu)
{
    uint32_t root = cpu->r[5];
    uint32_t nested = read32(ram, root + 0x2cu);
    uint32_t limit = read32(ram, nested + 0x1cu) - 2u;
    int32_t value = (int32_t)cpu->r[4];
    int32_t signed_limit = (int32_t)limit;
    int below_limit = value < signed_limit;
    int positive = value > 0;

    cpu->r[2] = root;
    cpu->r[3] = nested;
    cpu->r[5] = limit;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(below_limit ? positive : 1);
    cpu->r[0] = (below_limit && positive) ? 0u : 1u;
    cpu->pc = cpu->pr;
}
