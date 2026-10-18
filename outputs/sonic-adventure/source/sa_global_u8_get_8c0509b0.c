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

SA_EXPORT void sa_global_u8_get_8c0509b0(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t address = 0x8c19e3a8u;
    const uint32_t at = ram_index(address);
    state->r[3] = address;
    state->r[0] = ram[at];
    state->pc = state->pr;
}
