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

SA_EXPORT void sa_get_nested_object_word_8c0d5d80(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t object = state->r[4];
    if (object == 0u) {
        state->sr = (state->sr & ~1u) | 1u;
        state->r[0] = 0u;
    } else {
        const uint32_t nested = read32(ram, object + 0x20u);
        state->sr &= ~1u;
        state->r[0] = read32(ram, nested + 0x10u);
    }
    state->pc = state->pr;
}
