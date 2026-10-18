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

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static uint32_t arithmetic_shift_right_11(uint32_t value)
{
    uint32_t shifted = value >> 11;
    if ((value & 0x80000000u) != 0u)
        shifted |= 0xffe00000u;
    return shifted;
}

/* Retail entry 0x8c65bfc2: store a direct or rounded 2 KiB stream position. */
SA_EXPORT void sa_stream_position_set_8c65bfc2(
    uint8_t *ram,
    struct sa_cpu_state *state)
{
    const uint32_t object = state->r[4];
    const uint32_t input = state->r[5];
    uint32_t position = input;

    if ((input & 0x80000000u) != 0u) {
        const uint32_t biased = read32(ram, object + 0x14u) + 0x07ffu;
        position = biased;
        if ((biased & 0x80000000u) != 0u)
            position += 0x07ffu;
        position = arithmetic_shift_right_11(position);
        state->r[1] = 0xfffffff5u;
        state->r[2] = position;
        state->r[3] = 0x07ffu;
        state->sr = (state->sr & ~1u) | ((biased >> 31) ^ 1u);
    } else {
        state->sr |= 1u;
    }

    write32(ram, object + 0x30u, position);
    state->pc = state->pr;
}
