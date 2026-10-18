#include <stdint.h>

typedef struct { uint32_t r[16], sr, pr, pc; } sa_cpu;

#define RAM_BASE 0x0c000000u
static uint32_t offset(uint32_t a) { return (a & 0x1fffffffu) - RAM_BASE; }

static void clear_if_unflagged(uint8_t *ram, sa_cpu *cpu,
                               uint32_t flag_address, uint32_t clear_address,
                               uint32_t return_pc)
{
    int32_t value=(int8_t)ram[offset(flag_address)];
    cpu->r[0]=(uint32_t)value;
    cpu->sr=(cpu->sr&~1u)|(uint32_t)(((uint32_t)value&3u)==0u);
    if (cpu->sr&1u) {
        cpu->r[0]=0;
        ram[offset(clear_address)]=0;
    }
    cpu->pc=return_pc;
}

__declspec(dllexport)
void sa_stg09_flagged_byte_clear_0c903a60(uint8_t *ram, sa_cpu *cpu)
{
    clear_if_unflagged(ram,cpu,cpu->r[4],cpu->r[4]+8u,cpu->pr);
}

__declspec(dllexport)
void sa_stg09_flagged_byte_clear_0c903a6e(uint8_t *ram, sa_cpu *cpu)
{
    clear_if_unflagged(ram,cpu,cpu->r[4]+1u,cpu->r[4]+9u,cpu->pr);
}
