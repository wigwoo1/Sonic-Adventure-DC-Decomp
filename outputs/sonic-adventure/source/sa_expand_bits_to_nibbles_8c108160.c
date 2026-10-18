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

SA_EXPORT void sa_expand_bits_to_nibbles_8c108160(
    uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t source = state->r[4];
    uint32_t destination = state->r[5];
    uint32_t remaining = state->r[6];
    uint32_t mask = 0x80u;
    uint32_t current = 0;
    write32(ram, state->r[15] - 4u, state->r[14]);
    state->r[0] = 0;
    state->r[1] = 0x0fu;
    state->sr &= ~1u;
    if ((int32_t)remaining > 0) {
        while (remaining != 0) {
            current = ram[ram_index(source)];
            ram[ram_index(destination)] = (current & mask) ? 0x0fu : 0;
            destination++;
            remaining--;
            mask >>= 1;
            if (mask == 0) {
                source++;
                mask = 0x80u;
            }
        }
        state->r[2] = current;
        state->r[4] = source;
        state->r[5] = destination;
        state->r[6] = 0;
    }
    state->r[7] = mask;
    state->pc = state->pr;
}
