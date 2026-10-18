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

static uint32_t offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write8(uint8_t *ram, uint32_t address, uint8_t value)
{
    uint32_t at = offset(address);
    ram[at] = value;
}

static void store_constant(uint8_t *ram, struct sa_cpu_state *state,
                           uint32_t literal, unsigned pointer_register,
                           unsigned value_register, uint32_t value)
{
    uint32_t target = read32(ram, literal);
    state->r[pointer_register] = target;
    state->r[value_register] = value;
    write8(ram, target, (uint8_t)value);
    state->pc = state->pr;
}

SA_EXPORT void sa_global_constant_setter_8c0509a0(uint8_t *ram,
                                                   struct sa_cpu_state *state)
{ store_constant(ram, state, 0x8c050a20u, 2u, 3u, 1u); }

SA_EXPORT void sa_global_constant_setter_8c0509a8(uint8_t *ram,
                                                   struct sa_cpu_state *state)
{ store_constant(ram, state, 0x8c050a20u, 0u, 1u, 0u); }

SA_EXPORT void sa_global_constant_setter_8c08a07c(uint8_t *ram,
                                                   struct sa_cpu_state *state)
{ store_constant(ram, state, 0x8c08a0f4u, 0u, 3u, 4u); }

SA_EXPORT void sa_global_five_word_initialize_8c608e24(
    uint8_t *ram, struct sa_cpu_state *state)
{
    uint32_t target = read32(ram, 0x8c608e70u);
    state->r[4] = target;
    state->r[5] = 0xffffffffu;
    for (unsigned i = 0; i < 2u; ++i) {
        uint32_t at = offset(target + i * 4u);
        ram[at] = 0xffu;
        ram[at + 1u] = 0xffu;
        ram[at + 2u] = 0xffu;
        ram[at + 3u] = 0xffu;
    }
    state->r[5] = 0u;
    for (unsigned i = 2u; i < 5u; ++i) {
        uint32_t at = offset(target + i * 4u);
        ram[at] = 0u;
        ram[at + 1u] = 0u;
        ram[at + 2u] = 0u;
        ram[at + 3u] = 0u;
    }
    state->pc = state->pr;
}
