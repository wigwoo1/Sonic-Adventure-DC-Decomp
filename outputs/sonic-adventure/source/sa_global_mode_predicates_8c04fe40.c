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

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_index(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static int32_t read_signed16(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_index(address);
    const uint16_t value = (uint16_t)((uint16_t)ram[at] |
                                      ((uint16_t)ram[at + 1u] << 8));
    return (int16_t)value;
}

static void return_boolean(struct sa_cpu_state *state, uint32_t result)
{
    state->r[0] = result;
    state->sr = (state->sr & ~1u) | result;
    state->pc = state->pr;
}

SA_EXPORT void sa_global_mode_is_0x10_8c04fe40(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[3] = read32(ram, 0x8c04fe8cu);
    return_boolean(state, read_signed16(ram, state->r[3]) == 0x10);
}

SA_EXPORT void sa_global_mode_is_0xf_4_or_2_8c04fe4a(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[3] = read32(ram, 0x8c04fe8cu);
    state->r[0] = (uint32_t)read_signed16(ram, state->r[3]);
    state->r[4] = state->r[0];
    return_boolean(state, state->r[0] == 0x0fu ||
                          state->r[0] == 4u || state->r[0] == 2u);
}
