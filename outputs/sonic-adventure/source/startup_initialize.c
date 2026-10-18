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

void sa_initialize_sections_8c0100d8(uint8_t *ram, struct sa_startup_state *state);
void sa_startup_dispatch_8c0100c8(uint8_t *ram, struct sa_startup_state *state,
                                 sa_startup_dispatch dispatch, void *context);
void sa_startup_dispatch_8c0100d0(uint8_t *ram, struct sa_startup_state *state,
                                 sa_startup_dispatch dispatch, void *context);

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = address - 0x8c000000u;
    return (uint32_t)ram[offset] | ((uint32_t)ram[offset + 1u] << 8) |
           ((uint32_t)ram[offset + 2u] << 16) | ((uint32_t)ram[offset + 3u] << 24);
}

static uint32_t read_signed_byte(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = address - 0x8c000000u;
    return (uint32_t)(int32_t)(int8_t)ram[offset];
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t offset = address - 0x8c000000u;
    ram[offset] = (uint8_t)value;
    ram[offset + 1u] = (uint8_t)(value >> 8);
    ram[offset + 2u] = (uint8_t)(value >> 16);
    ram[offset + 3u] = (uint8_t)(value >> 24);
}

/* Retail 0x8c0100a0 writes the SEGA RAM signature, initializes linker-described
   sections, calls two startup services, then tail-dispatches the final service. */
SA_EXPORT void sa_startup_initialize_8c0100a0(uint8_t *ram,
                                               struct sa_startup_state *state,
                                               sa_startup_dispatch dispatch,
                                               void *context)
{
    state->r[2] = 0x8c66c3acu;
    state->r[15] -= 4u;
    write32(ram, state->r[15], state->pr);
    state->r[3] = read32(ram, state->r[2]);
    state->r[1] = read_signed_byte(ram, state->r[3]);

    state->r[4] = 0x8c00c000u;
    state->r[5] = 0x8c00f400u;
    state->r[6] = 0x41474553u;
    do {
        write32(ram, state->r[4], state->r[6]);
        state->r[4] += 4u;
        state->sr = (state->sr & ~1u) | (uint32_t)(state->r[4] >= state->r[5]);
    } while ((state->sr & 1u) == 0u);

    state->pr = 0x8c0100bau;
    sa_initialize_sections_8c0100d8(ram, state);

    state->pr = 0x8c0100beu;
    sa_startup_dispatch_8c0100c8(ram, state, dispatch, context);

    state->r[2] = read32(ram, 0x8c010144u);
    state->pr = 0x8c0100c4u;
    dispatch(ram, state, state->r[2], context);
    state->pc = state->pr;

    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    sa_startup_dispatch_8c0100d0(ram, state, dispatch, context);
}
