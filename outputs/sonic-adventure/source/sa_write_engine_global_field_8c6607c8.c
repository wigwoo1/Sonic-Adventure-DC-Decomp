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

static uint32_t read16_signed(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    uint16_t value = (uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8);
    return (uint32_t)(int32_t)(int16_t)value;
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
void sa_write_engine_global_field_8c6607c8(uint8_t *ram, sa_cpu_state *cpu)
{
    cpu->r[2] = read32(ram, 0x8c6607e4u);
    cpu->r[0] = read16_signed(ram, 0x8c6607d6u);
    cpu->r[1] = read32(ram, 0x8c6607f0u);
    cpu->r[3] = read32(ram, cpu->r[2]);
    write32(ram, cpu->r[0] + cpu->r[3], cpu->r[1]);
    cpu->pc = cpu->pr;
}
