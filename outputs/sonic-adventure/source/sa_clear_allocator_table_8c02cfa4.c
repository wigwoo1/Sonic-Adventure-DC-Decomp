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

static uint32_t index_of(uint32_t address) {
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value) {
    uint32_t offset = index_of(address);
    volatile uint8_t *target = ram + offset;
    target[0] = (uint8_t)value;
    target[1] = (uint8_t)(value >> 8);
    target[2] = (uint8_t)(value >> 16);
    target[3] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_clear_allocator_table_8c02cfa4(uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t cursor = 0x8c02d560u;
    for (uint32_t i = 0; i < 128; ++i, cursor += 8u) {
        write32(ram, cursor, 0);
        write32(ram, cursor + 4u, 0);
    }
    state->r[0] = 0;
    state->r[1] = cursor;
    state->r[7] = 0;
    state->sr |= 1u;
    state->pc = state->pr;
}
