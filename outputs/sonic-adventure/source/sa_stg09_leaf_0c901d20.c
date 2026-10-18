#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_stg09_cpu;

/* Retail subtracts then restores 44 bytes in its RTS delay slot. */
__declspec(dllexport)
void sa_stg09_leaf_0c901d20(sa_stg09_cpu *cpu)
{
    cpu->pc = cpu->pr;
}
