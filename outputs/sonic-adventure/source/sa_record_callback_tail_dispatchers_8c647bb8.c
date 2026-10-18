#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

static uint32_t ram_offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static void set_t(sa_cpu_state *cpu, uint32_t value)
{
    cpu->sr = (cpu->sr & ~1u) | (value & 1u);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_record_callback_arg_tail_dispatch_8c647bb8(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t input_index = cpu->r[4];
    const uint32_t saved_r14 = cpu->r[14];
    const uint32_t stack = cpu->r[15] - 4u;
    cpu->r[15] = stack;
    write32(ram, stack, saved_r14);
    cpu->r[14] = input_index;
    cpu->r[14] <<= 2;
    cpu->r[14] <<= 2;
    cpu->r[14] <<= 2;
    cpu->r[14] <<= 1;
    cpu->r[3] = read32(ram, 0x8c647c4cu);
    cpu->r[14] += cpu->r[3];
    cpu->r[0] = 0x74u;
    uint32_t target = read32(ram, cpu->r[14] + cpu->r[0]);
    cpu->r[2] = target;
    set_t(cpu, target == 0u);
    if (target != 0u) {
        target = read32(ram, cpu->r[14] + cpu->r[0]);
        cpu->r[2] = target;
        cpu->r[0] = 0x7cu;
        cpu->r[7] = read32(ram, cpu->r[14] + cpu->r[0]);
        cpu->r[15] += 4u;
        cpu->r[14] = saved_r14;
        cpu->pc = target;
        return;
    }
    cpu->r[15] += 4u;
    cpu->r[14] = saved_r14;
    cpu->pc = cpu->pr;
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_record_callback_tail_dispatch_8c647bde(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t input_index = cpu->r[4];
    const uint32_t saved_r14 = cpu->r[14];
    const uint32_t stack = cpu->r[15] - 4u;
    cpu->r[15] = stack;
    write32(ram, stack, saved_r14);
    cpu->r[14] = input_index;
    cpu->r[14] <<= 2;
    cpu->r[14] <<= 2;
    cpu->r[14] <<= 2;
    cpu->r[14] <<= 1;
    cpu->r[3] = read32(ram, 0x8c647c4cu);
    cpu->r[14] += cpu->r[3];
    cpu->r[0] = 0x78u;
    uint32_t target = read32(ram, cpu->r[14] + cpu->r[0]);
    cpu->r[2] = target;
    set_t(cpu, target == 0u);
    if (target != 0u) {
        target = read32(ram, cpu->r[14] + cpu->r[0]);
        cpu->r[2] = target;
        cpu->r[15] += 4u;
        cpu->r[14] = saved_r14;
        cpu->pc = target;
        return;
    }
    cpu->r[15] += 4u;
    cpu->r[14] = saved_r14;
    cpu->pc = cpu->pr;
}