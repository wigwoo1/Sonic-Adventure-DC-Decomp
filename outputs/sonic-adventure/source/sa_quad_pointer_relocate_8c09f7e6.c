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

/* Relocate four nonzero pointers in the first half of each 32-byte group. */
SA_EXPORT void sa_quad_pointer_relocate_8c09f7e6(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t base = state->r[4];
    const uint32_t stack = state->r[15];
    write32(ram, stack - 4u, state->r[14]);
    write32(ram, stack - 8u, state->r[13]);
    uint32_t cursor = state->r[5];
    int32_t remaining = (int32_t)state->r[6];

    while (remaining > 0) {
        for (uint32_t offset = 0u; offset < 16u; offset += 4u) {
            const uint32_t address = cursor + offset;
            const uint32_t value = read32(ram, address);
            if (value != 0u)
                write32(ram, address, value + base);
        }
        state->r[1] = 12u;
        state->r[2] = cursor + 12u;
        state->r[3] = read32(ram, cursor + 12u);
        state->r[7] = 4u;
        remaining -= 8;
        cursor += 32u;
    }

    state->r[0] = cursor;
    state->r[5] = cursor;
    state->r[6] = (uint32_t)remaining;
    state->sr &= ~1u;
    state->pc = state->pr;
}
