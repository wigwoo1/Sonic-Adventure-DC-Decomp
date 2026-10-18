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

static uint16_t read16(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_index(address);
    return (uint16_t)((uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8));
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_index(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write16(uint8_t *ram, uint32_t address, uint16_t value)
{
    uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_stg09_offset_short_pairs_0c90bc18(
    uint8_t *ram, struct sa_cpu_state *state)
{
    uint32_t destination = state->r[4];
    uint32_t source = state->r[5];
    uint32_t dx = state->r[6] & 0x0fffu;
    uint32_t dy = state->r[7] & 0x0fffu;
    uint32_t stack = state->r[15];
    uint32_t saved_r14 = state->r[14];
    uint32_t count = read32(ram, stack);

    while (count != 0u) {
        uint16_t x = (uint16_t)(read16(ram, source) + dx);
        uint16_t y = (uint16_t)(read16(ram, source + 2u) + dy);
        write16(ram, destination, x);
        write16(ram, destination + 2u, y);
        source += 4u;
        destination += 4u;
        --count;
    }
    write32(ram, stack - 4u, saved_r14);
    write32(ram, stack, UINT32_MAX);
    state->pc = state->pr;
}
