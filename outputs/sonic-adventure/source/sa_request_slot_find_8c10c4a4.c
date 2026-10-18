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
    const uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

/* Retail entry 0x8c10c4a4: return the first free 40-byte request record. */
SA_EXPORT void sa_request_slot_find_8c10c4a4(
    const uint8_t *ram,
    struct sa_cpu_state *state)
{
    const uint32_t table = read32(ram, 0x8c10c50cu);
    const uint32_t end = table + read32(ram, 0x8c10c500u);
    uint32_t slot = table;

    while (slot < end && ram[ram_offset(slot)] != 0u)
        slot += 0x28u;

    state->r[7] = end;
    if (slot < end) {
        state->r[0] = slot;
        state->r[3] = 0u;
        state->r[4] = slot;
        state->r[5] = slot;
        state->r[6] = slot;
    } else {
        state->r[0] = 0u;
        state->r[3] = (uint32_t)(int32_t)(int8_t)ram[ram_offset(end - 0x28u)];
        state->r[4] = end;
        state->r[5] = end;
        state->r[6] = 0u;
    }

    state->sr = (state->sr & ~1u) | 1u;
    state->pc = state->pr;
}
