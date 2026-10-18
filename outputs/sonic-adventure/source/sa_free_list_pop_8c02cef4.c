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

static uint32_t read32(uint8_t *ram, uint32_t address) {
    uint32_t offset = index_of(address);
    return (uint32_t)ram[offset] | ((uint32_t)ram[offset + 1] << 8) |
           ((uint32_t)ram[offset + 2] << 16) | ((uint32_t)ram[offset + 3] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value) {
    uint32_t offset = index_of(address);
    ram[offset] = (uint8_t)value;
    ram[offset + 1] = (uint8_t)(value >> 8);
    ram[offset + 2] = (uint8_t)(value >> 16);
    ram[offset + 3] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_free_list_pop_8c02cef4(uint8_t *ram, struct sa_cpu_state *state) {
    const uint32_t free_head_address = 0x8c02d544u;
    const uint32_t active_head_address = 0x8c02d548u;
    uint32_t node = read32(ram, free_head_address);
    uint32_t next = read32(ram, node);
    uint32_t active = read32(ram, active_head_address);

    state->r[1] = free_head_address;
    state->r[3] = next;
    state->r[0] = 0;
    if (next != 0) {
        state->r[2] = active;
        state->r[4] = 0u - active_head_address;
        state->sr = (state->sr & ~1u) | (active == 0);
        write32(ram, free_head_address, next);
        write32(ram, node, active);
        if (active != 0) write32(ram, active + 4, node);
        write32(ram, active_head_address, node);
        write32(ram, node + 4, 0u - active_head_address);
        state->r[0] = node;
    } else {
        state->sr |= 1u;
    }
    state->pc = state->pr;
}
