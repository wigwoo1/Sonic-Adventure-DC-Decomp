#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_stg09_cpu;

extern void sa_stg09_clear_flagged_byte_0c903a60(uint8_t *ram,
                                                  sa_stg09_cpu *cpu);
extern void sa_stg09_clear_shifted_flagged_byte_0c903a6e(uint8_t *ram,
                                                         sa_stg09_cpu *cpu);

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = (address & 0x1fffffffu) - 0x0c000000u;
    return (uint32_t)ram[offset] | ((uint32_t)ram[offset + 1] << 8) |
           ((uint32_t)ram[offset + 2] << 16) | ((uint32_t)ram[offset + 3] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t offset = (address & 0x1fffffffu) - 0x0c000000u;
    ram[offset] = (uint8_t)value;
    ram[offset + 1] = (uint8_t)(value >> 8);
    ram[offset + 2] = (uint8_t)(value >> 16);
    ram[offset + 3] = (uint8_t)(value >> 24);
}

static uint16_t read16(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = (address & 0x1fffffffu) - 0x0c000000u;
    return (uint16_t)((uint16_t)ram[offset] | ((uint16_t)ram[offset + 1] << 8));
}

static void write16(uint8_t *ram, uint32_t address, uint16_t value)
{
    uint32_t offset = (address & 0x1fffffffu) - 0x0c000000u;
    ram[offset] = (uint8_t)value;
    ram[offset + 1] = (uint8_t)(value >> 8);
}

__declspec(dllexport)
void sa_stg09_tikal_status_and_byte_cleanup_0c903c0c(uint8_t *ram,
                                                     sa_stg09_cpu *cpu)
{
    uint32_t object = cpu->r[4];
    uint32_t original_pr = cpu->pr;
    uint32_t original_r14 = cpu->r[14];
    uint32_t sp = cpu->r[15];
    uint32_t global_slot = read32(ram, 0x0c903d00u);
    uint32_t global = read32(ram, global_slot);
    uint32_t state = read32(ram, global + 0x10u);

    sp -= 4u;
    cpu->r[15] = sp;
    write32(ram, sp, original_r14);
    sp -= 4u;
    cpu->r[15] = sp;
    write32(ram, sp, original_pr);

    cpu->r[0] = state;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)((state & 4u) == 0u);
    if ((state & 4u) != 0u) {
        cpu->r[3] = 0xfffffbffu;
        write16(ram, object + 4u,
                (uint16_t)(read16(ram, object + 4u) & 0xfbffu));
    }

    cpu->r[14] = object;
    cpu->pr = 0x0c903c2cu;
    cpu->pc = 0x0c903a60u;
    sa_stg09_clear_flagged_byte_0c903a60(ram, cpu);

    cpu->pr = read32(ram, sp);
    sp += 4u;
    cpu->r[15] = sp;
    cpu->r[4] = cpu->r[14];
    cpu->r[14] = read32(ram, sp);
    sp += 4u;
    cpu->r[15] = sp;
    cpu->pr = original_pr;
    sa_stg09_clear_shifted_flagged_byte_0c903a6e(ram, cpu);
}
