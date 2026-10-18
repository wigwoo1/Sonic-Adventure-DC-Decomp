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

SA_EXPORT void sa_clear_0x600_bytes_8c107ec0(
    uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t cursor = state->r[4];
    uint32_t end = cursor + 0x600u;
    while (cursor != end) {
        ram[ram_index(cursor)] = 0;
        cursor++;
    }
    state->r[4] = 0;
    state->r[5] = end;
    state->r[6] = end;
    state->sr |= 1u;
    state->pc = state->pr;
}
