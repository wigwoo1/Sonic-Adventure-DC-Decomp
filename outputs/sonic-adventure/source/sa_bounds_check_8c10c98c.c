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

static uint32_t ram_offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static int less_signed(uint32_t left, uint32_t right)
{
    return (int32_t)left < (int32_t)right;
}

/* Retail entry 0x8c10c98c: validate an item index, then an item-local bound. */
SA_EXPORT void sa_bounds_check_8c10c98c(
    const uint8_t *ram,
    struct sa_cpu_state *state)
{
    const uint32_t index = state->r[4];
    const uint32_t item = state->r[5];
    const uint32_t maximum = read32(ram, 0x8c10ca54u);
    uint32_t bound;
    int valid;

    state->r[3] = maximum;
    valid = less_signed(index, maximum);
    if (valid) {
        const uint32_t table = read32(ram, 0x8c10ca58u);
        const uint32_t entry = read32(ram, table + index * 4u);
        bound = read32(ram, entry + 0x0cu);
        state->r[4] = entry;
        state->r[3] = bound;
        valid = less_signed(item, bound);
    }

    state->r[0] = valid ? 0u : 0xfffffffdu;
    state->sr = (state->sr & ~1u) | (valid ? 0u : 1u);
    state->pc = state->pr;
}
