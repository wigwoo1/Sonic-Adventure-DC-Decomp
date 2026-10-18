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

static uint32_t arithmetic_shift_one(uint32_t count)
{
    uint32_t amount = ((~count) & 31u) + 1u;
    if (amount >= 32u) {
        return 0u;
    }
    return (uint32_t)((int32_t)1 >> amount);
}

SA_EXPORT void sa_startup_state_initialize_8c010b00(
        uint8_t *ram, struct sa_startup_state *state,
        sa_startup_dispatch dispatch, void *context)
{
    state->r[6] = 2u;
    state->r[4] = read32(ram, 0x8c010c00u);
    state->r[15] -= 4u;
    write32(ram, state->r[15], state->pr);
    state->r[3] = read32(ram, 0x8c010c04u);
    state->r[6] = (state->r[6] << 8) + 0x60u;
    state->pr = 0x8c010b10u;
    state->r[5] = 0u;
    dispatch(ram, state, state->r[3], context);
    state->pc = state->pr;

    state->r[2] = read32(ram, 0x8c010c10u);
    state->r[5] = 1u;
    state->r[1] = read32(ram, 0x8c010c14u);
    state->r[3] = read32(ram, state->r[2]);
    state->r[6] = read32(ram, 0x8c010c0cu);
    state->r[7] = read32(ram, 0x8c010c08u);
    if ((int32_t)state->r[3] < 0) {
        state->r[5] = arithmetic_shift_one(state->r[3]);
    } else {
        state->r[5] = 1u << (state->r[3] & 31u);
    }
    state->pr = 0x8c010b22u;
    state->r[4] = 0x0fu;
    dispatch(ram, state, state->r[1], context);
    state->pc = state->pr;

    state->r[0] = read32(ram, 0x8c010c1cu);
    state->r[3] = 0u;
    state->r[2] = read32(ram, 0x8c010c18u);
    state->r[1] = state->r[3];
    state->r[1] = read32(ram, 0x8c010c20u);
    write32(ram, state->r[2], state->r[3]);
    write32(ram, state->r[0], state->r[3]);
    state->r[3] = 1u;
    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    write32(ram, state->r[1], state->r[3]);
    state->pc = state->pr;
}
