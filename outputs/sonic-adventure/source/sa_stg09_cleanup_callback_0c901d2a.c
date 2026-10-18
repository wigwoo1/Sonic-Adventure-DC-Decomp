#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_stg09_cpu;

typedef void (*sa_stg09_call)(uint8_t *ram, sa_stg09_cpu *cpu,
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

static void sa_stg09_call_service(uint8_t *ram, sa_stg09_cpu *cpu,
                                  sa_stg09_call call, void *context,
                                  uint32_t target, uint32_t return_pc)
{
    cpu->pr = return_pc;
    cpu->pc = target;
    if (call != 0) call(ram, cpu, target, context);
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_stg09_cleanup_callback_0c901d2a(uint8_t *ram, sa_stg09_cpu *cpu,
                                        sa_stg09_call call, void *context)
{
    uint32_t stack = cpu->r[15] - 4u;
    cpu->r[15] = stack;
    sa_stg09_write32(ram, stack, cpu->r[14]);
    stack -= 4u;
    cpu->r[15] = stack;
    sa_stg09_write32(ram, stack, cpu->pr);
    stack -= 4u;
    cpu->r[15] = stack;

    cpu->r[3] = sa_stg09_read32(ram, 0x0c901ee0u);
    cpu->r[14] = sa_stg09_read32(ram, cpu->r[4] + 0x20u);
    cpu->r[4] = 0x22u;
    sa_stg09_call_service(ram, cpu, call, context, cpu->r[3], 0x0c901d38u);

    cpu->r[2] = sa_stg09_read32(ram, 0x0c901ee4u);
    cpu->r[4] = 0u;
    sa_stg09_call_service(ram, cpu, call, context, cpu->r[2], 0x0c901d3eu);

    cpu->r[3] = sa_stg09_read32(ram, 0x0c901ee8u);
    cpu->r[5] = cpu->r[14] + 0x20u;
    cpu->r[4] = 0u;
    sa_stg09_call_service(ram, cpu, call, context, cpu->r[3], 0x0c901d48u);

    cpu->r[2] = sa_stg09_read32(ram, cpu->r[14] + 0x1cu);
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(cpu->r[2] == 0u);
    sa_stg09_write32(ram, cpu->r[15], cpu->r[2]);
    if (cpu->r[2] != 0u) {
        cpu->r[3] = sa_stg09_read32(ram, 0x0c901eecu);
        cpu->r[5] = sa_stg09_read32(ram, cpu->r[15]);
        cpu->r[4] = 0u;
        sa_stg09_call_service(ram, cpu, call, context, cpu->r[3], 0x0c901d58u);
    }

    cpu->r[3] = sa_stg09_read32(ram, cpu->r[14] + 0x14u);
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(cpu->r[3] == 0u);
    sa_stg09_write32(ram, cpu->r[15], cpu->r[3]);
    if (cpu->r[3] != 0u) {
        cpu->r[3] = sa_stg09_read32(ram, 0x0c901ef0u);
        cpu->r[5] = sa_stg09_read32(ram, cpu->r[15]);
        cpu->r[4] = 0u;
        sa_stg09_call_service(ram, cpu, call, context, cpu->r[3], 0x0c901d68u);
    }

    cpu->r[2] = sa_stg09_read32(ram, cpu->r[14] + 0x18u);
    cpu->r[3] = sa_stg09_read32(ram, cpu->r[14] + 0x0cu);
    cpu->r[2] += cpu->r[3];
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(cpu->r[2] == 0u);
    sa_stg09_write32(ram, cpu->r[15], cpu->r[2]);
    if (cpu->r[2] != 0u) {
        cpu->r[1] = sa_stg09_read32(ram, 0x0c901ef4u);
        cpu->r[5] = cpu->r[2];
        cpu->r[4] = 0u;
        sa_stg09_call_service(ram, cpu, call, context, cpu->r[1], 0x0c901d7cu);
    }

    cpu->r[2] = sa_stg09_read32(ram, 0x0c901ef8u);
    cpu->r[4] = sa_stg09_read32(ram, cpu->r[14] + 8u);
    sa_stg09_call_service(ram, cpu, call, context, cpu->r[2], 0x0c901d82u);
    cpu->r[4] = 1u;

    cpu->r[15] += 4u;
    cpu->pr = sa_stg09_read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->r[3] = sa_stg09_read32(ram, 0x0c901efcu);
    cpu->r[14] = sa_stg09_read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->pc = cpu->r[3];
    if (call != 0) call(ram, cpu, cpu->pc, context);
    cpu->pc = cpu->pr;
}
