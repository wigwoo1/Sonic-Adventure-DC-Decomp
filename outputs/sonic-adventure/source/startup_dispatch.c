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

typedef void (*sa_startup_dispatch)(uint8_t *ram,
                                    struct sa_startup_state *state,
                                    uint32_t target,
                                    void *context);

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = address - 0x8c000000u;
    return (uint32_t)ram[offset] | ((uint32_t)ram[offset + 1u] << 8) |
           ((uint32_t)ram[offset + 2u] << 16) | ((uint32_t)ram[offset + 3u] << 24);
}

/* Retail 0x8c0100c8 tail-dispatches the routine pointer stored at 0x8c01014c,
   passing the word referenced by the pointer at 0x8c010148 in R4. */
SA_EXPORT void sa_startup_dispatch_8c0100c8(uint8_t *ram,
                                              struct sa_startup_state *state,
                                              sa_startup_dispatch dispatch,
                                              void *context)
{
    state->r[2] = read32(ram, 0x8c01014cu);
    state->r[3] = read32(ram, 0x8c010148u);
    state->r[4] = read32(ram, state->r[3]);
    dispatch(ram, state, state->r[2], context);
    state->pc = state->pr;
}

/* Retail 0x8c0100d0 uses the same argument cell with a second service target. */
SA_EXPORT void sa_startup_dispatch_8c0100d0(uint8_t *ram,
                                              struct sa_startup_state *state,
                                              sa_startup_dispatch dispatch,
                                              void *context)
{
    state->r[3] = read32(ram, 0x8c010150u);
    state->r[2] = read32(ram, 0x8c010148u);
    state->r[4] = read32(ram, state->r[2]);
    dispatch(ram, state, state->r[3], context);
    state->pc = state->pr;
}
