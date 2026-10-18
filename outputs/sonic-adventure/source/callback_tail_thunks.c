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

typedef void (*sa_callback_dispatch)(uint8_t *, struct sa_cpu_state *,
                                     uint32_t, void *);

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = address & 0x00ffffffu;
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void tail_call(uint8_t *ram, struct sa_cpu_state *state,
                      sa_callback_dispatch dispatch, void *context,
                      uint32_t literal, unsigned target_register)
{
    uint32_t target = read32(ram, literal);
    state->r[target_register] = target;
    state->pc = target;
    dispatch(ram, state, target, context);
    state->pc = state->pr;
}

SA_EXPORT void sa_callback_tail_8c014422(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c014460u, 3u); }
SA_EXPORT void sa_callback_tail_8c0544dc(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c054618u, 3u); }
SA_EXPORT void sa_callback_tail_8c054998(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c0549e8u, 3u); }
SA_EXPORT void sa_callback_tail_8c0585ae(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c05860cu, 3u); }
SA_EXPORT void sa_callback_tail_8c0585b4(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c058610u, 3u); }
SA_EXPORT void sa_callback_tail_8c0585ca(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c058604u, 3u); }
SA_EXPORT void sa_callback_tail_8c0585d0(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c058600u, 3u); }
SA_EXPORT void sa_callback_tail_8c0585d6(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c058618u, 3u); }
SA_EXPORT void sa_callback_tail_8c0585e8(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c058620u, 3u); }
SA_EXPORT void sa_callback_tail_8c058640(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c058680u, 3u); }
SA_EXPORT void sa_callback_tail_8c09da08(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c09da3cu, 3u); }
SA_EXPORT void sa_callback_tail_8c0d2156(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c0d21b8u, 3u); }
SA_EXPORT void sa_callback_tail_8c600032(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c6000e0u, 3u); }
SA_EXPORT void sa_callback_tail_8c601874(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c6019f8u, 3u); }
SA_EXPORT void sa_callback_tail_8c602dda(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c602f2cu, 3u); }
SA_EXPORT void sa_callback_tail_8c602df2(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c602f3cu, 3u); }
SA_EXPORT void sa_callback_tail_8c602e9c(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c602f50u, 3u); }
SA_EXPORT void sa_callback_tail_8c604370(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c604434u, 3u); }
SA_EXPORT void sa_callback_tail_8c6055ba(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c6055d0u, 3u); }
SA_EXPORT void sa_callback_tail_8c63fe3c(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c63fe44u, 3u); }
SA_EXPORT void sa_callback_tail_8c63fe48(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c63fe50u, 3u); }
SA_EXPORT void sa_callback_tail_8c64489c(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c6448fcu, 3u); }
SA_EXPORT void sa_callback_tail_8c646884(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c6468c0u, 3u); }
SA_EXPORT void sa_callback_tail_8c646b36(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c646c34u, 3u); }
SA_EXPORT void sa_callback_tail_8c646b3c(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c646c38u, 3u); }
SA_EXPORT void sa_callback_tail_8c647908(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c64797cu, 3u); }
SA_EXPORT void sa_callback_tail_8c65b4c8(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c65b588u, 3u); }
SA_EXPORT void sa_callback_tail_8c65b4d4(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c65b590u, 3u); }
SA_EXPORT void sa_callback_tail_8c65b4f8(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c65b5a0u, 3u); }
SA_EXPORT void sa_callback_tail_8c65b4fe(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c65b5a4u, 3u); }
SA_EXPORT void sa_callback_tail_8c65b50a(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c65b5acu, 3u); }
SA_EXPORT void sa_callback_tail_8c65b51a(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c65b5b4u, 3u); }
SA_EXPORT void sa_callback_tail_8c65b526(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c65b5bcu, 3u); }
SA_EXPORT void sa_callback_tail_8c65b52c(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c65b5c0u, 3u); }
SA_EXPORT void sa_callback_tail_8c65b538(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c65b5c8u, 3u); }
SA_EXPORT void sa_callback_tail_8c65b53e(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c65b5ccu, 3u); }
SA_EXPORT void sa_callback_tail_8c65b55c(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c65b5e0u, 3u); }
SA_EXPORT void sa_callback_tail_8c66086a(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c660890u, 2u); }
SA_EXPORT void sa_callback_tail_8c669260(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c669268u, 3u); }
SA_EXPORT void sa_callback_tail_8c66926c(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c669274u, 3u); }
SA_EXPORT void sa_callback_tail_8c669444(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c66944cu, 3u); }
SA_EXPORT void sa_callback_tail_8c6694dc(uint8_t *ram, struct sa_cpu_state *state,
        sa_callback_dispatch dispatch, void *context)
{ tail_call(ram, state, dispatch, context, 0x8c6694e4u, 3u); }
