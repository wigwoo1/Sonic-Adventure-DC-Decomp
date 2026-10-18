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

static uint8_t read8(const uint8_t *ram, uint32_t address)
{
    return ram[ram_index(address)];
}

SA_EXPORT void sa_signed_byte_table_lookup_8c0d2032(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t table = read32(ram, 0x8c0d2060u);
    const int32_t input = (int32_t)(int8_t)read8(ram, state->r[4]);
    const uint32_t result_address = table + (uint32_t)input;
    state->r[1] = table;
    state->r[0] = (uint32_t)(int32_t)(int8_t)read8(ram, result_address);
    state->pc = state->pr;
}
