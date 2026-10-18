#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = address & 0x00ffffffu;
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void tail_dispatch(const uint8_t *ram, sa_cpu_state *cpu, uint32_t entry)
{
    cpu->r[6] = 0u;
    cpu->r[7] = read32(ram, entry + 12u);
    cpu->r[0] = read32(ram, entry + 16u);
    cpu->r[0] = read32(ram, cpu->r[0]);
    cpu->pc = cpu->r[0];
}

__declspec(dllexport)
void sa_indirect_service_tail_8c10d938(const uint8_t *ram, sa_cpu_state *cpu)
{ tail_dispatch(ram, cpu, 0x8c10d938u); }

__declspec(dllexport)
void sa_indirect_service_tail_8c10d94c(const uint8_t *ram, sa_cpu_state *cpu)
{ tail_dispatch(ram, cpu, 0x8c10d94cu); }

__declspec(dllexport)
void sa_indirect_service_tail_8c10d960(const uint8_t *ram, sa_cpu_state *cpu)
{ tail_dispatch(ram, cpu, 0x8c10d960u); }

__declspec(dllexport)
void sa_indirect_service_tail_8c10d974(const uint8_t *ram, sa_cpu_state *cpu)
{ tail_dispatch(ram, cpu, 0x8c10d974u); }

__declspec(dllexport)
void sa_indirect_service_tail_8c10d988(const uint8_t *ram, sa_cpu_state *cpu)
{ tail_dispatch(ram, cpu, 0x8c10d988u); }

__declspec(dllexport)
void sa_indirect_service_tail_8c10d9b0(const uint8_t *ram, sa_cpu_state *cpu)
{ tail_dispatch(ram, cpu, 0x8c10d9b0u); }

__declspec(dllexport)
void sa_indirect_service_tail_8c10d9c4(const uint8_t *ram, sa_cpu_state *cpu)
{ tail_dispatch(ram, cpu, 0x8c10d9c4u); }
