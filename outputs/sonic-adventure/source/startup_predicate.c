#include <stdint.h>

#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif

struct sa_startup_state {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
};

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = address & 0x00ffffffu;
    return (uint32_t)ram[offset] | ((uint32_t)ram[offset + 1u] << 8) |
           ((uint32_t)ram[offset + 2u] << 16) |
           ((uint32_t)ram[offset + 3u] << 24);
}

SA_EXPORT void sa_startup_predicate_8c010b58(uint8_t *ram,
                                              struct sa_startup_state *state)
{
    state->r[0] = read32(ram, 0x8c010c20u);
    state->r[1] = read32(ram, state->r[0]);
    state->sr = (state->sr & ~1u) | (uint32_t)(state->r[1] == 0u);
    if (state->sr & 1u) {
        state->r[0] = 0u;
        state->pc = state->pr;
        return;
    }

    state->r[3] = read32(ram, 0x8c010c1cu);
    state->r[2] = 1u;
    state->r[4] = read32(ram, state->r[3]);
    state->sr = (state->sr & ~1u) |
                (uint32_t)((int32_t)state->r[4] > (int32_t)state->r[2]);
    if (!(state->sr & 1u)) {
        state->r[0] = 0u;
        state->pc = state->pr;
        return;
    }

    state->r[0] = 3u;
    state->sr = (state->sr & ~1u) |
                (uint32_t)((int32_t)state->r[4] > (int32_t)state->r[0]);
    state->r[0] = (state->sr & 1u) ? 0u : 0xffffffffu;
    state->pc = state->pr;
}
