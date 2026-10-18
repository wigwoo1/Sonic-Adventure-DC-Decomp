#include <stdint.h>

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

static uint32_t ram_index(uint32_t address) {
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value) {
    uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1] = (uint8_t)(value >> 8);
    ram[at + 2] = (uint8_t)(value >> 16);
    ram[at + 3] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_shift_right_bytes_with_carry_8c111128(
    uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t start = state->r[4];
    uint32_t cursor = start;
    uint32_t count = state->r[5];
    uint32_t carry = 0;
    uint32_t shifted = state->r[3];
    write32(ram, state->r[15] - 4u, state->r[14]);
    write32(ram, state->r[15] - 8u, state->r[13]);
    write32(ram, state->r[15] - 12u, state->r[12]);
    write32(ram, state->r[15] - 16u, start);
    if ((int32_t)count > 0) {
        for (uint32_t i = 0; i < count; ++i) {
            uint32_t at = ram_index(cursor);
            uint8_t value = ram[at];
            uint32_t next_carry = value & 1u;
            shifted = value >> 1;
            value = (uint8_t)(shifted | (carry ? 0x80u : 0u));
            ram[at] = value;
            carry = next_carry;
            cursor++;
        }
        state->r[3] = shifted;
        state->r[4] = cursor;
        state->r[6] = cursor;
        state->r[7] = carry;
        state->r[0] = carry;
        state->sr |= 1u;
    } else {
        state->r[0] = 0;
        state->r[7] = 0;
        state->r[6] = start;
        state->sr &= ~1u;
    }
    state->pc = state->pr;
}
