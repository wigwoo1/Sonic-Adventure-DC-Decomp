#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

__declspec(dllexport)
void sa_bcd_nibble_to_int_8c64b3b4(uint8_t *ram, sa_cpu_state *cpu)
{
    (void)ram;
    cpu->r[5] = cpu->r[4] >> 4;
    cpu->r[0] = cpu->r[5];
    cpu->r[3] = cpu->r[5];
    cpu->r[0] <<= 2;
    cpu->r[2] = 15u;
    cpu->r[0] += cpu->r[3];
    cpu->sr = (cpu->sr & ~1u) | (cpu->r[0] >> 31);
    cpu->r[0] <<= 1;
    cpu->r[4] &= cpu->r[2];
    cpu->r[0] += cpu->r[4];
    cpu->pc = cpu->pr;
}
