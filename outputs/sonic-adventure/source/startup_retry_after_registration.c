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

static void restore_caller(uint8_t *ram, struct sa_startup_state *state)
{
    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->r[13] = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->r[14] = read32(ram, state->r[15]);
    state->r[15] += 4u;
}

static int poll_registration(uint8_t *ram, struct sa_startup_state *state,
                             sa_startup_dispatch dispatch, void *context,
                             uint32_t call_return)
{
    do {
        state->pr = call_return;
        state->r[4] = read32(ram, state->r[13]);
        dispatch(ram, state, state->r[14], context);
        state->pc = state->pr;
        uint32_t retry = state->r[0] == 0xffffffffu;
        state->sr = (state->sr & ~1u) | retry;
        if (!retry) {
            return 0;
        }
    } while (1);
}

SA_EXPORT int32_t sa_startup_retry_after_registration_8c011524(
        uint8_t *ram, struct sa_startup_state *state,
        sa_startup_dispatch dispatch, void *context)
{
    state->r[15] -= 4u;
    write32(ram, state->r[15], state->r[14]);
    state->r[15] -= 4u;
    write32(ram, state->r[15], state->r[13]);
    state->r[15] -= 4u;
    write32(ram, state->r[15], state->pr);

    state->r[3] = 0x8c088560u;
    state->pr = 0x8c011530u;
    dispatch(ram, state, state->r[3], context);
    state->pc = state->pr;

    state->r[3] = 0x8c161cacu;
    state->r[2] = read32(ram, state->r[3]);
    uint32_t enabled = state->r[2] == 0u;
    state->sr = (state->sr & ~1u) | enabled;
    if (!enabled) {
        state->r[0] = 0xffffffffu;
        restore_caller(ram, state);
        state->pc = state->pr;
        return -1;
    }

    state->r[14] = 0x8c6033e4u;
    state->r[13] = 0x8c161ca8u;
    (void)poll_registration(ram, state, dispatch, context, 0x8c011540u);

    state->pr = 0x8c011548u;
    dispatch(ram, state, 0x8c0112e6u, context);
    state->pc = state->pr;
    uint32_t retry_poll = state->r[0] == 0u;
    state->sr = (state->sr & ~1u) | retry_poll;
    if (retry_poll) {
        (void)poll_registration(ram, state, dispatch, context, 0x8c011584u);
        state->r[0] = 0u;
    } else {
        state->r[0] = 0xffffffffu;
    }

    restore_caller(ram, state);
    state->pc = state->pr;
    return (int32_t)state->r[0];
}
