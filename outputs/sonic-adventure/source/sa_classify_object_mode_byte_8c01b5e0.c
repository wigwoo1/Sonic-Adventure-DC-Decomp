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

__declspec(dllexport)
void sa_classify_object_mode_byte_8c01b5e0(const uint8_t *ram, sa_cpu_state *cpu)
{
    uint32_t global_cell = read32(ram, 0x8c01b5fcu);
    uint32_t object = read32(ram, global_cell);
    int32_t value = (int8_t)ram[ram_offset(object + 9u)];

    cpu->r[4] = (uint32_t)value;
    if (value >= 5 && value <= 26) {
        cpu->r[1] = (uint32_t)(value - 5) * 2u;
        cpu->r[0] = (uint32_t)(((value & 1) != 0) || value == 26);
        cpu->sr &= ~1u;
    } else {
        cpu->r[1] = 22u;
        cpu->r[0] = 0u;
        cpu->sr |= 1u;
    }
    cpu->pc = cpu->pr;
}
