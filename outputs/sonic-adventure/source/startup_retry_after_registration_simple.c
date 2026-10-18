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

static void restore_caller(uint8_t *ram, struct sa_startup_state *state)
{
    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->r[13] = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->r[14] = read32(ram, state->r[15]);
    state->r[15] += 4u;
}

static void poll_registration(uint8_t *ram, struct sa_startup_state *state,
                              sa_startup_dispatch dispatch, void *context)
{
    do {
        state->pr = 0x8c0115ecu;
        state->r[4] = read32(ram, state->r[13]);
        dispatch(ram, state, state->r[14], context);
        state->pc = state->pr;
        uint32_t retry = state->r[0] == 0xffffffffu;
        state->sr = (state->sr & ~1u) | retry;
        if (!retry) {
            return;
        }
    } while (1);
}

SA_EXPORT int32_t sa_startup_retry_after_registration_simple_8c0115d0(
        uint8_t *ram, struct sa_startup_state *state,
        sa_startup_dispatch dispatch, void *context)
{
    state->r[15] -= 4u;
    uint32_t at = offset(state->r[15]);
    ram[at] = (uint8_t)state->r[14];
    ram[at + 1u] = (uint8_t)(state->r[14] >> 8);
    ram[at + 2u] = (uint8_t)(state->r[14] >> 16);
    ram[at + 3u] = (uint8_t)(state->r[14] >> 24);
    state->r[15] -= 4u;
    at = offset(state->r[15]);
    ram[at] = (uint8_t)state->r[13];
    ram[at + 1u] = (uint8_t)(state->r[13] >> 8);
    ram[at + 2u] = (uint8_t)(state->r[13] >> 16);
    ram[at + 3u] = (uint8_t)(state->r[13] >> 24);
    state->r[15] -= 4u;
    at = offset(state->r[15]);
    ram[at] = (uint8_t)state->pr;
    ram[at + 1u] = (uint8_t)(state->pr >> 8);
    ram[at + 2u] = (uint8_t)(state->pr >> 16);
    ram[at + 3u] = (uint8_t)(state->pr >> 24);

    state->r[3] = 0x8c088608u;
    state->pr = 0x8c0115dcu;
    dispatch(ram, state, state->r[3], context);
    state->pc = state->pr;

    state->r[3] = 0x8c161cacu;
    state->r[2] = read32(ram, state->r[3]);
    uint32_t enabled = state->r[2] == 0u;
    state->sr = (state->sr & ~1u) | enabled;
    if (enabled) {
        state->r[14] = 0x8c6033e4u;
        state->r[13] = 0x8c161ca8u;
        poll_registration(ram, state, dispatch, context);

        state->pr = 0x8c0115f4u;
        dispatch(ram, state, 0x8c0112e6u, context);
        state->pc = state->pr;
        uint32_t success = state->r[0] == 0u;
        state->sr = (state->sr & ~1u) | success;
        if (success) {
            state->r[0] = 0u;
        } else {
            state->r[0] = 0xffffffffu;
        }
    } else {
        state->r[0] = 0xffffffffu;
    }

    restore_caller(ram, state);
    state->pc = state->pr;
    return (int32_t)state->r[0];
}
