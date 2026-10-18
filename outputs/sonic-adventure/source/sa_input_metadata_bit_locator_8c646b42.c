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
void sa_input_metadata_bit_locator_8c646b42(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t initial_sp = cpu->r[15];
    const uint32_t stack = initial_sp - 4u;
    const uint32_t saved_r12 = cpu->r[12];
    cpu->r[7] = cpu->r[4];
    cpu->r[15] = stack;
    write32(ram, stack, saved_r12);
    cpu->r[6] = read32(ram, 0x8c646c3cu);
    cpu->r[0] = 0x20u;
    cpu->r[12] = read32(ram, cpu->r[4]);
    cpu->r[4] += 4u;
    cpu->r[7] += 4u;
    cpu->r[1] = 0u;

    for (uint32_t i = 0u; i < 32u; ++i) {
        cpu->r[3] = cpu->r[6];
        if ((cpu->r[3] & cpu->r[12]) != 0u) {
            if (cpu->r[5] == cpu->r[6]) {
                cpu->r[0] = cpu->r[7];
                cpu->r[12] = read32(ram, stack);
                cpu->r[15] = initial_sp;
                cpu->sr |= 1u;
                cpu->pc = cpu->pr;
                return;
            }
            cpu->r[4] += 4u;
            cpu->r[7] = cpu->r[4];
        }
        cpu->r[1] += 1u;
        cpu->r[6] >>= 1;
        cpu->sr = (cpu->sr & ~1u) | (cpu->r[1] == cpu->r[0]);
    }

    cpu->r[0] = 0u;
    cpu->r[12] = read32(ram, stack);
    cpu->r[15] = initial_sp;
    cpu->pc = cpu->pr;
}