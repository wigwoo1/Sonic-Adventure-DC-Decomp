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

static int16_t read16(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_offset(address);
    const uint16_t value = (uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8);
    return (int16_t)value;
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
void sa_input_record_request_store_8c646f68(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t pointer_cell = read32(ram, 0x8c647070u);
    cpu->r[2] = pointer_cell;
    cpu->r[7] = cpu->r[4];
    cpu->r[0] = (uint32_t)(int32_t)read16(ram, 0x8c64706au);
    cpu->r[7] <<= 1;
    cpu->r[3] = read32(ram, cpu->r[2]);
    cpu->r[1] = read32(ram, cpu->r[0] + cpu->r[3]);
    cpu->r[7] += cpu->r[1];
    cpu->r[1] = read32(ram, 0x8c647074u);
    cpu->r[3] = cpu->r[7];
    const uint32_t t = cpu->r[7] >> 31;
    cpu->r[7] <<= 1;
    cpu->r[7] += cpu->r[3];
    cpu->r[7] <<= 2;
    cpu->r[7] += cpu->r[1];
    cpu->sr = (cpu->sr & ~1u) | t;
    write32(ram, cpu->r[7] + 8u, cpu->r[5]);
    write32(ram, cpu->r[7] + 4u, cpu->r[6]);
    write32(ram, cpu->r[7], cpu->r[4]);
    cpu->r[0] = 0u;
    cpu->pc = cpu->pr;
}