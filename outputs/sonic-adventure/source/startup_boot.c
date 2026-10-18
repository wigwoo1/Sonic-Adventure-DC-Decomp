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

typedef void (*sa_boot_dispatch)(uint8_t *ram, struct sa_startup_state *state,
                                 uint32_t target, void *context);

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = address - 0x8c000000u;
    return (uint32_t)ram[offset] | ((uint32_t)ram[offset + 1u] << 8) |
           ((uint32_t)ram[offset + 2u] << 16) | ((uint32_t)ram[offset + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t offset = address - 0x8c000000u;
    ram[offset] = (uint8_t)value;
    ram[offset + 1u] = (uint8_t)(value >> 8);
    ram[offset + 2u] = (uint8_t)(value >> 16);
    ram[offset + 3u] = (uint8_t)(value >> 24);
}

static void call_service(uint8_t *ram, struct sa_startup_state *state,
                         sa_boot_dispatch dispatch, void *context,
                         uint32_t target, uint32_t return_address)
{
    state->pr = return_address;
    dispatch(ram, state, target, context);
    state->pc = state->pr;
}

static void restore_stack(uint8_t *ram, struct sa_startup_state *state)
{
    state->r[15] += 0x0cu;
    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->r[9] = read32(ram, state->r[15]); state->r[15] += 4u;
    state->r[10] = read32(ram, state->r[15]); state->r[15] += 4u;
    state->r[11] = read32(ram, state->r[15]); state->r[15] += 4u;
    state->r[12] = read32(ram, state->r[15]); state->r[15] += 4u;
    state->r[13] = read32(ram, state->r[15]); state->r[15] += 4u;
    state->r[14] = read32(ram, state->r[15]); state->r[15] += 4u;
}

static void cleanup_and_report(uint8_t *ram, struct sa_startup_state *state,
                               sa_boot_dispatch dispatch, void *context,
                               uint32_t cleanup_return, uint32_t report_return)
{
    call_service(ram, state, dispatch, context, 0x8c01025au, cleanup_return);
    call_service(ram, state, dispatch, context, state->r[13], report_return);
}

/* Retail 0x8c010180 initializes early services and retries the boot-disc
   transfer at most eight times. Firmware and subsystem calls use a native
   dispatcher because their original SH-4 code is outside this routine. */
SA_EXPORT void sa_startup_boot_8c010180(uint8_t *ram,
                                        struct sa_startup_state *state,
                                        sa_boot_dispatch dispatch,
                                        void *context)
{
    state->r[15] -= 4u; write32(ram, state->r[15], state->r[14]);
    state->r[15] -= 4u; write32(ram, state->r[15], state->r[13]);
    state->r[15] -= 4u; write32(ram, state->r[15], state->r[12]);
    state->r[15] -= 4u; write32(ram, state->r[15], state->r[11]);
    state->r[15] -= 4u; write32(ram, state->r[15], state->r[10]);
    state->r[15] -= 4u; write32(ram, state->r[15], state->r[9]);
    state->r[15] -= 4u; write32(ram, state->r[15], state->pr);
    state->r[15] -= 0x0cu;

    write32(ram, state->r[15] + 4u, state->r[4]);
    write32(ram, state->r[15], state->r[5]);
    write32(ram, state->r[15] + 8u, state->r[6]);

    state->r[0] = state->sr;
    state->r[3] = 0xffffff0fu;
    state->r[0] = (state->r[0] & state->r[3]) | 0xf0u;
    state->sr = state->r[0];

    call_service(ram, state, dispatch, context, read32(ram, 0x8c0102a0u), 0x8c0101aau);
    call_service(ram, state, dispatch, context, read32(ram, 0x8c0102a4u), 0x8c0101b0u);

    state->r[2] = read32(ram, 0x8c0102b0u);
    state->r[3] = read32(ram, 0x8c0102acu);
    state->r[4] = read32(ram, state->r[2]);
    state->r[1] = read32(ram, 0x8c0102b4u);
    state->r[4] = (state->r[4] & state->r[3]) | state->r[1];
    state->r[2] = read32(ram, 0x8c0102b8u);
    state->r[3] = read32(ram, 0x8c0102bcu);
    state->r[5] = read32(ram, 0x8c0102a8u);
    state->r[4] += state->r[2];
    call_service(ram, state, dispatch, context, state->r[3], 0x8c0101c6u);

    state->r[3] = read32(ram, 0x8c0102c0u);
    state->r[5] = read32(ram, state->r[15]);
    state->r[6] = read32(ram, state->r[15] + 8u);
    state->r[4] = read32(ram, state->r[15] + 4u);
    call_service(ram, state, dispatch, context, state->r[3], 0x8c0101d0u);

    call_service(ram, state, dispatch, context, read32(ram, 0x8c0102c4u), 0x8c0101d6u);
    state->r[3] = read32(ram, 0x8c0102d0u);
    state->r[5] = read32(ram, 0x8c0102ccu);
    state->r[6] = read32(ram, 0x8c0102c8u);
    state->r[4] = 0u;
    call_service(ram, state, dispatch, context, state->r[3], 0x8c0101e0u);
    state->r[4] = 0u;
    call_service(ram, state, dispatch, context, read32(ram, 0x8c0102d4u), 0x8c0101e6u);

    state->r[3] = 0xffffffffu;
    state->r[3] = (state->r[3] << 8) + 0x0fu;
    state->r[0] = state->sr & state->r[3];
    state->sr = state->r[0];
    state->r[11] = read32(ram, 0x8c0102dcu);
    state->r[4] = 0xffffffe0u;
    state->r[9] = read32(ram, 0x8c0102d8u);
    state->r[12] = 0x800u + 0x34u;
    state->r[10] = read32(ram, 0x8c0102e0u);
    state->r[13] = read32(ram, 0x8c0102e4u);
    state->r[9] &= state->r[4];
    state->r[11] &= state->r[4];
    state->r[9] += 0x20u;
    state->r[11] += 0x20u;
    state->r[14] = 8u;

    for (;;) {
        state->r[5] = state->r[9];
        state->r[6] = state->r[12];
        state->r[7] = state->r[11];
        state->r[4] = 8u;
        call_service(ram, state, dispatch, context, state->r[10], 0x8c010216u);
        uint32_t status = state->r[0];
        state->sr = (state->sr & ~1u) | (uint32_t)(status == 0xffffffe9u);
        state->r[4] = status; /* mov r0,r4 is the BT/S delay-slot instruction. */
        if (status == 0xffffffe9u) {
            cleanup_and_report(ram, state, dispatch, context, 0x8c010226u, 0x8c01022au);
        } else {
            state->r[0] = state->r[4];
            state->sr = (state->sr & ~1u) | (uint32_t)(status == 0xffffffdfu);
            if (status == 0xffffffdfu) {
                cleanup_and_report(ram, state, dispatch, context, 0x8c010226u, 0x8c01022au);
            } else {
                state->sr = (state->sr & ~1u) | (uint32_t)(state->r[4] == 0u);
                if (state->r[4] == 0u)
                    break;
            }
        }

        --state->r[14];
        state->sr = (state->sr & ~1u) | (uint32_t)(state->r[14] != 0u);
        if (state->r[14] == 0u)
            break;
    }

    state->sr = (state->sr & ~1u) | (uint32_t)(state->r[14] == 0u);
    if (state->r[14] == 0u)
        cleanup_and_report(ram, state, dispatch, context, 0x8c010240u, 0x8c010244u);

    state->r[4] = read32(ram, 0x8c0102e8u);
    restore_stack(ram, state);
    state->r[2] = read32(ram, 0x8c0102ecu);
    call_service(ram, state, dispatch, context, state->r[2], state->pr);
}
