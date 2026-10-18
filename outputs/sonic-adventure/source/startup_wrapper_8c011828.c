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

SA_EXPORT int32_t sa_startup_service_wrapper_8c011828(
        uint8_t *ram, struct sa_startup_state *state,
        sa_startup_dispatch dispatch, void *context)
{
    uint32_t caller_sp = state->r[15];
    uint32_t frame = caller_sp - 0x40u;
    uint32_t saved_pr = caller_sp - 4u;
    uint32_t original_r4 = state->r[4];
    uint32_t service = read32(ram, 0x8c01187cu);
    uint32_t global_address = read32(ram, 0x8c011878u);

    write32(ram, saved_pr, state->pr);
    write32(ram, frame, original_r4);
    state->r[15] = frame;
    state->r[2] = service;
    state->r[3] = global_address;
    state->r[4] = read32(ram, global_address);
    state->r[5] = frame + 4u;
    invoke(ram, state, dispatch, context, service, 0x8c01183au);

    state->r[3] = global_address;
    state->r[4] = read32(ram, global_address);
    state->r[5] = read32(ram, frame);
    invoke(ram, state, dispatch, context, 0x8c010fccu, 0x8c011842u);

    state->pr = read32(ram, saved_pr);
    state->r[15] = caller_sp;
    state->pc = state->pr;
    return (int32_t)state->r[0];
}
