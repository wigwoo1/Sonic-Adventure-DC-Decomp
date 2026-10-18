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
void sa_record_callback_mode_select_8c647de8(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t flag = cpu->r[4] & 1u;
    const uint32_t global_cell = read32(ram, 0x8c647e9cu);
    const uint32_t object = read32(ram, global_cell);
    const uint32_t mode_one = read32(ram, 0x8c647ea0u);
    const uint32_t mode_one_secondary = read32(ram, 0x8c647ea4u);
    const uint32_t mode_zero = read32(ram, 0x8c647ea8u);
    const uint32_t mode_zero_secondary = read32(ram, 0x8c647eacu);

    write32(ram, object, flag);
    if (flag != 0u) {
        write32(ram, object + 0x0cu, mode_one);
        write32(ram, object + 0x10u, mode_one_secondary);
        cpu->r[0] = flag;
        cpu->r[1] = object;
        cpu->r[2] = mode_one_secondary;
        cpu->r[4] = flag;
    } else {
        write32(ram, object + 0x0cu, mode_zero);
        write32(ram, object + 0x10u, mode_zero_secondary);
        cpu->r[0] = mode_zero;
        cpu->r[1] = mode_zero_secondary;
        cpu->r[2] = object;
        cpu->r[4] = 0u;
    }
    cpu->r[3] = 1u;
    cpu->r[5] = global_cell;
    cpu->r[6] = object;
    cpu->sr = (cpu->sr & ~1u) | flag;
    cpu->pc = cpu->pr;
}
