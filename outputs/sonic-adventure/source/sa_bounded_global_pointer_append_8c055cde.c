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
    const uint32_t at = ram_index(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_bounded_global_pointer_append_8c055cde(
    uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[6] = read32(ram, 0x8c055d08u);
    state->r[3] = 8u;
    state->r[5] = read32(ram, state->r[6]);
    const uint32_t count_at_least_eight =
        ((state->r[5] & 0x80000000u) == 0u) &&
        (state->r[5] >= state->r[3]);
    state->sr = (state->sr & ~1u) | count_at_least_eight;
    if ((state->sr & 1u) == 0u) {
        state->r[0] = read32(ram, 0x8c055d0cu);
        state->r[5] <<= 2;
        write32(ram, state->r[0] + state->r[5], state->r[4]);
        state->r[3] = read32(ram, state->r[6]);
        state->r[3] += 1u;
        write32(ram, state->r[6], state->r[3]);
    }
    state->pc = state->pr;
}
