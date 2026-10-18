#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_stg09_cpu;

typedef struct {
    uint32_t f[16];
    uint32_t fpscr;
} sa_stg09_fpu;

typedef void (*sa_stg09_external_call)(uint8_t *ram, sa_stg09_cpu *cpu,
                                       sa_stg09_fpu *fpu, uint32_t target,
                                       void *context);

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
                          sa_stg09_fpu *fpu,
                          sa_stg09_external_call external_call,
                          void *context, uint32_t target, uint32_t return_pc)
{
    cpu->pr = return_pc;
    cpu->pc = target;
    if (external_call != 0) {
        external_call(ram, cpu, fpu, target, context);
    }
    cpu->pc = cpu->pr;
}

__declspec(dllexport)
void sa_stg09_callback_0c9005e4(uint8_t *ram, sa_stg09_cpu *cpu,
                                sa_stg09_fpu *fpu,
                                sa_stg09_external_call external_call,
                                void *context)
{
    uint32_t original_r14 = cpu->r[14];
    uint32_t stack = cpu->r[15] - 4u;
    uint32_t callback_state;
    uint32_t countdown;
    uint32_t dispatch;
    cpu->r[15] = stack;
    sa_stg09_write32(ram, stack, original_r14);
    stack -= 4u;
    cpu->r[15] = stack;
    sa_stg09_write32(ram, stack, cpu->pr);

    cpu->r[3] = sa_stg09_read32(ram, 0x0c900690u);
    cpu->r[14] = cpu->r[4];
    sa_stg09_call(ram, cpu, fpu, external_call, context,
                  cpu->r[3], 0x0c9005eeu);

    /* R4 is narrowed to a byte before its sign-extended word comparison. */
    callback_state = (uint8_t)cpu->r[0];
    cpu->r[4] = callback_state;
    cpu->r[5] = (uint32_t)(int32_t)(int16_t)(uint16_t)callback_state;

    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(cpu->r[5] == 0u);
    if (cpu->r[5] != 0u) {
        cpu->sr = (cpu->sr & ~1u) | (uint32_t)(cpu->r[5] == 1u);
    }
    if (cpu->r[5] != 0u && cpu->r[5] != 1u) {
        cpu->r[0] = cpu->r[5];
        callback_state = sa_stg09_read32(ram, cpu->r[14] + 0x20u);
        cpu->r[2] = callback_state;
        countdown = (uint32_t)(int32_t)(int8_t)ram[(cpu->r[2] & 0x1fffffffu) -
                                                    0x0c000000u + 3u];
        cpu->r[0] = countdown - 1u;
        ram[(cpu->r[2] & 0x1fffffffu) - 0x0c000000u + 3u] = (uint8_t)cpu->r[0];
        cpu->r[0] += 1u;
        cpu->r[0] = (uint8_t)cpu->r[0];
        cpu->sr = (cpu->sr & ~1u) | (uint32_t)(cpu->r[0] == 0u);

        if (cpu->r[0] == 0u) {
            dispatch = sa_stg09_read32(ram, 0x0c900698u);
            cpu->r[2] = dispatch;
            cpu->r[5] = 45u;
            cpu->r[4] = 0u;
            sa_stg09_call(ram, cpu, fpu, external_call, context,
                          dispatch, 0x0c90061eu);
            cpu->pr = sa_stg09_read32(ram, cpu->r[15]);
            cpu->r[15] += 4u;
            cpu->r[3] = sa_stg09_read32(ram, 0x0c900694u);
            cpu->r[4] = cpu->r[14];
            cpu->r[14] = sa_stg09_read32(ram, cpu->r[15]);
            cpu->r[15] += 4u;
            cpu->pc = cpu->r[3];
            return;
        }

        cpu->pr = sa_stg09_read32(ram, cpu->r[15]);
        cpu->r[15] += 4u;
        cpu->r[14] = sa_stg09_read32(ram, cpu->r[15]);
        cpu->r[15] += 4u;
        cpu->pc = cpu->pr;
        return;
    }

    cpu->pr = sa_stg09_read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->r[2] = sa_stg09_read32(ram, 0x0c900694u);
    cpu->r[4] = cpu->r[14];
    cpu->r[14] = sa_stg09_read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->pc = cpu->r[2];
}
