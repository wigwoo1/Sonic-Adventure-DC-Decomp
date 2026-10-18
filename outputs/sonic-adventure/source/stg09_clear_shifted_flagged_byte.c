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

SA_EXPORT void sa_stg09_clear_shifted_flagged_byte_0c903a6e(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t address = state->r[4];
    const uint32_t value = (uint32_t)(int32_t)(int8_t)ram[ram_index(address + 1u)];
    const uint32_t is_clearable = (value & 3u) == 0u;

    state->r[0] = value;
    state->sr = (state->sr & ~1u) | is_clearable;
    if (is_clearable) {
        state->r[0] = 0u;
        ram[ram_index(address + 9u)] = 0u;
    }
    state->pc = state->pr;
}
