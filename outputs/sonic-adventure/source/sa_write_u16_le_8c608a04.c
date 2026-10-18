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

SA_EXPORT void sa_write_u16_le_8c608a04(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t destination = state->r[5];
    const uint32_t value = state->r[4];
    ram[ram_index(destination)] = (uint8_t)value;
    state->r[4] = value >> 8;
    state->r[0] = state->r[4];
    ram[ram_index(destination + 1u)] = (uint8_t)state->r[0];
    state->pc = state->pr;
}
