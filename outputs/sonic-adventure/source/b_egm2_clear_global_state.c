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
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

SA_EXPORT void sa_b_egm2_clear_global_state_0c906b80(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t word_address = read32(ram, 0x0c906c60u);
    const uint32_t byte_address = read32(ram, 0x0c906c64u);
    const uint32_t word_offset = ram_index(word_address);
    const uint32_t byte_offset = ram_index(byte_address);

    state->r[3] = word_address;
    state->r[4] = 0u;
    state->r[2] = byte_address;
    ram[word_offset] = 0u;
    ram[word_offset + 1u] = 0u;
    ram[word_offset + 2u] = 0u;
    ram[word_offset + 3u] = 0u;
    ram[byte_offset] = 0u;
    state->pc = state->pr;
}
