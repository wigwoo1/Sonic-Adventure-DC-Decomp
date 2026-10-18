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

static uint16_t read16(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_index(address);
    return (uint16_t)((uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8));
}

SA_EXPORT void sa_task_count_predicate_8c021868(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t count_address = read32(ram, state->r[6] + 4u);
    const uint32_t threshold = (uint32_t)read16(ram, count_address + 2u) - 3u;
    state->r[5] = threshold;
    state->r[0] = (state->r[4] != 0u && (int32_t)state->r[4] < (int32_t)threshold)
                      ? 0u : 1u;
    state->sr = (state->sr & ~1u) | state->r[0];
    state->pc = state->pr;
}
