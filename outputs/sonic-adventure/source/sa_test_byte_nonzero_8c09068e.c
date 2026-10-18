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

SA_EXPORT void sa_test_byte_nonzero_8c09068e(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t address = 0x8c799920u + state->r[4];
    const uint32_t at = ram_index(address);
    const uint8_t value = ram[at];

    state->r[3] = value;
    state->sr = (state->sr & ~1u) | (uint32_t)(value == 0u);
    state->r[0] = (state->sr & 1u) ^ 1u;
    state->pc = state->pr;
}
