#include <stdint.h>

#define RAM_BASE 0x0c000000u

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu;

static uint32_t offset(uint32_t address)
{
    return (address & 0x1fffffffu) - RAM_BASE;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void indirect_tail(const uint8_t *ram, sa_cpu *cpu,
                          uint32_t entry, uint32_t literal_displacement,
                          uint32_t register_index)
{
    uint32_t literal = ((entry + 4u) & ~3u) + literal_displacement * 4u;
    uint32_t target = read32(ram, literal);
    cpu->r[register_index] = target;
    cpu->pc = target;
}

__declspec(dllexport)
void sa_al_main_thunk_ext_fun_8c010b00(const uint8_t *ram, sa_cpu *cpu)
{
    indirect_tail(ram, cpu, 0x0c901ae6u, 0x2au, 3u);
}

__declspec(dllexport)
void sa_b_chaos0_thunk_ext_fun_8c107aaa(const uint8_t *ram, sa_cpu *cpu)
{
    indirect_tail(ram, cpu, 0x0c9002f6u, 0x0bu, 3u);
}

__declspec(dllexport)
void sa_b_chaos6_thunk_ext_fun_8c049a4e(const uint8_t *ram, sa_cpu *cpu)
{
    indirect_tail(ram, cpu, 0x0c9030bcu, 0x16u, 3u);
}

__declspec(dllexport)
void sa_b_egm2_thunk_ext_fun_8c0606c6(const uint8_t *ram, sa_cpu *cpu)
{
    indirect_tail(ram, cpu, 0x0c906d6cu, 0x0cu, 2u);
}

__declspec(dllexport)
void sa_advertise_thunk_ext_fun_8c01140a(const uint8_t *ram, sa_cpu *cpu)
{
    indirect_tail(ram, cpu, 0x8c901356u, 0x15u, 3u);
}

__declspec(dllexport)
void sa_advertise_thunk_ext_fun_8c0114f0(const uint8_t *ram, sa_cpu *cpu)
{
    indirect_tail(ram, cpu, 0x8c90135cu, 0x14u, 3u);
}

__declspec(dllexport)
void sa_advertise_thunk_ext_fun_8c011606(const uint8_t *ram, sa_cpu *cpu)
{
    indirect_tail(ram, cpu, 0x8c901362u, 0x14u, 3u);
}

__declspec(dllexport)
void sa_advertise_thunk_ext_fun_8c011238(const uint8_t *ram, sa_cpu *cpu)
{
    indirect_tail(ram, cpu, 0x8c9013c0u, 0x07u, 3u);
}

__declspec(dllexport)
void sa_advertise_thunk_ext_fun_8c088680(const uint8_t *ram, sa_cpu *cpu)
{
    indirect_tail(ram, cpu, 0x8c9013c6u, 0x07u, 3u);
}

__declspec(dllexport)
void sa_advertise_thunk_ext_fun_8c107ada(const uint8_t *ram, sa_cpu *cpu)
{
    indirect_tail(ram, cpu, 0x8c9018a4u, 0x1du, 1u);
}

__declspec(dllexport)
void sa_advertise_thunk_ext_fun_8c107af6(const uint8_t *ram, sa_cpu *cpu)
{
    indirect_tail(ram, cpu, 0x8c9018aau, 0x1du, 1u);
}

__declspec(dllexport)
void sa_advertise_thunk_ext_fun_8c010b00(const uint8_t *ram, sa_cpu *cpu)
{
    indirect_tail(ram, cpu, 0x8c9024c6u, 0x55u, 3u);
}
