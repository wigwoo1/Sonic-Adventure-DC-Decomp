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

static uint32_t read_signed_byte(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_index(address);
    return (uint32_t)(int32_t)(int8_t)ram[at];
}

static void indirect_byte_get(uint8_t *ram, struct sa_cpu_state *state,
                              uint32_t field_offset)
{
    state->r[3] = 0x8c11204cu;
    state->r[0] = read32(ram, state->r[3]);
    state->r[0] = read_signed_byte(ram, state->r[0] + field_offset);
    state->pc = state->pr;
}

SA_EXPORT void sa_indirect_byte_get_9_8c01b406(
    uint8_t *ram, struct sa_cpu_state *state)
{
    indirect_byte_get(ram, state, 9u);
}

SA_EXPORT void sa_indirect_byte_get_6_8c01b40e(
    uint8_t *ram, struct sa_cpu_state *state)
{
    indirect_byte_get(ram, state, 6u);
}
