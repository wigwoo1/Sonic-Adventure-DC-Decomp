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

SA_EXPORT void sa_indexed_flag_set_8c09067e(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t table = 0x8c799920u;
    state->r[0] = table;
    state->r[3] = 1u;
    ram[ram_index(table + state->r[4])] = (uint8_t)state->r[3];
    state->pc = state->pr;
}
