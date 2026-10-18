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

static void indirect_tail(uint8_t *ram, sa_cpu_state *cpu, uint32_t literal)
{
    const uint32_t target = read32(ram, literal);
    cpu->r[3] = target;
    cpu->pc = target;
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_input_request_completion_thunk_8c646884(uint8_t *ram, sa_cpu_state *cpu)
{
    indirect_tail(ram, cpu, 0x8c6468c0u);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_input_device_busy_thunk_8c646b36(uint8_t *ram, sa_cpu_state *cpu)
{
    indirect_tail(ram, cpu, 0x8c646c34u);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_input_device_request_thunk_8c646b3c(uint8_t *ram, sa_cpu_state *cpu)
{
    indirect_tail(ram, cpu, 0x8c646c38u);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_input_device_state_get_8c646eda(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t pointer = read32(ram, 0x8c646f58u);
    cpu->r[3] = pointer;
    cpu->r[0] = read32(ram, pointer);
    cpu->pc = cpu->pr;
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_input_status_table_store_8c646eee(uint8_t *ram, sa_cpu_state *cpu)
{
    cpu->r[3] = 0x1fu;
    const uint32_t out_of_range = cpu->r[4] > cpu->r[3];
    cpu->sr = (cpu->sr & ~1u) | out_of_range;
    if (out_of_range == 0u) {
        cpu->r[0] = read32(ram, 0x8c646f64u);
        cpu->r[4] <<= 2;
        write32(ram, cpu->r[0] + cpu->r[4], cpu->r[5]);
    }
    cpu->pc = cpu->pr;
}