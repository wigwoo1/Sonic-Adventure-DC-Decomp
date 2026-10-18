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
    const uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_record_word_at_index_stride_0x80_store_offset_8_8c647cb4(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t index = cpu->r[4];
    const uint32_t table = read32(ram, 0x8c647d10u);
    cpu->r[3] = table;
    cpu->r[4] = table + (index << 7);
    cpu->sr = (cpu->sr & ~1u) | ((index >> 25) & 1u);
    write32(ram, cpu->r[4] + 8u, cpu->r[5]);
    cpu->pc = cpu->pr;
}
