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

static uint8_t read8(const uint8_t *ram, uint32_t address)
{
    return ram[ram_index(address)];
}

SA_EXPORT void sa_slot_list_available_8c05f458(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t table =
        (uint32_t)ram[0x5f500u] |
        ((uint32_t)ram[0x5f501u] << 8) |
        ((uint32_t)ram[0x5f502u] << 16) |
        ((uint32_t)ram[0x5f503u] << 24);
    uint32_t cursor = state->r[4];
    uint32_t available = 0u;

    state->r[1] = 0xffffbfffu;
    state->r[3] = 0xffffffffu;
    state->r[6] = table;
    state->r[7] = 0x4000u;
    state->sr &= ~1u;
    for (;;) {
        const uint32_t entry = read16(ram, cursor);
        state->r[5] = entry;
        const uint32_t slot = (entry & 0x4000u) != 0u
            ? (entry & 0xffffbfffu) : entry;
        const uint8_t occupied = read8(ram, table + slot);
        if (((entry & 0x4000u) == 0u && occupied == 0u) ||
            ((entry & 0x4000u) != 0u && occupied != 0u)) {
            available = 1u;
            if ((entry & 0x4000u) == 0u) {
                state->r[3] = 0u;
                state->r[4] = cursor;
                state->sr |= 1u;
            } else {
                state->r[3] = 1u;
                state->r[4] = cursor;
                state->sr &= ~1u;
            }
            break;
        }
        cursor += 2u;
        state->r[4] = cursor;
        if ((int16_t)read16(ram, cursor) < 0)
            break;
    }

    state->r[0] = available;
    state->pc = state->pr;
}
