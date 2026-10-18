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

SA_EXPORT void sa_bounded_string_copy_8c10f95c(
    uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t destination = state->r[4];
    uint32_t source = state->r[5];
    uint32_t length = state->r[6];
    uint32_t copied = 0;
    uint32_t last = state->r[3];
    int terminated = 0;
    while (copied < length) {
        uint8_t value = 0;
        if (!terminated) {
            value = ram[ram_index(source)];
            source++;
            if (value == 0) terminated = 1;
        }
        ram[ram_index(destination + copied)] = value;
        last = value;
        copied++;
    }
    state->r[0] = destination;
    state->r[1] = 0;
    if (copied != 0) {
        state->r[3] = (uint32_t)(int32_t)(int8_t)last;
    }
    state->r[5] = copied;
    state->r[7] = destination + length;
    state->sr |= 1u;
    state->pc = state->pr;
}
