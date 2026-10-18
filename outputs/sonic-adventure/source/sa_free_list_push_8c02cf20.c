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

SA_EXPORT void sa_free_list_push_8c02cf20(uint8_t *ram, struct sa_cpu_state *state) {
    const uint32_t free_head_address = 0x8c02d544u;
    uint32_t node = state->r[4];
    uint32_t next = read32(ram, node);
    uint32_t previous = read32(ram, node + 4);
    uint32_t old_free_head = read32(ram, free_head_address);

    if (next != 0) write32(ram, next + 4, previous);
    if ((int32_t)previous < 0) write32(ram, previous, next);
    else write32(ram, 0u - previous, next);
    write32(ram, free_head_address, node);
    write32(ram, node, old_free_head);

    state->r[0] = old_free_head;
    state->r[1] = free_head_address;
    state->r[2] = next;
    state->r[3] = (int32_t)previous < 0 ? previous : 0u - previous;
    state->sr = (state->sr & ~1u) | ((int32_t)previous >= 0);
    state->pc = state->pr;
}
