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

static uint32_t ram_index(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read_signed16(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_index(address);
    const uint16_t value = (uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8);
    return (uint32_t)(int32_t)(int16_t)value;
}

static void write16(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
}

SA_EXPORT void sa_global_word_pair_copy_8c04f6a6(
    uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[1] = 0x8c7493bau;
    state->r[3] = 0x8c7493b6u;
    state->r[2] = read_signed16(ram, state->r[1]);
    write16(ram, state->r[3], state->r[2]);
    state->r[2] = 0x8c7493bcu;
    state->r[3] = 0x8c7493b8u;
    state->r[0] = read_signed16(ram, state->r[2]);
    write16(ram, state->r[3], state->r[0]);
    state->pc = state->pr;
}
