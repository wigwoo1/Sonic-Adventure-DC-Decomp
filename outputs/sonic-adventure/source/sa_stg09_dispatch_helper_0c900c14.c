#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_stg09_cpu;

typedef void (*sa_stg09_tail_call)(uint8_t *ram, sa_stg09_cpu *cpu,
                                   uint32_t target, void *context);

static uint32_t sa_stg09_read32(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = (address & 0x1fffffffu) - 0x0c000000u;
    return (uint32_t)ram[offset] |
           ((uint32_t)ram[offset + 1] << 8) |
           ((uint32_t)ram[offset + 2] << 16) |
           ((uint32_t)ram[offset + 3] << 24);
}

__declspec(dllexport)
void sa_stg09_dispatch_helper_0c900c14(uint8_t *ram, sa_stg09_cpu *cpu,
                                       sa_stg09_tail_call tail_call,
                                       void *context)
{
    uint32_t target;
    cpu->r[0] = cpu->r[4];
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(cpu->r[0] == 12u);
    if (cpu->sr & 1u) {
        cpu->r[2] = sa_stg09_read32(ram, 0x0c900c50u);
        cpu->r[4] = sa_stg09_read32(ram, 0x0c900c4cu);
        target = cpu->r[2];
        cpu->pc = target;
        if (tail_call != 0) tail_call(ram, cpu, target, context);
        cpu->pc = cpu->pr;
        return;
    }

    cpu->sr &= ~1u;
    cpu->pc = 0x0c900c20u;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(cpu->r[0] == 0x22u);
    if (cpu->sr & 1u) {
        cpu->r[3] = sa_stg09_read32(ram, 0x0c900c50u);
        cpu->r[4] = sa_stg09_read32(ram, 0x0c900c54u);
        target = cpu->r[3];
        cpu->pc = target;
        if (tail_call != 0) tail_call(ram, cpu, target, context);
        cpu->pc = cpu->pr;
        return;
    }

    cpu->sr &= ~1u;
    cpu->pc = 0x0c900c2eu;
}
