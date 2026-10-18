#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_stg09_cpu;

typedef void (*sa_stg09_external_call)(uint8_t *ram, sa_stg09_cpu *cpu,
                                       uint32_t target, void *context);

static uint32_t sa_stg09_read32(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = (address & 0x1fffffffu) - 0x0c000000u;
    return (uint32_t)ram[offset] |
           ((uint32_t)ram[offset + 1] << 8) |
           ((uint32_t)ram[offset + 2] << 16) |
           ((uint32_t)ram[offset + 3] << 24);
}

static void sa_stg09_write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t offset = (address & 0x1fffffffu) - 0x0c000000u;
    ram[offset] = (uint8_t)value;
    ram[offset + 1] = (uint8_t)(value >> 8);
    ram[offset + 2] = (uint8_t)(value >> 16);
    ram[offset + 3] = (uint8_t)(value >> 24);
}

static void sa_stg09_call(uint8_t *ram, sa_stg09_cpu *cpu,
                          sa_stg09_external_call external_call,
                          void *context, uint32_t target, uint32_t return_pc)
{
    cpu->pr = return_pc;
    cpu->pc = target;
    if (external_call != 0) {
        external_call(ram, cpu, target, context);
    }
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_stg09_callback_0c9005ba(uint8_t *ram, sa_stg09_cpu *cpu,
                                sa_stg09_external_call external_call,
                                void *context)
{
    uint32_t original_pr = cpu->pr;
    uint32_t stack = cpu->r[15] - 4u;
    cpu->r[15] = stack;
    sa_stg09_write32(ram, stack, original_pr);

    cpu->r[2] = sa_stg09_read32(ram, 0x0c900680u);
    sa_stg09_call(ram, cpu, external_call, context, cpu->r[2], 0x0c9005c2u);

    cpu->r[4] = 0x32u;
    cpu->r[3] = sa_stg09_read32(ram, 0x0c900684u);
    cpu->r[4] <<= 8;
    cpu->r[4] += (uint32_t)-0x39;
    sa_stg09_call(ram, cpu, external_call, context, cpu->r[3], 0x0c9005ceu);

    cpu->r[2] = sa_stg09_read32(ram, 0x0c900688u);
    cpu->r[5] = 0;
    cpu->r[4] = cpu->r[5];
    sa_stg09_call(ram, cpu, external_call, context, cpu->r[2], 0x0c9005d6u);

    cpu->r[4] = 0;
    sa_stg09_call(ram, cpu, external_call, context, 0x0c900040u, 0x0c9005dau);
    cpu->r[4] = 1;
    sa_stg09_call(ram, cpu, external_call, context, 0x0c900040u, 0x0c9005deu);

    cpu->r[3] = sa_stg09_read32(ram, 0x0c90068cu);
    cpu->pr = sa_stg09_read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->pc = cpu->r[3];
}
