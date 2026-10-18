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

/* Retail entry 0x8c10c4ca: claim and clear the first free request record. */
SA_EXPORT void sa_request_slot_acquire_8c10c4ca(
    uint8_t *ram,
    struct sa_cpu_state *state)
{
    const uint32_t table = read32(ram, 0x8c10c50cu);
    const uint32_t end = table + read32(ram, 0x8c10c500u);
    const uint32_t saved_pr = state->pr;
    const uint32_t stack = state->r[15] - 4u;
    uint32_t slot = table;

    ram[ram_offset(stack)] = (uint8_t)saved_pr;
    ram[ram_offset(stack + 1u)] = (uint8_t)(saved_pr >> 8);
    ram[ram_offset(stack + 2u)] = (uint8_t)(saved_pr >> 16);
    ram[ram_offset(stack + 3u)] = (uint8_t)(saved_pr >> 24);

    while (slot < end && ram[ram_offset(slot)] != 0u)
        slot += 0x28u;

    state->r[7] = end;
    if (slot < end) {
        ram[ram_offset(slot + 1u)] = 1u;
        ram[ram_offset(slot + 2u)] = 0u;
        for (uint32_t offset = 4u; offset <= 8u; offset += 4u) {
            const uint32_t at = ram_offset(slot + offset);
            ram[at] = 0u;
            ram[at + 1u] = 0u;
            ram[at + 2u] = 0u;
            ram[at + 3u] = 0u;
        }
        for (uint32_t offset = 0x1cu; offset <= 0x24u; offset += 4u) {
            const uint32_t at = ram_offset(slot + offset);
            ram[at] = 0u;
            ram[at + 1u] = 0u;
            ram[at + 2u] = 0u;
            ram[at + 3u] = 0u;
        }
        ram[ram_offset(slot)] = 1u;
        state->r[0] = slot;
        state->r[3] = 0u;
        state->r[4] = slot;
        state->r[5] = 0u;
        state->r[6] = 1u;
        state->sr &= ~1u;
    } else {
        state->r[0] = 0u;
        state->r[3] = (uint32_t)(int32_t)(int8_t)ram[ram_offset(end - 0x28u)];
        state->r[4] = 0u;
        state->r[5] = end;
        state->r[6] = 0u;
        state->sr |= 1u;
    }

    state->pc = state->pr;
}
