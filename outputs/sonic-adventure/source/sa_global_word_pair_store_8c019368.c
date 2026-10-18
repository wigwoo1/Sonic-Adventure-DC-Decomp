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

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static uint32_t sign_extend_byte(uint32_t value)
{
    value &= 0xffu;
    return (value & 0x80u) != 0u ? value | 0xffffff00u : value;
}

SA_EXPORT void sa_global_word_pair_store_8c019368(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t value4 = sign_extend_byte(state->r[4]);
    state->r[3] = 0x8c6b7084u;
    state->r[4] = value4;
    state->r[2] = 0x8c6b7088u;
    const uint32_t value5 = sign_extend_byte(state->r[5]);
    state->r[5] = value5;
    write32(ram, state->r[3], state->r[4]);
    write32(ram, state->r[2], state->r[5]);
    state->pc = state->pr;
}
