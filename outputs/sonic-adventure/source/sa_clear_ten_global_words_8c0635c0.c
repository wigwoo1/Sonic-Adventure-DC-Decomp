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

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_clear_ten_global_words_8c0635c0(
    uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[6] = 0x8c78c564u;
    state->r[4] = 0u;
    state->r[5] = 10u;
    do {
        write32(ram, state->r[6], state->r[4]);
        state->r[5] -= 2u;
        state->r[6] += 4u;
        state->sr = (state->sr & ~1u) | (state->r[5] == 0u);
        write32(ram, state->r[6], state->r[4]);
        state->r[6] += 4u;
    } while ((state->sr & 1u) == 0u);
    state->pc = state->pr;
}
