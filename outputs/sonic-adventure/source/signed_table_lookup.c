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

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = address & 0x00ffffffu;
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

SA_EXPORT void sa_signed_table_byte_8c049a5a(const uint8_t *ram,
                                              struct sa_cpu_state *state)
{
    uint32_t table = read32(ram, 0x8c049ad8u);
    uint32_t address = (table + state->r[4]) & 0x00ffffffu;
    state->r[0] = (uint32_t)(int32_t)(int8_t)ram[address];
    state->pc = state->pr;
}
