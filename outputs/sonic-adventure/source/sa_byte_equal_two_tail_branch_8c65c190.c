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

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_byte_equal_two_tail_branch_8c65c190(const uint8_t *ram, sa_cpu_state *cpu)
{
    const int32_t value = (int32_t)(int8_t)ram[offset(cpu->r[4] + 1u)];
    cpu->r[0] = (uint32_t)value;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(value == 2);
    cpu->pc = (cpu->sr & 1u) ? 0x8c65c026u : cpu->pr;
}
