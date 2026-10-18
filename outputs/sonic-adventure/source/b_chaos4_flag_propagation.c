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
    uint32_t at = ram_index(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static void propagate_flag(uint8_t *ram, uint32_t node, uint32_t enabled)
{
    while (node != 0u) {
        uint32_t flags = read32(ram, node);
        write32(ram, node, enabled == 0u ? flags & ~8u : flags | 8u);
        uint32_t child = read32(ram, node + 44u);
        if (child != 0u)
            propagate_flag(ram, child, enabled);
        node = read32(ram, node + 48u);
    }
}

SA_EXPORT void sa_b_chaos4_propagate_flag_0c9038c2(
    uint8_t *ram, struct sa_cpu_state *state)
{
    propagate_flag(ram, state->r[4], state->r[5]);
    state->pc = state->pr;
}
