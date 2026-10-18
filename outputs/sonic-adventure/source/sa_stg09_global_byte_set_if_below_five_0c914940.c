#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu;

#define RAM_BASE 0x0c000000u
#define POINTER_LITERAL 0x0c914a40u

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
void sa_stg09_global_byte_set_if_below_five_0c914940(uint8_t *ram, sa_cpu *cpu)
{
    uint32_t slot = read32(ram, POINTER_LITERAL);
    uint32_t object;
    uint32_t saved_stack;
    int32_t argument = (int32_t)cpu->r[4];

    cpu->r[3] = slot;
    cpu->r[15] -= 4u;
    saved_stack = cpu->r[15];
    write32(ram, saved_stack, cpu->r[14]);
    object = read32(ram, slot);
    cpu->r[14] = object;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(object == 0u);

    if (object != 0u) {
        cpu->r[1] = 4u;
        cpu->sr = (cpu->sr & ~1u) | (uint32_t)(argument > 4);
        if (argument <= 4) {
            cpu->r[0] = cpu->r[4];
            ram[offset(object + 1u)] = (uint8_t)cpu->r[0];
        }
    }

    cpu->r[14] = read32(ram, saved_stack);
    cpu->r[15] += 4u;
    cpu->pc = cpu->pr;
}
