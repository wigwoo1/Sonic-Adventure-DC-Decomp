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

SA_EXPORT void sa_byte_copy_8c107fa4(uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t source = state->r[4];
    uint32_t destination = state->r[5];
    uint32_t end = source + state->r[6];
    state->r[7] = end;

    while (source < end) {
        uint8_t value = ram[ram_index(source++)];
        ram[ram_index(destination++)] = value;
        state->r[2] = (uint32_t)(int32_t)(int8_t)value;
    }

    state->r[4] = source;
    state->r[5] = destination;
    state->sr |= 1u;
    state->pc = state->pr;
}
