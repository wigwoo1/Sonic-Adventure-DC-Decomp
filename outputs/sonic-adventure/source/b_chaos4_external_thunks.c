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

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_index(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void tail_dispatch(const uint8_t *ram, struct sa_cpu_state *state,
                          uint32_t literal_address)
{
    state->r[3] = read32(ram, literal_address);
    state->pc = state->r[3];
}

SA_EXPORT void sa_b_chaos4_thunk_ext_fun_8c0606c6_0c904654(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    tail_dispatch(ram, state, 0x0c90467cu);
}

SA_EXPORT void sa_b_chaos4_thunk_ext_fun_8c107aaa_0c907116(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    tail_dispatch(ram, state, 0x0c907154u);
}

SA_EXPORT void sa_b_chaos4_thunk_0c90424c(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[3] = read32(ram, 0x0c904358u);
    state->r[4] = read32(ram, 0x0c904354u);
    state->pc = state->r[3];
}
