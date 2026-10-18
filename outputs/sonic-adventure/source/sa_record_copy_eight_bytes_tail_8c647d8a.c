#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_record_copy_eight_bytes_tail_8c647d8a(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t index = cpu->r[4];
    cpu->r[15] -= 4u;
    const uint32_t table = read32(ram, 0x8c647da4u);
    const uint32_t target = read32(ram, 0x8c647da8u);
    const uint32_t record = table + (index << 7);
    write32(ram, cpu->r[15], record);
    cpu->r[2] = target;
    cpu->r[3] = table;
    cpu->r[4] = record + 0x6cu;
    cpu->r[6] = 8u;
    cpu->sr = (cpu->sr & ~1u) | ((index >> 25) & 1u);
    cpu->r[15] += 4u;
    cpu->pc = target;
}
