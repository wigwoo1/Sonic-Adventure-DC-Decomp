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

SA_EXPORT void sa_string_append_8c10d3a8(
    uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t destination = state->r[4];
    uint32_t source = state->r[5];
    uint32_t destination_end = destination;
    while (ram[ram_index(destination_end)] != 0) destination_end++;
    for (;;) {
        uint8_t value = ram[ram_index(source++)];
        ram[ram_index(destination_end++)] = value;
        if (value == 0) break;
    }
    state->r[0] = destination;
    state->r[2] = 0;
    state->r[6] = destination_end;
    state->r[7] = source;
    state->sr |= 1u;
    state->pc = state->pr;
}
