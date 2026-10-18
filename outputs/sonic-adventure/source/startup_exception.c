#include <stdint.h>

#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif

struct sa_startup_exception_state {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
};

typedef void (*sa_startup_exception_dispatch)(uint8_t *,
        struct sa_startup_exception_state *, uint32_t, void *);

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = address - 0x8c000000u;
    return (uint32_t)ram[offset] | ((uint32_t)ram[offset + 1u] << 8) |
           ((uint32_t)ram[offset + 2u] << 16) |
           ((uint32_t)ram[offset + 3u] << 24);
}

SA_EXPORT void sa_startup_exception_8c01025a(
        uint8_t *ram, struct sa_startup_exception_state *state,
        sa_startup_exception_dispatch dispatch, void *context)
{
    static const uint32_t targets[] = {
        0x8c60164au, 0x8c10ba72u, 0x8c601f84u,
        0x8c6052e0u, 0x8c60215eu, 0x8c60250au
    };
    static const uint32_t returns[] = {
        0x8c010262u, 0x8c010268u, 0x8c01026eu,
        0x8c010274u, 0x8c01027au, 0x8c010280u
    };

    state->r[15] -= 4u;
    ram[state->r[15] - 0x8c000000u] = (uint8_t)state->pr;
    ram[state->r[15] - 0x8c000000u + 1u] = (uint8_t)(state->pr >> 8);
    ram[state->r[15] - 0x8c000000u + 2u] = (uint8_t)(state->pr >> 16);
    ram[state->r[15] - 0x8c000000u + 3u] = (uint8_t)(state->pr >> 24);
    for (unsigned i = 0; i < sizeof(targets) / sizeof(targets[0]); ++i) {
        state->r[(i & 1u) ? 2u : 3u] = targets[i];
        state->pr = returns[i];
        dispatch(ram, state, targets[i], context);
        state->pc = state->pr;
    }

    state->r[3] = 0xffffffffu;
    state->r[3] <<= 8;
    state->r[3] += 0x0fu;
    state->r[0] = (state->sr & state->r[3]) | 0xf0u;
    state->sr = state->r[0];

    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->pc = state->pr;
}
