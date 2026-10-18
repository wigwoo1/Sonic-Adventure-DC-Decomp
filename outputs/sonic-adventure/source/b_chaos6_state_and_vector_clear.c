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

static uint32_t ram_index(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_index(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_b_chaos6_state_and_vector_clear_0c9062ac(
    uint8_t *ram, struct sa_fpu_banks *fpu, struct sa_cpu_state *state)
{
    const uint32_t work = read32(ram, state->r[4] + 0x20u);
    const uint32_t current = read32(ram, work + 8u);
    const uint32_t parameter = state->r[5];

    state->r[4] = work;
    state->r[3] = current;
    state->sr = (state->sr & ~1u) | (uint32_t)(current == parameter);
    if (current != parameter) {
        const uint32_t active = (fpu->fpscr >> 21) & 1u;
        const uint32_t width = (fpu->fpscr >> 20) & 1u;
        fpu->bank[active][3] = 0u;
        state->r[0] = 12u;
        write32(ram, work + 8u, parameter);
        if (width == 0u) {
            write32(ram, work + 12u, 0u);
        } else {
            const uint32_t source_bank = active ^ 1u;
            write32(ram, work + 12u, fpu->bank[source_bank][2]);
            write32(ram, work + 16u, fpu->bank[source_bank][3]);
        }
    }
    state->pc = state->pr;
}
