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

SA_EXPORT void sa_bounded_byte_compare_8c1107a4(
    const uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t left = state->r[4];
    uint32_t right = state->r[5];
    uint32_t count = state->r[6];
    uint32_t index = 0;
    uint8_t left_byte = 0;
    uint8_t right_byte = 0;
    state->r[7] = right;
    if (count == 0) {
        state->r[0] = 0;
        state->sr |= 1u;
    } else {
        while (index < count) {
            left_byte = ram[ram_index(left)];
            right_byte = ram[ram_index(right)];
            state->r[2] = (uint32_t)(int32_t)(int8_t)right_byte;
            state->r[3] = right_byte;
            left++;
            state->r[4] = left;
            state->r[7] = right;
            if (left_byte != right_byte) {
                state->r[5] = index;
                state->r[0] = (uint32_t)left_byte - (uint32_t)right_byte;
                state->sr &= ~1u;
                break;
            }
            right++;
            index++;
            state->r[5] = index;
            if (index == count) {
                state->r[0] = 0;
                state->sr |= 1u;
            }
        }
    }
    state->pc = state->pr;
}
