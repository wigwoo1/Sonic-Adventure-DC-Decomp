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

SA_EXPORT int32_t sa_startup_service_wrapper_8c010a6e(
        uint8_t *ram, struct sa_startup_state *state,
        sa_startup_dispatch dispatch, void *context)
{
    uint32_t caller_sp = state->r[15];
    uint32_t pr_slot = caller_sp - 4u;
    uint32_t original_pr = state->pr;
    uint32_t service = read32(ram, 0x8c010adcu);
    uint32_t tail = read32(ram, 0x8c010ae0u);

    write32(ram, pr_slot, original_pr);
    state->r[15] = pr_slot;
    state->r[3] = service;
    state->r[4] = read32(ram, 0x8c010ad4u);
    state->r[5] = read32(ram, 0x8c010ad8u);
    invoke(ram, state, dispatch, context, service, 0x8c010a7au);

    state->r[2] = tail;
    state->r[4] = 1u;
    state->r[5] = 1u;
    state->pr = read32(ram, pr_slot);
    state->r[15] = caller_sp;
    invoke(ram, state, dispatch, context, tail, original_pr);
    state->pc = state->pr;
    return (int32_t)state->r[0];
}
