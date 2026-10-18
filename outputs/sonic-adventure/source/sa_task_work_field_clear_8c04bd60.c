#include <stdint.h>

#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif

struct sa_cpu_state {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
};

static uint32_t ram_index(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_index(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static void write16(uint8_t *ram, uint32_t address, uint16_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
}

SA_EXPORT void sa_task_work_field_clear_8c04bd60(
    uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[4] = read32(ram, state->r[4] + 0x3cu);
    state->r[5] = 0;
    state->r[0] = 0;
    state->r[4] += 0x5cu;
    write32(ram, state->r[4] + 8u, 0);
    write32(ram, state->r[4] + 12u, 0);
    write16(ram, state->r[4] + 2u, 0);
    state->pc = state->pr;
}
