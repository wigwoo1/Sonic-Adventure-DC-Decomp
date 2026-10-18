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

SA_EXPORT void sa_b_chaos0_state_word_mode_0c903400(
    uint8_t *ram, struct sa_cpu_state *state)
{
    uint32_t global = read32(ram, 0x0c903500u);
    uint32_t nested = read32(ram, global + 4u);
    uint32_t object = read32(ram, nested + 0x10u);
    uint32_t field_address = object + 0x10u;
    uint32_t parameter = state->r[4];

    if ((parameter & 1u) == 0u) {
        write32(ram, field_address, read32(ram, 0x0c90350cu));
    } else {
        uint32_t mask = read32(ram, 0x0c903504u);
        uint32_t set_bits = read32(ram, 0x0c903508u);
        uint32_t value = read32(ram, field_address);
        write32(ram, field_address, (value & mask) | set_bits);
    }
    state->sr = (state->sr & ~1u) | (uint32_t)((parameter & 1u) == 0u);
    state->pc = state->pr;
}
