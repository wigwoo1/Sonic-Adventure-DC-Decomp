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

static uint16_t read16(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint16_t)((uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8));
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

/* Retail request-position update helper at 0x8c600e7a. */
SA_EXPORT void sa_stream_request_position_update_8c600e7a(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t request = state->r[4];
    const uint32_t delta = state->r[5];
    const uint32_t origin = state->r[6];

    if (request == 0u) {
        state->r[0] = 0xfffffff6u;
        state->sr |= 1u;
        state->pc = state->pr;
        return;
    }

    const int32_t status = (int16_t)read16(ram, request + 0x4cu);
    state->r[3] = (uint32_t)status;
    if (status == 0) {
        state->r[0] = 0xfffffff6u;
        state->sr |= 1u;
        state->pc = state->pr;
        return;
    }

    const uint32_t maximum = read32(ram, request + 0x10u);
    state->r[7] = maximum;

    uint32_t updated;
    if (origin == 0u) {
        updated = delta;
    } else if (origin == 1u) {
        updated = read32(ram, request + 0x14u) + delta;
    } else if (origin == 2u) {
        updated = maximum + delta;
    } else {
        state->r[0] = 0xfffffff0u;
        state->sr &= ~1u;
        state->pc = state->pr;
        return;
    }

    state->r[6] = updated;
    const int32_t signed_updated = (int32_t)updated;
    const int32_t signed_maximum = (int32_t)maximum;
    if (signed_updated < 0 || signed_updated > signed_maximum) {
        state->r[0] = 0xffffffefu;
        state->sr = (state->sr & ~1u) |
                    (uint32_t)(signed_updated >= 0 && signed_updated > signed_maximum);
    } else {
        write32(ram, request + 0x14u, updated);
        state->r[0] = 0u;
        state->sr &= ~1u;
    }
    state->pc = state->pr;
}

