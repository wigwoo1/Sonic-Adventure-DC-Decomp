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

SA_EXPORT void sa_adv00_index_clamp_0c90ffbe(struct sa_cpu_state *state)
{
    uint32_t sum = state->r[4] + state->r[5];
    int32_t signed_sum = (int32_t)sum;
    int is_nonnegative = signed_sum >= 0;
    int at_or_above_limit = is_nonnegative && signed_sum >= (int32_t)state->r[6];

    state->r[4] = sum;
    state->sr = (state->sr & ~1u) | (uint32_t)at_or_above_limit;
    state->r[0] = (!is_nonnegative || at_or_above_limit) ? UINT32_MAX : sum;
    state->pc = state->pr;
}
