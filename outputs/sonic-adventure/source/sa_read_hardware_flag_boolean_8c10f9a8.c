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

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_read_hardware_flag_boolean_8c10f9a8(
    uint8_t *ram, struct sa_cpu_state *state, uint32_t hardware_register_value)
{
    state->r[2] = 0xa05f688cu;
    state->r[0] = hardware_register_value;
    state->sr = (state->sr & ~1u) | (uint32_t)((state->r[0] & 1u) == 0u);
    state->r[3] = state->sr & 1u;
    write32(ram, state->r[4], state->r[3]);
    state->r[0] = 0u;
    state->pc = state->pr;
}
