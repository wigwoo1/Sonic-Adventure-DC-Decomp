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

static uint32_t offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_zero_two_globals_8c062634(uint8_t *ram,
                                             struct sa_cpu_state *state)
{
    state->r[3] = read32(ram, 0x8c062740u);
    state->r[4] = 0u;
    state->r[2] = read32(ram, 0x8c062744u);
    write32(ram, state->r[3], state->r[4]);
    write32(ram, state->r[2], state->r[4]);
    state->pc = state->pr;
}

SA_EXPORT void sa_zero_global_return_value_8c5ff2de(
        uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[2] = read32(ram, 0x8c5ff4b4u);
    state->r[3] = 0u;
    write32(ram, state->r[2], state->r[3]);
    state->r[0] = state->r[3];
    state->pc = state->pr;
}
