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

SA_EXPORT void sa_b_chaos0_fill_global_bytes_0c900340(
    uint8_t *ram, struct sa_cpu_state *state)
{
    static const uint8_t offsets[8] = {3, 4, 5, 0x0f, 6, 7, 8, 0x12};
    uint32_t destination = read32(ram, 0x0c9004c0u);
    uint8_t value = (uint8_t)state->r[4];
    uint32_t base = ram_index(destination);

    for (uint32_t i = 0; i < 8u; ++i) {
        ram[base + offsets[i]] = value;
    }
    state->pc = state->pr;
}
