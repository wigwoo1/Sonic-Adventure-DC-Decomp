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

SA_EXPORT void sa_b_chaos0_set_state_bit_0c904f40(
    uint8_t *ram, struct sa_cpu_state *state)
{
    uint32_t at = ram_index(state->r[4] + 3u);
    uint8_t current = ram[at];
    state->sr = (state->sr & ~1u) | (uint32_t)((current & 1u) == 0u);
    ram[at] = current | 1u;
    state->pc = state->pr;
}
