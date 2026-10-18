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

SA_EXPORT void sa_zero_byte_range_check_8c11025c(
    const uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t cursor = state->r[4];
    int32_t remaining = (int32_t)state->r[5];
    state->r[6] = 0;
    state->r[0] = 1;
    state->sr &= ~1u;
    if (remaining > 0) {
        while (remaining > 0) {
            int32_t value = (int32_t)(int8_t)ram[ram_index(cursor)];
            state->r[2] = (uint32_t)value;
            if (value != 0) {
                state->r[0] = 0;
                state->sr &= ~1u;
                break;
            }
            cursor++;
            state->r[4] = cursor;
            state->r[6]++;
            remaining--;
        }
        if (remaining == 0) {
            state->sr |= 1u;
        }
    }
    state->pc = state->pr;
}
