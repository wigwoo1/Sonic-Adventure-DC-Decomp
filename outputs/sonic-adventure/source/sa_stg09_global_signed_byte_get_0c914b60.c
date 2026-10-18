#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu;

#define RAM_BASE 0x0c000000u
#define GLOBAL_POINTER_SLOT 0x0c914c40u

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

__declspec(dllexport)
void sa_stg09_global_signed_byte_get_0c914b60(uint8_t *ram, sa_cpu *cpu)
{
    uint32_t slot = read32(ram, GLOBAL_POINTER_SLOT);
    uint32_t value_pointer = read32(ram, slot);
    cpu->r[3] = slot;
    cpu->r[1] = value_pointer;
    if (value_pointer == 0u) {
        cpu->r[0] = 0u;
        cpu->sr = (cpu->sr & ~1u) | 1u;
    } else {
        cpu->r[0] = (uint32_t)(int32_t)(int8_t)ram[offset(value_pointer)];
        cpu->sr &= ~1u;
    }
    cpu->pc = cpu->pr;
}
