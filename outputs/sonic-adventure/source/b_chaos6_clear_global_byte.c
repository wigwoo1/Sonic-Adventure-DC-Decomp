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

SA_EXPORT void sa_b_chaos6_clear_global_byte_0c903bc6(
    uint8_t *ram, struct sa_cpu_state *state)
{
    uint32_t destination = read32(ram, 0x0c903c38u);
    state->r[1] = destination;
    state->r[2] = 0;
    ram[ram_index(destination)] = (uint8_t)state->r[2];
    state->pc = state->pr;
}
