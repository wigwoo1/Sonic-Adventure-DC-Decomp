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

static void poll_until_ready(uint8_t *ram, struct sa_startup_state *state,
                             sa_startup_dispatch dispatch, void *context,
                             uint32_t call_return)
{
    do {
        state->r[4] = read32(ram, state->r[12]);
        invoke(ram, state, dispatch, context, state->r[11], call_return);
        uint32_t retry = state->r[0] == 0xffffffffu;
        state->sr = (state->sr & ~1u) | retry;
        if (!retry) {
            return;
        }
    } while (1);
}

SA_EXPORT int32_t sa_startup_transfer_entry_8c011606(
        uint8_t *ram, struct sa_startup_state *state,
        sa_startup_dispatch dispatch, void *context)
{
    uint32_t caller_sp = state->r[15];
    uint32_t frame = caller_sp - 0xccu;
    uint32_t saved = caller_sp - 0x18u;
    uint32_t local = frame + 0x9cu;
    uint32_t buffer = frame + 4u;
    uint32_t identifier = read32(ram, 0x8c0116d0u);
    uint32_t staging = 0x8c6b2654u;
    uint32_t destination = 0x8c7989a0u;
    uint32_t argument_pointer = 0x8c161ca8u;

    write32(ram, saved, state->pr);
    write32(ram, saved + 4u, state->r[10]);
    write32(ram, saved + 8u, state->r[11]);
    write32(ram, saved + 12u, state->r[12]);
    write32(ram, saved + 16u, state->r[13]);
    write32(ram, saved + 20u, state->r[14]);
    state->r[15] = frame;

    state->r[1] = 0u;
    state->r[2] = 0x8c161cacu;
    state->r[3] = 0x8c111279u;
    state->r[10] = local;
    state->r[11] = 0x8c6033e4u;
    state->r[12] = argument_pointer;
    state->r[14] = staging;
    for (uint32_t index = 0; index < 16u; ++index) {
        ram[offset(local) + index] = ram[offset(identifier) + index];
    }
    uint32_t enabled = read32(ram, state->r[2]) == 0u;
    state->sr = (state->sr & ~1u) | enabled;

    int32_t result;
    if (!enabled) {
        result = -3;
    } else {
        poll_until_ready(ram, state, dispatch, context, 0x8c01167eu);

        state->r[3] = 0x8c602f5cu;
        state->r[4] = read32(ram, argument_pointer);
        state->r[5] = buffer;
        invoke(ram, state, dispatch, context, state->r[3], 0x8c01168cu);

        state->r[13] = destination;
        state->r[3] = 0x8c603474u;
        state->r[4] = read32(ram, argument_pointer);
        state->r[5] = local;
        invoke(ram, state, dispatch, context, state->r[3], 0x8c011696u);
        uint32_t metadata = state->r[0];
        write32(ram, frame, metadata);

        state->r[4] = read32(ram, argument_pointer);
        state->r[5] = local;
        invoke(ram, state, dispatch, context, 0x8c010fccu, 0x8c01169eu);
        if (state->r[0] != 0u) {
            state->r[4] = state->r[0];
            result = -1;
        } else {
            state->r[3] = 0x8c603508u;
            state->r[4] = read32(ram, argument_pointer);
            state->r[5] = local;
            state->r[6] = staging;
            state->r[7] = metadata;
            invoke(ram, state, dispatch, context, state->r[3],
                   0x8c01170cu);

            poll_until_ready(ram, state, dispatch, context, 0x8c011710u);

            state->r[3] = 0x8c0887c8u;
            state->r[2] = 0x480u;
            state->r[4] = staging + 0x480u;
            state->r[14] = staging + 0x480u;
            invoke(ram, state, dispatch, context, state->r[3],
                   0x8c011722u);
            state->sr = (state->sr & ~1u) | (state->r[0] == 0u);
            if (state->r[0] == 0u) {
                result = -2;
            } else {
                uint32_t source = staging + 0x480u;
                uint32_t target = destination;
                uint32_t count = 0u;
                while (count < 0xde0u) {
                    uint32_t src = offset(source);
                    uint32_t dst = offset(target);
                    ram[dst] = ram[src];
                    ram[dst + 1u] = ram[src + 1u];
                    ram[dst + 2u] = ram[src + 2u];
                    source += 3u;
                    target += 3u;
                    count += 3u;
                }
                state->sr |= 1u;
                state->r[2] = 0x21u;
                state->r[3] = 0x8c088680u;
                state->r[4] = 0xde0u;
                state->r[5] = 0xde0u;
                state->r[13] = target;
                state->r[14] = source;
                invoke(ram, state, dispatch, context, state->r[3],
                       0x8c011750u);
                result = 0;
            }
        }
    }

    state->pr = read32(ram, saved);
    state->r[10] = read32(ram, saved + 4u);
    state->r[11] = read32(ram, saved + 8u);
    state->r[12] = read32(ram, saved + 12u);
    state->r[13] = read32(ram, saved + 16u);
    state->r[14] = read32(ram, saved + 20u);
    state->r[15] = caller_sp;
    state->r[1] = 0xb4u;
    state->pc = state->pr;
    state->r[0] = (uint32_t)result;
    return result;
}
