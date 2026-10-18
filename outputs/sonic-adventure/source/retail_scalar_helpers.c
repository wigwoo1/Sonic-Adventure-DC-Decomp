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
    return address & 0x00ffffffu;
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_address_minus_30_8c5ff396(uint8_t *ram,
                                            struct sa_cpu_state *state)
{
    (void)ram;
    state->r[0] = state->r[4] - 0x30u;
    state->pc = state->pr;
}

SA_EXPORT void sa_subtract_arguments_8c602618(uint8_t *ram,
                                               struct sa_cpu_state *state)
{
    (void)ram;
    state->r[0] = state->r[5] - state->r[4];
    state->pc = state->pr;
}

SA_EXPORT void sa_zero_field_plus4_8c09de34(uint8_t *ram,
                                            struct sa_cpu_state *state)
{
    state->r[3] = 0;
    write32(ram, state->r[4] + 4u, 0);
    state->pc = state->pr;
}
