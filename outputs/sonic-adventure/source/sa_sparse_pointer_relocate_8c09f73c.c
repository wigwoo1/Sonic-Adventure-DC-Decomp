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

/* Relocate one pointer word per 8-byte pair while count remains positive. */
SA_EXPORT void sa_sparse_pointer_relocate_8c09f73c(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t base = state->r[4];
    uint32_t cursor = state->r[5];
    int32_t remaining = (int32_t)state->r[6];

    while (remaining > 0) {
        const uint32_t value = read32(ram, cursor);
        state->r[2] = value;
        if (value != 0u) {
            write32(ram, cursor, value + base);
            state->r[3] = value + base;
            state->r[7] = cursor;
        }
        remaining -= 2;
        cursor += 8u;
    }

    state->r[0] = cursor;
    state->r[5] = cursor;
    state->r[6] = (uint32_t)remaining;
    state->sr &= ~1u;
    state->pc = state->pr;
}
