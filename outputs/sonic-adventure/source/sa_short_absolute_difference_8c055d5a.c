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

SA_EXPORT void sa_short_absolute_difference_8c055d5a(
    uint8_t *ram, struct sa_cpu_state *state)
{
    (void)ram;
    state->r[5] &= 0xffffu;
    state->r[3] = state->r[5];
    state->r[4] &= 0xffffu;
    state->r[3] -= state->r[4];
    state->r[4] = state->r[3];
    state->r[2] = state->r[4] & 0xffffu;
    if (state->r[2] & 0x8000u) {
        state->r[2] |= 0xffff0000u;
    }
    state->sr = (state->sr & ~1u) | ((state->r[2] & 0x80000000u) == 0u);
    if ((state->sr & 1u) == 0u) {
        state->r[4] = 0u - state->r[4];
    }
    state->r[0] = state->r[4] & 0xffffu;
    state->pc = state->pr;
}
