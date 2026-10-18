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

static void update_input_flag(uint8_t *ram, sa_cpu_state *cpu, uint8_t value)
{
    uint32_t index = cpu->r[4] & 0xffu;
    uint32_t first = read32(ram, 0x8c050a24u);
    cpu->r[0] = index;
    cpu->r[5] = first;
    cpu->r[6] = value;
    cpu->r[7] = index;

    if (index > 1u) {
        uint32_t second = read32(ram, 0x8c050a28u);
        cpu->sr &= ~1u;
        cpu->r[3] = second;
        ram[ram_offset(first)] = value;
        ram[ram_offset(second)] = value;
    } else {
        cpu->sr |= 1u;
        cpu->r[4] = index;
        ram[ram_offset(first + index)] = value;
    }
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_set_input_flag_by_index_8c0509b8(uint8_t *ram, sa_cpu_state *cpu)
{
    update_input_flag(ram, cpu, 1u);
}

__declspec(dllexport)
void sa_clear_input_flag_by_index_8c0509da(uint8_t *ram, sa_cpu_state *cpu)
{
    update_input_flag(ram, cpu, 0u);
}
