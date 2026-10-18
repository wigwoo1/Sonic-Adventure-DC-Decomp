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

__declspec(dllexport)
void sa_stg09_callback_setup_0c901c98(uint8_t *ram, sa_stg09_cpu *cpu,
                                      sa_stg09_call call, void *context)
{
    uint32_t original_r14 = cpu->r[14];
    uint32_t stack = cpu->r[15] - 4u;
    cpu->r[15] = stack;
    sa_stg09_write32(ram, stack, original_r14);
    stack -= 4u;
    cpu->r[15] = stack;
    sa_stg09_write32(ram, stack, cpu->pr);
    cpu->r[3] = 1u;
    stack -= 4u;
    cpu->r[15] = stack;
    sa_stg09_write32(ram, stack, cpu->r[3]);

    cpu->r[5] = sa_stg09_read32(ram, 0x0c901d04u);
    cpu->r[6] = 4u;
    cpu->r[2] = sa_stg09_read32(ram, 0x0c901d08u);
    cpu->r[14] = cpu->r[4];
    cpu->r[7] = cpu->r[3];
    cpu->r[4] = cpu->r[14];
    cpu->pr = 0x0c901caeu;
    cpu->pc = cpu->r[2];
    if (call != 0) call(ram, cpu, cpu->r[2], context);
    cpu->pc = cpu->pr;

    cpu->r[3] = sa_stg09_read32(ram, cpu->r[14] + 0x20u);
    cpu->r[0] = 60u;
    cpu->r[2] = sa_stg09_read32(ram, 0x0c901d10u);
    ram[(cpu->r[3] & 0x1fffffffu) - 0x0c000000u + 3u] = (uint8_t)cpu->r[0];
    cpu->r[3] = sa_stg09_read32(ram, 0x0c901d0cu);
    sa_stg09_write32(ram, cpu->r[14] + 0x10u, cpu->r[3]);
    sa_stg09_write32(ram, cpu->r[14] + 0x18u, cpu->r[2]);

    cpu->r[15] += 4u;
    cpu->pr = sa_stg09_read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->r[14] = sa_stg09_read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->pc = cpu->pr;
}
