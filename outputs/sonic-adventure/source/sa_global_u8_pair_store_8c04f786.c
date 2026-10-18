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

static void write16(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
}

SA_EXPORT void sa_global_u8_pair_store_8c04f786(
    uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[3] = 0x8c7493c2u;
    state->r[4] &= 0xffu;
    state->r[2] = 0x8c7493c4u;
    state->r[5] &= 0xffu;
    write16(ram, state->r[3], state->r[4]);
    write16(ram, state->r[2], state->r[5]);
    state->pc = state->pr;
}
