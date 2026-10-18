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
    const uint32_t at = ram_index(address);
    return (uint16_t)((uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8));
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_index(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write16(uint8_t *ram, uint32_t address, uint16_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_b_chaos6_countdown_accumulator_0c9062f0(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t work = read32(ram, state->r[4] + 0x20u);
    const uint16_t stored_count = (uint16_t)(read16(ram, work + 0x10u) - 1u);
    const int32_t count = stored_count < 0x8000u
        ? (int32_t)stored_count : (int32_t)stored_count - 0x10000;

    state->r[4] = work;
    write16(ram, work + 0x10u, stored_count);
    state->r[0] = (uint32_t)count;
    state->sr = (state->sr & ~1u) | (uint32_t)(count > 0);
    if (count <= 0) {
        const uint32_t accumulated = read32(ram, work + 0x18u) + 0xb6u;
        state->r[3] = 0xb6u;
        state->r[0] = 3u;
        state->r[2] = accumulated;
        write16(ram, work + 0x10u, 3u);
        write32(ram, work + 0x18u, accumulated);
    }
    state->pc = state->pr;
}
