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

static int less_binary32(uint32_t left_bits, uint32_t right_bits)
{
    uint32_t left_exponent = left_bits & 0x7f800000u;
    uint32_t right_exponent = right_bits & 0x7f800000u;
    uint32_t left_magnitude = left_bits & 0x7fffffffu;
    uint32_t right_magnitude = right_bits & 0x7fffffffu;
    uint32_t left_fraction = left_bits & 0x007fffffu;
    uint32_t right_fraction = right_bits & 0x007fffffu;

    if ((left_exponent == 0x7f800000u && left_fraction != 0u) ||
        (right_exponent == 0x7f800000u && right_fraction != 0u)) {
        return 0;
    }
    if (left_magnitude == 0u && right_magnitude == 0u) {
        return 0;
    }
    if ((left_bits ^ right_bits) & 0x80000000u) {
        return (left_bits >> 31) != 0u;
    }
    if (left_bits >> 31) {
        return left_magnitude > right_magnitude;
    }
    return left_magnitude < right_magnitude;
}

SA_EXPORT void sa_b_chaos4_state_limit_0c901dd6(
    uint8_t *ram, struct sa_fpu_banks *fpu, struct sa_cpu_state *state)
{
    uint32_t object_pointer = read32(ram, 0x0c901eecu);
    uint32_t object = read32(ram, object_pointer);
    uint32_t limit = read32(ram, 0x0c901ee8u);
    uint32_t current = read32(ram, object + 0x10u);
    uint32_t *bank = fpu->bank[(fpu->fpscr >> 21) & 1u];

    /* Recovered comparison is the FPSCR.PR=0, SZ=0 single-precision path. */
    if ((fpu->fpscr & 0x00180000u) == 0u) {
        int current_is_below_limit = less_binary32(current, limit);
        bank[3] = current;
        bank[4] = limit;
        state->sr = (state->sr & ~1u) | (uint32_t)current_is_below_limit;
        if (current_is_below_limit) {
            write32(ram, object + 0x10u, limit);
        }
    }
    state->pc = state->pr;
}
