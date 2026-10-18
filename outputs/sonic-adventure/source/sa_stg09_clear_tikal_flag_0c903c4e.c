#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_stg09_cpu;

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = (address & 0x1fffffffu) - 0x0c000000u;
    return (uint32_t)ram[offset] | ((uint32_t)ram[offset + 1] << 8) |
           ((uint32_t)ram[offset + 2] << 16) | ((uint32_t)ram[offset + 3] << 24);
}

static uint16_t read16(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = (address & 0x1fffffffu) - 0x0c000000u;
    return (uint16_t)((uint16_t)ram[offset] | ((uint16_t)ram[offset + 1] << 8));
}

static void write16(uint8_t *ram, uint32_t address, uint16_t value)
{
    uint32_t offset = (address & 0x1fffffffu) - 0x0c000000u;
    ram[offset] = (uint8_t)value;
    ram[offset + 1] = (uint8_t)(value >> 8);
}

__declspec(dllexport)
void sa_stg09_clear_tikal_flag_0c903c4e(uint8_t *ram, sa_stg09_cpu *cpu)
{
    uint32_t object = cpu->r[4];
    uint32_t global_slot = read32(ram, 0x0c903d00u);
    uint32_t global = read32(ram, global_slot);
    uint32_t state = read32(ram, global + 0x10u);
    cpu->r[0] = state;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)((state & 4u) == 0u);

    if ((state & 4u) != 0u) {
        uint32_t flags = (uint32_t)(int32_t)(int16_t)read16(ram, object + 4u);
        uint32_t updated_flags = flags & 0xfffffeffu;
        ram[(object + 0x13u & 0x1fffffffu) - 0x0c000000u] = 0u;
        cpu->r[0] = updated_flags;
        cpu->r[2] = 0u;
        cpu->r[3] = 0xfffffeffu;
        cpu->sr = (cpu->sr & ~1u) | (uint32_t)(updated_flags >= 0xfffffeffu);
        write16(ram, object + 4u, (uint16_t)updated_flags);
    }
    cpu->pc = cpu->pr;
}
