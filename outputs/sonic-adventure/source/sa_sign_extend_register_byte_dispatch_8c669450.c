#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

static uint32_t offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

__declspec(dllexport)
void sa_sign_extend_register_byte_dispatch_8c669450(uint8_t *ram, sa_cpu_state *cpu)
{
    uint32_t stack = cpu->r[15] - 4u;
    uint32_t target = read32(ram, 0x8c66945cu);
    uint8_t value = (uint8_t)cpu->r[6];

    cpu->r[15] = stack;
    ram[offset(stack)] = value;
    cpu->r[5] = (uint32_t)(int32_t)(int8_t)value;
    cpu->r[15] += 4u;
    cpu->r[3] = target;
    cpu->pc = target;
}
