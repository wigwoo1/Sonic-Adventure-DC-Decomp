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

/* Relocate the three pointer fields used by each 0x34-byte resource record. */
SA_EXPORT void sa_resource_record_header_relocate_8c09f708(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t base = state->r[4];
    const uint32_t end = state->r[6];
    uint32_t cursor = state->r[5];
    state->r[7] = cursor;

    while (cursor < end) {
        const uint32_t fields[] = {4u, 0x2cu, 0x30u};
        for (unsigned i = 0; i < 3; ++i) {
            const uint32_t address = cursor + fields[i];
            const uint32_t value = read32(ram, address);
            if (value != 0u)
                write32(ram, address, value + base);
        }
        state->r[2] = read32(ram, cursor + 0x2cu);
        state->r[3] = read32(ram, cursor + 0x30u);
        cursor += 0x34u;
        state->r[7] = cursor;
    }

    state->sr = (state->sr & ~1u) | (cursor >= end);
    state->pc = state->pr;
}
