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

static uint32_t read32(const uint8_t *ram, uint32_t address) {
    uint32_t offset = ram_index(address);
    return (uint32_t)ram[offset] |
           ((uint32_t)ram[offset + 1] << 8) |
           ((uint32_t)ram[offset + 2] << 16) |
           ((uint32_t)ram[offset + 3] << 24);
}

SA_EXPORT void sa_conditional_byte_clear_8c10e566(
    uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t argument = state->r[4];
    uint32_t condition = read32(ram, argument);
    state->r[0] = condition;
    state->sr = (state->sr & ~1u) | (condition == 1u);
    if (condition == 1u) {
        uint32_t destination = read32(ram, argument + 4u);
        state->r[2] = destination;
        state->r[3] = 0;
        ram[ram_index(destination)] = 0;
    }
    state->pc = state->pr;
}
