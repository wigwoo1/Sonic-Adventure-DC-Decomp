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
void sa_record_mode_value_store_8c647f10(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t input = cpu->r[4];
    const uint32_t global_cell = read32(ram, 0x8c647fc8u);
    const uint32_t object = read32(ram, global_cell);
    uint32_t mode;
    if (input == 0x0fu) mode = 0u;
    else if (input == 0x21u) mode = 1u;
    else if (input == 0x41u) mode = 2u;
    else if (input == 0x7au) mode = 3u;
    else {
        cpu->r[0] = 0xffffff00u;
        cpu->r[3] = global_cell;
        cpu->r[5] = object;
        cpu->sr &= ~1u;
        cpu->pc = cpu->pr;
        return;
    }

    write32(ram, object + 4u, mode);
    cpu->r[0] = 0u;
    if (input == 0x0fu) cpu->r[2] = 0u;
    if (input == 0x41u) cpu->r[1] = 2u;
    cpu->r[3] = global_cell;
    cpu->r[5] = object;
    cpu->sr |= 1u;
    cpu->pc = cpu->pr;
}
