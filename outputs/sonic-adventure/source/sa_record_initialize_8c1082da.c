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

static void write16(uint8_t *ram, uint32_t address, uint16_t value) {
    uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1] = (uint8_t)(value >> 8);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value) {
    uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1] = (uint8_t)(value >> 8);
    ram[at + 2] = (uint8_t)(value >> 16);
    ram[at + 3] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_record_initialize_8c1082da(
    uint8_t *ram, struct sa_cpu_state *state) {
    uint32_t object = state->r[4];
    uint32_t value = state->r[5];
    write16(ram, object + 0x60c, 0);
    write16(ram, object + 0x60e, 0);
    write32(ram, object + 0x610, value);
    state->r[0] = 0x60e;
    state->r[5] = 0;
    state->pc = state->pr;
}
