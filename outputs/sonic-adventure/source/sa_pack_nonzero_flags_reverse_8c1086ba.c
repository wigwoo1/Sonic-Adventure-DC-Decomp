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

static void write32(uint8_t *ram, uint32_t address, uint32_t value) {
    uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1] = (uint8_t)(value >> 8);
    ram[at + 2] = (uint8_t)(value >> 16);
    ram[at + 3] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_pack_nonzero_flags_reverse_8c1086ba(
    uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t source = state->r[4];
    uint32_t base = state->r[5];
    uint32_t destination = base + 0xbfu;
    uint32_t mask = 1u;
    uint8_t output = 0;
    uint32_t last = state->r[1];
    int32_t last_source = 0;
    write32(ram, state->r[15] - 4u, state->r[14]);
    write32(ram, state->r[15] - 8u, state->r[13]);
    write32(ram, state->r[15] - 12u, state->r[12]);
    for (uint32_t i = 0; i < 0x600u; ++i) {
        if (mask == 1u) {
            output = 0;
            ram[ram_index(destination)] = 0;
        }
        last_source = (int32_t)(int8_t)ram[ram_index(source)];
        if (last_source != 0) {
            output |= (uint8_t)mask;
            ram[ram_index(destination)] = output;
            last = output;
        }
        source++;
        mask <<= 1;
        if (mask == 0x100u) {
            mask = 1u;
            destination--;
        }
    }
    state->r[0] = 0x80u;
    state->r[1] = last;
    state->r[2] = (uint32_t)last_source;
    state->r[3] = 0xbfu;
    state->r[4] = source;
    state->r[5] = destination;
    state->r[6] = 1u;
    state->r[7] = 1u;
    state->sr |= 1u;
    state->pc = state->pr;
}
