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

extern void sa_input_mapping_value_8c0a69a8(uint8_t *, struct sa_cpu_state *);
extern void sa_input_bitset_test_8c0a68d8(uint8_t *, struct sa_cpu_state *);

SA_EXPORT void sa_input_mapping_query_8c0a6ab4(uint8_t *ram,
                                                struct sa_cpu_state *state)
{
    struct sa_cpu_state helper = *state;
    helper.r[4] = state->r[5];
    helper.r[5] = state->r[6];
    helper.r[6] = state->r[7];
    sa_input_mapping_value_8c0a69a8(ram, &helper);
    uint32_t mapped_index = helper.r[0];

    helper = *state;
    helper.r[4] = state->r[4];
    helper.r[5] = mapped_index;
    sa_input_bitset_test_8c0a68d8(ram, &helper);
    state->r[0] = helper.r[0];
    state->pc = state->pr;
}
