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
    ram[offset] = (uint8_t)value;
    ram[offset + 1] = (uint8_t)(value >> 8);
    ram[offset + 2] = (uint8_t)(value >> 16);
    ram[offset + 3] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_work_pool_initialize_8c02ce80(uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t base = state->r[4];
    uint32_t count = state->r[5];
    uint32_t node = base;
    uint32_t next = base + 0x40u;
    uint32_t remaining = count - 1u;
    write32(ram, 0x8c02d540u, base);
    do {
        write32(ram, node, next);
        node += 0x40u;
        next += 0x40u;
        remaining -= 1u;
    } while (remaining != 0);
    write32(ram, node, 0);
    state->r[0] = 0;
    state->r[1] = next;
    state->r[4] = node;
    state->r[5] = 0;
    state->sr |= 1u;
    state->pc = state->pr;
}
