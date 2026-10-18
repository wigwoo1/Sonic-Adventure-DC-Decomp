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

typedef void (*sa_input_dispatch)(uint8_t *, struct sa_cpu_state *,
                                  uint32_t, void *);

extern void sa_input_index_to_code_8c0a6992(uint8_t *, struct sa_cpu_state *);
extern void sa_input_mapping_value_8c0a69a8(uint8_t *, struct sa_cpu_state *);
extern void sa_input_bitset_set_8c0a6918(uint8_t *, struct sa_cpu_state *,
                                         sa_input_dispatch, void *);

static void set_mapped_index(uint8_t *ram, struct sa_cpu_state *state,
                             uint32_t mapped, sa_input_dispatch dispatch,
                             void *context)
{
    struct sa_cpu_state setter = *state;
    setter.r[4] = state->r[4];
    setter.r[5] = mapped;
    sa_input_bitset_set_8c0a6918(ram, &setter, dispatch, context);
    for (unsigned i = 0; i < 8; ++i)
        state->r[i] = setter.r[i];
    state->sr = setter.sr;
    state->pc = state->pr;
}

SA_EXPORT void sa_input_index_bitset_set_8c0a6a88(
        uint8_t *ram, struct sa_cpu_state *state,
        sa_input_dispatch dispatch, void *context)
{
    struct sa_cpu_state mapper = *state;
    mapper.r[4] = state->r[5];
    sa_input_index_to_code_8c0a6992(ram, &mapper);
    set_mapped_index(ram, state, mapper.r[0], dispatch, context);
}

SA_EXPORT void sa_input_mapping_bitset_set_8c0a6ad2(
        uint8_t *ram, struct sa_cpu_state *state,
        sa_input_dispatch dispatch, void *context)
{
    struct sa_cpu_state mapper = *state;
    mapper.r[4] = state->r[5];
    mapper.r[5] = state->r[6];
    mapper.r[6] = state->r[7];
    sa_input_mapping_value_8c0a69a8(ram, &mapper);
    set_mapped_index(ram, state, mapper.r[0], dispatch, context);
}
