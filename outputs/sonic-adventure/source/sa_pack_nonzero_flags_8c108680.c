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

SA_EXPORT void sa_pack_nonzero_flags_8c108680(
    uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t source = state->r[4];
    uint32_t destination = state->r[5];
    uint32_t mask = 0x80u;
    uint8_t output = 0;
    uint32_t last_output_register = state->r[1];
    int32_t last = 0;
    write32(ram, state->r[15] - 4u, state->r[14]);
    for (uint32_t i = 0; i < 0x600u; ++i) {
        if (mask == 0x80u) {
            output = 0;
            ram[ram_index(destination)] = 0;
        }
        last = (int32_t)(int8_t)ram[ram_index(source)];
        if (last != 0) {
            output |= (uint8_t)mask;
            last_output_register = (uint32_t)(int32_t)(int8_t)output;
        }
        ram[ram_index(destination)] = output;
        source++;
        mask >>= 1;
        if (mask == 0) {
            destination++;
            mask = 0x80u;
        }
    }
    state->r[0] = 0;
    state->r[1] = last_output_register;
    state->r[3] = (uint32_t)last;
    state->r[4] = source;
    state->r[5] = destination;
    state->r[6] = 0x80u;
    state->r[7] = 0x80u;
    state->sr |= 1u;
    state->pc = state->pr;
}
