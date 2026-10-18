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

SA_EXPORT void sa_global_byte_triplet_set_8c08ab20(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t first = read32(ram, 0x8c08aba8u);
    const uint32_t second = read32(ram, 0x8c08aba4u);
    const uint32_t third = read32(ram, 0x8c08abacu);
    state->r[2] = first;
    state->r[3] = second;
    state->r[1] = third;
    ram[ram_index(second)] = (uint8_t)state->r[5];
    ram[ram_index(first)] = (uint8_t)state->r[4];
    ram[ram_index(third)] = (uint8_t)state->r[6];
    state->pc = state->pr;
}
