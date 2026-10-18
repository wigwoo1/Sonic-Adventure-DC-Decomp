#include <stdint.h>

#include "fpu_cleanup_transfer.h"

#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif

struct sa_cpu_state {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
};

extern int sa_signed_division(
    uint8_t *memory, struct sa_fpu_banks *fpu, uint32_t *sr,
    uint32_t *registers, uint32_t *pr, uint32_t *fpul, uint32_t *macl,
    void *context,
    int (*call)(void *, uint32_t, uint8_t *, struct sa_fpu_banks *,
                uint32_t *, uint32_t *, uint32_t *));

static uint32_t ram_index(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_index(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_b_chaos4_list_callback_update_0c900aa4(
    uint8_t *ram, struct sa_cpu_state *state)
{
    struct sa_fpu_banks fpu = {{0}, 0};
    uint32_t fpul = 0;
    uint32_t macl = 0;
    uint32_t list = state->r[4];
    uint32_t callback = read32(ram, 0x0c900ae8u);
    uint32_t add_mask = read32(ram, 0x0c900aecu);
    uint32_t clear_mask = read32(ram, 0x0c900af4u);
    uint32_t final_mask = read32(ram, 0x0c900af0u);

    state->r[15] -= 8u;
    state->r[0] = 0x64u;
    state->r[1] = (state->r[5] << 8) - state->r[5];
    state->r[2] = callback;
    state->r[3] = state->r[5];
    (void)sa_signed_division(ram, &fpu, &state->sr, state->r,
                             &state->pr, &fpul, &macl, 0, 0);
    state->r[15] += 8u;
    uint8_t callback_value = (uint8_t)state->r[0];

    for (;;) {
        uint32_t node = read32(ram, list);
        if (node == 0u) {
            break;
        }
        list += 4u;
        uint32_t child = read32(ram, node + 4u);
        if (child != 0u) {
            uint32_t object = read32(ram, child + 0x10u);
            uint32_t flags = read32(ram, object + 0x10u);
            flags = (flags | add_mask) & clear_mask;
            flags |= final_mask;
            write32(ram, object + 0x10u, flags);
            ram[ram_index(object + 3u)] = callback_value;
        }
    }
    state->pc = state->pr;
}
