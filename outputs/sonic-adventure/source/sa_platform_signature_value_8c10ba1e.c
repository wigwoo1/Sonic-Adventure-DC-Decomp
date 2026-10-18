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

static uint16_t read16(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint16_t)((uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8));
}

/* Retail entry 0x8c10ba1e checks the AGES signature before reading the
 * associated 32-bit platform value. */
SA_EXPORT void sa_platform_signature_value_8c10ba1e(
    const uint8_t *ram,
    struct sa_cpu_state *state)
{
    const uint32_t value_address = read32(ram, 0x8c10ba40u);
    const uint32_t signature = read32(ram, 0x8c10ba38u);
    const uint32_t signature_address = read32(ram, 0x8c10ba44u);
    const int matches = read32(ram, signature_address) == signature;

    state->r[0] = matches ? (uint32_t)(int32_t)(int16_t)read16(ram, value_address) : 0u;
    state->r[2] = signature;
    state->r[3] = matches ? value_address : signature_address;
    state->sr = (state->sr & ~1u) | (matches ? 1u : 0u);
    state->pc = state->pr;
}
