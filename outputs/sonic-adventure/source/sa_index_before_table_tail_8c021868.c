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

static uint32_t read16(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8);
}

__declspec(dllexport)
void sa_index_before_table_tail_8c021868(const uint8_t *ram, sa_cpu_state *cpu)
{
    uint32_t table = read32(ram, cpu->r[6] + 4u);
    uint32_t limit = read16(ram, table + 2u) - 3u;
    int is_before_tail = (int32_t)cpu->r[4] < (int32_t)limit;
    uint32_t result = (is_before_tail && cpu->r[4] != 0u) ? 0u : 1u;

    cpu->r[5] = limit;
    cpu->r[0] = result;
    cpu->sr = (cpu->sr & ~1u) | result;
    cpu->pc = cpu->pr;
}
