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

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = address & 0x00ffffffu;
    return (uint32_t)ram[offset] | ((uint32_t)ram[offset + 1u] << 8) |
           ((uint32_t)ram[offset + 2u] << 16) |
           ((uint32_t)ram[offset + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t offset = address & 0x00ffffffu;
    ram[offset] = (uint8_t)value;
    ram[offset + 1u] = (uint8_t)(value >> 8);
    ram[offset + 2u] = (uint8_t)(value >> 16);
    ram[offset + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_startup_event_poll_8c010a0e(
        uint8_t *ram, struct sa_startup_state *state,
        sa_startup_dispatch dispatch, void *context)
{
    state->r[15] -= 4u;
    write32(ram, state->r[15], state->r[14]);
    state->r[3] = 2u;
    state->r[15] -= 4u;
    write32(ram, state->r[15], state->pr);

    state->r[2] = read32(ram, 0x8c010ac0u);
    write32(ram, state->r[2], state->r[3]);
    state->r[1] = read32(ram, 0x8c010ac4u);
    write32(ram, state->r[1], state->r[4]);
    state->r[3] = read32(ram, 0x8c010ac8u);
    write32(ram, state->r[3], state->r[5]);
    state->r[2] += 12u;
    write32(ram, state->r[2], state->r[6]);

    state->r[0] = read32(ram, 0x8c010ad0u);
    state->r[1] = 1u;
    write32(ram, state->r[0], state->r[1]);
    state->r[14] = read32(ram, 0x8c010accu);
    do {
        state->pr = 0x8c010a30u;
        dispatch(ram, state, state->r[14], context);
        state->pc = state->pr;
        state->r[2] = read32(ram, 0x8c010ad0u);
        state->r[3] = read32(ram, state->r[2]);
        state->sr = (state->sr & ~1u) | (uint32_t)(state->r[3] == 0u);
    } while (!(state->sr & 1u));

    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->r[14] = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->pc = state->pr;
}
