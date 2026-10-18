#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu;

#define RAM_BASE 0x0c000000u
#define FLAG_ADDRESS_LITERAL 0x0c9135c0u
#define DESTINATION 0x0caf3490u

static uint32_t offset(uint32_t address)
{
    return (address & 0x1fffffffu) - RAM_BASE;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
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
void sa_stg09_gated_global_word_set_0c91344e(uint8_t *ram, sa_cpu *cpu)
{
    uint32_t flag_address = read32(ram, FLAG_ADDRESS_LITERAL);
    uint32_t flag = read32(ram, flag_address);
    cpu->r[2] = flag_address;
    cpu->r[3] = flag;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(flag == 0u);
    if (flag != 0u) {
        cpu->r[3] = DESTINATION;
        write32(ram, DESTINATION, cpu->r[4]);
        cpu->r[0] = 1u;
    } else {
        cpu->r[0] = 0u;
    }
    cpu->pc = cpu->pr;
}
