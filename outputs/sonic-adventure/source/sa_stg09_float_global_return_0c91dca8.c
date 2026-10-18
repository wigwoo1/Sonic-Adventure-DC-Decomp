#include <stdint.h>

#define CODE_BASE 0x0c000000u
#define GLOBAL_POINTER_LITERAL 0x0c91dd10u

typedef struct {
    uint32_t banks[2][16];
    uint32_t fpscr;
    uint32_t fpul;
} sa_fpu;

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu;

static uint32_t offset(uint32_t address)
{
    return (address & 0x1fffffffu) - CODE_BASE;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

__declspec(dllexport)
void sa_stg09_float_global_return_0c91dca8(const uint8_t *ram,
                                           sa_cpu *cpu,
                                           sa_fpu *fpu)
{
    uint32_t pointer = read32(ram, GLOBAL_POINTER_LITERAL);
    uint32_t active = (fpu->fpscr >> 21) & 1u;
    cpu->r[3] = pointer;

    fpu->banks[active][0] = read32(ram, pointer);
    if (fpu->fpscr & 0x00100000u) {
        fpu->banks[active][1] = read32(ram, pointer + 4u);
    }

    cpu->pc = cpu->pr;
}
