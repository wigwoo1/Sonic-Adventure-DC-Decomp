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

static uint32_t offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

SA_EXPORT int32_t sa_startup_hex_digit_index_8c011042(
        const uint8_t *ram, struct sa_startup_state *state)
{
    uint32_t index = 0u;
    uint32_t digit = (uint32_t)(int32_t)(int8_t)state->r[4];
    state->r[1] = read32(ram, 0x8c0110bcu);
    state->r[6] = 16u;
    state->r[7] = 0xffffffffu;

    while (index < 16u) {
        uint32_t character = (uint32_t)(int32_t)(int8_t)
                ram[offset(state->r[1] + index)];
        state->r[2] = character;
        state->r[3] = digit;
        state->sr = (state->sr & ~1u) | (uint32_t)(character == digit);
        if (character == digit) {
            state->r[7] = index;
            break;
        }
        ++index;
        state->sr = (state->sr & ~1u) | (uint32_t)(index >= 16u);
    }

    state->r[0] = state->r[7];
    state->pc = state->pr;
    return (int32_t)state->r[0];
}
