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

SA_EXPORT void sa_work_id_clear_8c02ce18(uint8_t *ram, struct sa_cpu_state *state) {
    const uint32_t head_address = 0x8c02d548u;
    uint32_t node = read32(ram, head_address);
    uint32_t target = state->r[4];
    state->r[3] = head_address;
    state->r[5] = node;
    state->r[6] = 0;
    while (node != 0) {
        uint32_t identifier = read32(ram, node + 8);
        state->r[3] = identifier;
        if (identifier == target) {
            write32(ram, node + 8, 0);
            break;
        }
        node = read32(ram, node);
        state->r[5] = node;
    }
    state->sr |= 1u;
    state->pc = state->pr;
}
