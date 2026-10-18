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

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

/* Retail initializer at 0x8c65b434 clears the sound driver's 256-byte buffer. */
SA_EXPORT void sa_audio_buffer_initialize_8c65b434(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t original_stack = state->r[15];
    const uint32_t original_pr = state->pr;
    const uint32_t buffer = 0x8c8a3810u;

    /* Match the caller's saved-PR stack write, retained after the stack is restored. */
    write32(ram, original_stack - 4u, original_pr);
    for (uint32_t index = 0; index < 0x100u; ++index)
        ram[ram_offset(buffer + index)] = 0u;
    write32(ram, 0x8c67c1e8u, 0u);
    write32(ram, 0x8c67c1ecu, 0u);

    state->r[0] = buffer;
    state->r[2] = 0x8c67c1e8u;
    state->r[3] = 0x8c67c1ecu;
    state->r[4] = 0u;
    state->r[5] = 0u;
    state->r[6] = 0x100u;
    state->r[7] = 0x100u;
    state->sr |= 1u;
    state->pc = original_pr;
}

