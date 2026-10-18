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

SA_EXPORT void sa_b_chaos2_write_state_bytes_0c9004b2(
    uint8_t *ram, struct sa_cpu_state *state)
{
    uint32_t destination = read32(ram, 0x0c9004c4u);
    uint8_t value = (uint8_t)state->r[4];
    uint32_t at = ram_index(destination + 2u);

    ram[at] = value;
    ram[at + 1u] = value;
    ram[at + 2u] = value;
    ram[at + 3u] = value;
    state->pc = state->pr;
}
