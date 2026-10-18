#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_stg09_cpu;

void sa_stg09_tikal_compare_worker_0c903a7c(uint8_t *ram, sa_stg09_cpu *cpu)
{
    (void)ram;
    cpu->pc = cpu->pr;
}
