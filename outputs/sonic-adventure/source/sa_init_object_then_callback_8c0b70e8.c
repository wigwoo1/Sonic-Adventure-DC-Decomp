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

static void write8(uint8_t *ram, uint32_t address, uint8_t value)
{
    ram[offset(address)] = value;
}

static void write16(uint8_t *ram, uint32_t address, uint16_t value)
{
    const uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_init_object_then_callback_8c0b70e8(uint8_t *ram, sa_cpu_state *cpu)
{
    cpu->r[3] = 1u;
    write8(ram, cpu->r[4], (uint8_t)cpu->r[3]);
    cpu->r[0] = 0u;
    cpu->r[3] = read32(ram, 0x8c0b71f4u);
    write16(ram, cpu->r[5] + 14u, (uint16_t)cpu->r[0]);
    cpu->pc = cpu->r[3];
}
