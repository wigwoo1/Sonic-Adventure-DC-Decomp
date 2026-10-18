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

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static uint16_t read16(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint16_t)((uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8));
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

/* Retail entry 0x8c10d334. Update the global LCG seed and return its masked
 * upper 16 bits, while preserving the SH-4 MACL value saved on the stack. */
SA_EXPORT void sa_random_u15_8c10d334(
    uint8_t *ram,
    struct sa_cpu_state *state,
    uint32_t *macl)
{
    const uint32_t addend = read16(ram, 0x8c10d352u);
    const uint32_t mask = read32(ram, 0x8c10d354u);
    const uint32_t seed_address = read32(ram, 0x8c10d358u);
    const uint32_t multiplier = read32(ram, 0x8c10d35cu);
    const uint32_t seed = read32(ram, seed_address);
    const uint32_t next_seed = seed * multiplier + addend;
    const uint32_t saved_macl = *macl;

    write32(ram, state->r[15] - 4u, saved_macl);
    write32(ram, seed_address, next_seed);
    *macl = saved_macl;

    state->r[0] = (next_seed >> 16) & mask;
    state->r[1] = addend;
    state->r[2] = mask;
    state->r[3] = next_seed;
    state->r[4] = seed_address;
    state->pc = state->pr;
}
