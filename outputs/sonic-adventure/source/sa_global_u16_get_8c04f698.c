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

static uint32_t read16(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_index(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8);
}

SA_EXPORT void sa_global_u16_get_8c04f698(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[2] = 0x8c7493c8u;
    state->r[0] = read16(ram, state->r[2]);
    state->pc = state->pr;
}
