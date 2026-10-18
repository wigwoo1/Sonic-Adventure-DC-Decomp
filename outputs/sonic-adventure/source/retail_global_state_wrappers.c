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

SA_EXPORT void sa_global_setter_8c64401c(uint8_t *ram,
                                          struct sa_cpu_state *state)
{
    state->r[3] = read32(ram, 0x8c6440e8u);
    write32(ram, state->r[3], state->r[4]);
    state->pc = state->pr;
}

SA_EXPORT void sa_dual_global_setter_8c606f14(uint8_t *ram,
                                               struct sa_cpu_state *state)
{
    state->r[5] = read32(ram, 0x8c607054u);
    state->r[3] = state->r[4];
    state->r[2] = read32(ram, 0x8c607058u);
    write32(ram, state->r[5], state->r[4]);
    write32(ram, state->r[2], state->r[4]);
    state->pc = state->pr;
}

SA_EXPORT void sa_dual_argument_global_setter_8c60ede6(
        uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[2] = read32(ram, 0x8c60ee38u);
    state->r[3] = read32(ram, 0x8c60ee3cu);
    write32(ram, state->r[2], state->r[4]);
    write32(ram, state->r[3], state->r[5]);
    state->pc = state->pr;
}

SA_EXPORT void sa_conditional_global_setter_8c107ba0(
        uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[2] = read32(ram, 0x8c107c60u);
    state->r[3] = read32(ram, state->r[2]);
    state->sr = (state->sr & ~1u) | (uint32_t)(state->r[3] == 0u);
    if (state->r[3] != 0u) {
        state->r[3] = read32(ram, 0x8c107c64u);
        write32(ram, state->r[3], state->r[4]);
    }
    state->pc = state->pr;
}
