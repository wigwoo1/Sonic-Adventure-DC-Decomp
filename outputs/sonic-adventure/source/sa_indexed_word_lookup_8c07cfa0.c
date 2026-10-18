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
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

SA_EXPORT void sa_indexed_word_lookup_8c07cfa0(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t index = state->r[4] & 0xffu;
    const uint32_t table = read32(ram, 0x8c07d040u);
    state->r[1] = table;
    state->r[0] = read32(ram, table + (index << 2));
    state->pc = state->pr;
}
