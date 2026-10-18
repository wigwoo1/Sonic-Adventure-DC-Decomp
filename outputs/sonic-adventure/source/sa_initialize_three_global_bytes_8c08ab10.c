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

SA_EXPORT void sa_initialize_three_global_bytes_8c08ab10(
    uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[2] = 0x8c752be1u;
    state->r[1] = 0u;
    state->r[3] = 0x8c752be2u;
    state->r[0] = 0x8c752be3u;
    ram[ram_index(state->r[3])] = (uint8_t)state->r[5];
    ram[ram_index(state->r[2])] = (uint8_t)state->r[4];
    ram[ram_index(state->r[0])] = (uint8_t)state->r[1];
    state->pc = state->pr;
}
