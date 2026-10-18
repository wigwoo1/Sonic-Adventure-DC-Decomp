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

static void sa_stg09_write16(uint8_t *ram, uint32_t address, uint16_t value)
{
    uint32_t offset = (address & 0x1fffffffu) - 0x0c000000u;
    ram[offset] = (uint8_t)value;
    ram[offset + 1] = (uint8_t)(value >> 8);
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
void sa_stg09_task_callback_0c901192(uint8_t *ram, sa_stg09_cpu *cpu,
                                     sa_stg09_call call, void *context)
{
    uint32_t stack = cpu->r[15] - 4u;
    cpu->r[15] = stack;
    uint32_t saved_argument = stack - 4u;
    sa_stg09_write32(ram, stack, cpu->pr);
    cpu->r[15] = saved_argument;
    ram[(saved_argument & 0x1fffffffu) - 0x0c000000u] = (uint8_t)cpu->r[4];

    cpu->r[3] = sa_stg09_read32(ram, 0x0c9011f0u);
    cpu->r[5] = 3u;
    cpu->r[6] = sa_stg09_read32(ram, 0x0c9011ecu);
    cpu->r[4] = 2u;
    cpu->pr = 0x0c9011a2u;
    cpu->pc = cpu->r[3];
    if (call != 0) call(ram, cpu, cpu->r[3], context);
    cpu->pc = cpu->pr;

    uint32_t task = cpu->r[0];
    uint32_t work = sa_stg09_read32(ram, task + 0x20u);
    cpu->r[4] = work;
    cpu->r[0] = 0x1234u;
    sa_stg09_write16(ram, work + 6u, 0x1234u);
    cpu->r[0] = (uint32_t)(int32_t)(int8_t)ram[(cpu->r[15] & 0x1fffffffu) -
                                                     0x0c000000u];
    ram[(cpu->r[4] & 0x1fffffffu) - 0x0c000000u + 3u] = (uint8_t)cpu->r[0];
    cpu->r[15] += 4u;
    cpu->pr = sa_stg09_read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->pc = cpu->pr;
}
