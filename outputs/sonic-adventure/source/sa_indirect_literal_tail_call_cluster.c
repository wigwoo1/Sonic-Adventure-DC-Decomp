#include <stdint.h>

/* Retail entry points implemented by this parameterized body:
 * sa_indirect_literal_tail_call_8c10da3c
 * sa_indirect_literal_tail_call_8c10da50
 * sa_indirect_literal_tail_call_8c10da64
 * sa_indirect_literal_tail_call_8c10da78
 */

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
    const uint32_t at = (address & 0x1fffffffu) - 0x0c000000u;
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

SA_EXPORT void sa_indirect_literal_tail_call_cluster(
    const uint8_t *ram,
    struct sa_cpu_state *state,
    uint32_t argument_literal,
    uint32_t callback_slot_literal)
{
    state->r[7] = read32(ram, argument_literal);
    state->r[0] = read32(ram, callback_slot_literal);
    state->r[0] = read32(ram, state->r[0]);
    state->pc = state->r[0];
}
