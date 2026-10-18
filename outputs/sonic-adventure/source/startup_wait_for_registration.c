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

static void pop_saved_context(uint8_t *ram, struct sa_startup_state *state)
{
    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->r[13] = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->r[14] = read32(ram, state->r[15]);
    state->r[15] += 4u;
}

SA_EXPORT int32_t sa_startup_wait_for_registration_8c0114f0(
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
    state->pr = 0x8c0114fcu;
    dispatch(ram, state, state->r[3], context);
    state->pc = state->pr;

    state->r[3] = 0x8c161cacu;
    state->r[2] = read32(ram, state->r[3]);
    uint32_t enabled = state->r[2] == 0u;
    state->sr = (state->sr & ~1u) | enabled;
    if (!enabled) {
        state->r[0] = 0xffffffffu;
        pop_saved_context(ram, state);
        state->pc = state->pr;
        return -1;
    }

    state->r[14] = 0x8c6033e4u;
    state->r[13] = 0x8c161ca8u;
    do {
        state->pr = 0x8c011510u;
        state->r[4] = read32(ram, state->r[13]);
        dispatch(ram, state, state->r[14], context);
        state->pc = state->pr;
        uint32_t retry = state->r[0] == 0xffffffffu;
        state->sr = (state->sr & ~1u) | retry;
        if (!retry) {
            break;
        }
    } while (1);

    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->r[13] = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->r[14] = read32(ram, state->r[15]);
    state->r[15] += 4u;

    dispatch(ram, state, 0x8c0112e6u, context);
    state->pc = state->pr;
    return (int32_t)state->r[0];
}
