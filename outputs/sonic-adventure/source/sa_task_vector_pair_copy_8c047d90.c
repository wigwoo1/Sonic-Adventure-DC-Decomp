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

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_vector_copy_to_offset_0x68_8c047d90(
    uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[3] = read32(ram, state->r[4] + 0x20u);
    state->r[0] = state->r[5];
    state->r[0] += 0x68u;
    state->r[2] = read32(ram, state->r[4] + 0x24u);
    write32(ram, state->r[0], state->r[3]);
    write32(ram, state->r[0] + 4u, state->r[2]);
    state->r[3] = read32(ram, state->r[4] + 0x28u);
    write32(ram, state->r[0] + 8u, state->r[3]);
    state->pc = state->pr;
}

SA_EXPORT void sa_vector_copy_from_offset_0x68_8c047da2(
    uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[1] = state->r[5];
    state->r[1] += 0x68u;
    state->r[3] = read32(ram, state->r[1]);
    state->r[2] = read32(ram, state->r[1] + 4u);
    write32(ram, state->r[4] + 0x20u, state->r[3]);
    write32(ram, state->r[4] + 0x24u, state->r[2]);
    state->r[3] = read32(ram, state->r[1] + 8u);
    write32(ram, state->r[4] + 0x28u, state->r[3]);
    state->pc = state->pr;
}
