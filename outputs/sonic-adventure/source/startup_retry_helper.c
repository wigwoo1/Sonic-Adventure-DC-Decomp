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

static void set_t(struct sa_startup_state *state, uint32_t value)
{
    state->sr = (state->sr & ~1u) | (value & 1u);
}

static void invoke(uint8_t *ram, struct sa_startup_state *state,
                   sa_startup_dispatch dispatch, void *context,
                   uint32_t target, uint32_t call_return)
{
    state->pr = call_return;
    dispatch(ram, state, target, context);
    state->pc = state->pr;
}

static uint32_t signed_nonnegative(uint32_t value)
{
    return value < 0x80000000u;
}

SA_EXPORT int32_t sa_startup_retry_helper_8c010fcc(
        uint8_t *ram, struct sa_startup_state *state,
        sa_startup_dispatch dispatch, void *context)
{
    uint32_t saved = state->r[15] - 0x1cu;
    uint32_t arg0 = state->r[4];
    uint32_t arg1 = state->r[5];
    uint32_t frame = 0u;
    uint32_t local = 0u;
    write32(ram, saved, state->pr);
    write32(ram, saved + 4u, state->r[9]);
    write32(ram, saved + 8u, state->r[10]);
    write32(ram, saved + 12u, state->r[11]);
    write32(ram, saved + 16u, state->r[12]);
    write32(ram, saved + 20u, state->r[13]);
    write32(ram, saved + 24u, state->r[14]);
    state->r[15] = saved;

    state->r[2] = 0x8c161cacu;
    state->r[3] = read32(ram, state->r[2]);
    set_t(state, state->r[3] == 0u);
    uint32_t first = state->r[3];
    uint32_t current = first;
    uint32_t second = 0xffffffffu;
    state->r[9] = arg1;
    state->r[12] = arg0;

    if (state->r[3] != 0u) {
        state->r[0] = 0xffffffffu;
    } else {
        state->r[15] -= 0x14u;
        frame = state->r[15];
        local = frame + 4u;
        state->r[3] = 0x8c602fb8u;
        state->r[4] = arg0;
        state->r[5] = local;
        invoke(ram, state, dispatch, context, state->r[3], 0x8c010ff6u);
        first = state->r[0];
        current = first;
        write32(ram, frame, first);

        state->r[14] = 0x8c10d3c8u;
        state->r[4] = local;
        state->r[5] = arg1;
        invoke(ram, state, dispatch, context, state->r[14], 0x8c011002u);
        second = state->r[0];
        state->r[4] = second;
        state->r[3] = first;

        for (;;) {
            uint32_t first_ok = signed_nonnegative(current);
            set_t(state, first_ok);
            if (!first_ok) {
                break;
            }
            uint32_t second_nonzero = second != 0u;
            set_t(state, !second_nonzero);
            if (!second_nonzero) {
                break;
            }

            state->r[10] = 0x8c602fe6u;
            state->r[11] = local;
            state->r[4] = arg0;
            state->r[5] = local;
            invoke(ram, state, dispatch, context, state->r[10],
                   0x8c01101cu);
            current = state->r[0];
            state->r[13] = current;

            state->r[14] = 0x8c10d3c8u;
            state->r[4] = local;
            state->r[5] = arg1;
            invoke(ram, state, dispatch, context, state->r[14],
                   0x8c011024u);
            second = state->r[0];
            state->r[4] = second;
        }
        state->r[0] = second;
    }

    if (frame != 0u) {
        state->r[15] += 0x14u;
    }
    state->pr = read32(ram, saved);
    state->r[9] = read32(ram, saved + 4u);
    state->r[10] = read32(ram, saved + 8u);
    state->r[11] = read32(ram, saved + 12u);
    state->r[12] = read32(ram, saved + 16u);
    state->r[13] = read32(ram, saved + 20u);
    state->r[14] = read32(ram, saved + 24u);
    state->r[15] += 0x1cu;
    state->pc = state->pr;
    return (int32_t)state->r[0];
}
