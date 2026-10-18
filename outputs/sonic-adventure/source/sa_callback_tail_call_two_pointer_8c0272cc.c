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

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = (address & 0x1fffffffu) - 0x0c000000u;
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

SA_EXPORT void sa_callback_tail_call_two_pointer_8c0272cc(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[3] = read32(ram, 0x8c027344u);
    state->r[5] = 2u;
    state->r[6] = read32(ram, 0x8c027340u);
    state->r[4] = state->r[5];
    state->pc = state->r[3];
}
