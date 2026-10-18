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

__declspec(dllexport)
void sa_engine_service_tail_8c644d84(const uint8_t *ram, sa_cpu_state *cpu)
{
    cpu->r[3] = read32(ram, 0x8c644d94u);
    cpu->r[4] = read32(ram, 0x8c644d90u);
    cpu->r[5] = read16_signed(ram, 0x8c644d8eu);
    cpu->pc = cpu->r[3];
}
