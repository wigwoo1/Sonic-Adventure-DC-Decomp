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
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_read_relative_table_word_8c10d428(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t table_base = read32(ram, 0x8c5fbb14u);
    state->r[3] = 0x8c5fbb14u;
    state->r[4] = (state->r[4] << 2) + table_base + 0xecu;
    state->r[2] = table_base + 0xecu;
    state->r[0] = table_base;
    state->r[1] = read32(ram, state->r[4]);
    write32(ram, state->r[5], state->r[1]);
    state->r[0] = 0u;
    state->pc = state->pr;
}
