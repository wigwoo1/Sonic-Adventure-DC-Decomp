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

SA_EXPORT void sa_store_u16_be_8c111174(
    uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t destination = state->r[4];
    uint32_t source = state->r[5];
    uint32_t source_offset = ram_index(source);
    uint16_t raw = (uint16_t)((uint16_t)ram[source_offset] |
                              ((uint16_t)ram[source_offset + 1] << 8));
    uint32_t value = raw;
    ram[ram_index(destination)] = (uint8_t)(raw >> 8);
    source_offset = ram_index(source);
    raw = (uint16_t)((uint16_t)ram[source_offset] |
                     ((uint16_t)ram[source_offset + 1] << 8));
    value = raw;
    ram[ram_index(destination + 1u)] = (uint8_t)raw;
    state->r[0] = destination + 2u;
    state->r[2] = 0;
    state->r[3] = value;
    state->r[6] = destination + 2u;
    state->r[7] = 0xfffffff8u;
    state->sr |= 1u;
    state->pc = state->pr;
}
