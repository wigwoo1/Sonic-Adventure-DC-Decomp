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
    uint32_t at = ram_index(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1] << 8) |
           ((uint32_t)ram[at + 2] << 16) | ((uint32_t)ram[at + 3] << 24);
}

SA_EXPORT void sa_store_u32_be_8c110c9c(
    uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t destination = state->r[4];
    uint32_t source = state->r[5];
    uint32_t value = 0;
    for (uint32_t i = 0; i < 4; ++i) {
        value = read32(ram, source);
        ram[ram_index(destination + i)] = (uint8_t)(value >> (24u - i * 8u));
    }
    state->r[0] = destination + 4u;
    state->r[2] = value;
    state->r[3] = 0;
    state->r[6] = destination + 4u;
    state->r[7] = 0xfffffff8u;
    state->sr |= 1u;
    state->pc = state->pr;
}
