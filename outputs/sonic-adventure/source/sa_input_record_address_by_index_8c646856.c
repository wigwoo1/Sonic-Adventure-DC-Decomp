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
    const uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_input_record_address_by_index_8c646856(uint8_t *ram, sa_cpu_state *cpu)
{
    cpu->r[0] = cpu->r[4];
    cpu->r[0] <<= 1;
    cpu->r[3] = cpu->r[4];
    cpu->r[0] += cpu->r[3];
    cpu->r[2] = read32(ram, 0x8c6468b0u);
    cpu->r[0] <<= 2;
    const uint32_t t = cpu->r[0] >> 31;
    cpu->r[0] <<= 1;
    cpu->sr = (cpu->sr & ~1u) | t;
    cpu->r[0] += cpu->r[2];
    cpu->pc = cpu->pr;
}