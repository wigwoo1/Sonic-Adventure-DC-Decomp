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

typedef void (*sa_startup_dispatch)(uint8_t *, struct sa_startup_state *,
                                    uint32_t, void *);

static uint32_t offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static void invoke(uint8_t *ram, struct sa_startup_state *state,
                   sa_startup_dispatch dispatch, void *context,
                   uint32_t target, uint32_t call_return)
{
    state->pr = call_return;
    dispatch(ram, state, target, context);
    state->pc = state->pr;
}

SA_EXPORT int32_t sa_startup_callback_flag_reset_8c107aaa(
        uint8_t *ram, struct sa_startup_state *state,
        sa_startup_dispatch dispatch, void *context)
{
    state->r[3] = read32(ram, 0x8c107b80u);
    state->r[15] -= 4u;
    write32(ram, state->r[15], state->pr);
    state->r[4] = read32(ram, state->r[3]);
    state->sr = (state->sr & ~1u) | (uint32_t)(state->r[4] == 0u);

    if (state->r[4] != 0u) {
        state->r[1] = read32(ram, 0x8c107b84u);
        invoke(ram, state, dispatch, context, state->r[1], 0x8c107ab8u);
        state->r[2] = read32(ram, 0x8c107b80u);
        state->r[3] = 0u;
        write32(ram, state->r[2], state->r[3]);
    }

    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->pc = state->pr;
    return (int32_t)state->r[0];
}
