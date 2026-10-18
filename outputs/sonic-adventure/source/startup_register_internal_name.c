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

SA_EXPORT int32_t sa_startup_register_internal_name_8c0113da(
        uint8_t *ram, struct sa_startup_state *state,
        sa_startup_dispatch dispatch, void *context)
{
    state->r[15] -= 4u;
    write32(ram, state->r[15], state->pr);
    state->r[3] = 0x80000000u;
    state->r[2] = 0x8c6b685cu;
    state->r[15] -= 4u;
    write32(ram, state->r[15], state->r[3]);
    state->r[1] = 0x8c6b6858u;
    state->r[15] -= 4u;
    write32(ram, state->r[15], state->r[2]);

    state->r[3] = 0x8c161ca8u;
    state->r[2] = 0x8c603560u;
    state->r[5] = 0x8c161cbcu;
    state->r[6] = 0x8c6b2654u;
    state->r[7] = read32(ram, state->r[1]);
    state->pr = 0x8c0113f4u;
    state->r[4] = read32(ram, state->r[3]);
    dispatch(ram, state, state->r[2], context);
    state->pc = state->pr;

    state->r[4] = state->r[0];
    uint32_t success = state->r[4] == 0u;
    state->sr = (state->sr & ~1u) | success;
    state->r[15] += 8u;
    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    if (success) {
        state->r[0] = 0u;
    } else {
        state->r[0] = 0xffffffffu;
    }
    state->pc = state->pr;
    return (int32_t)state->r[0];
}
