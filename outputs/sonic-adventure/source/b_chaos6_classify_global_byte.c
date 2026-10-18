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

SA_EXPORT void sa_b_chaos6_classify_global_byte_0c902f18(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t source = read32(ram, 0x0c903014u);
    const uint32_t byte_address = read32(ram, source);
    const uint32_t value = (uint32_t)(int32_t)(int8_t)ram[ram_index(byte_address)];
    const uint32_t return_pc = state->pr;

    state->r[0] = value;
    state->r[3] = source;
    state->r[4] = value;
    if (value == 1u || value == 2u || value == 0x13u || value == 3u) {
        state->r[0] = 0u;
        state->sr |= 1u;
    } else {
        state->r[0] = 1u;
        state->sr &= ~1u;
    }
    state->pc = return_pc;
}
