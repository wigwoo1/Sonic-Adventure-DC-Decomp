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

/* Retail index name: thunk_EXT_FUN_8c01ae60. */
SA_EXPORT void sa_stg12_thunk_ext_fun_8c01ae60_0c915a80(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[3] = read32(ram, 0x0c915bc0u);
    state->pc = state->r[3];
}
