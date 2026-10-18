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

/* Retail routine at 0x8c65bd2c clears the audio work buffer and returns 1. */
SA_EXPORT void sa_audio_work_buffer_initialize_8c65bd2c(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t original_stack = state->r[15];
    const uint32_t original_pr = state->pr;
    const uint32_t buffer = 0x8c8a3d90u;
    const uint32_t length = 0x0c80u;

    /* The routine saves PR on the stack, then restores R15 after the fill call. */
    write32(ram, original_stack - 4u, original_pr);
    volatile uint8_t *buffer_bytes = &ram[ram_offset(buffer)];
    for (uint32_t index = 0; index < length; ++index)
        buffer_bytes[index] = 0u;

    state->r[0] = 1u;
    state->r[3] = 0u;
    state->r[4] = buffer;
    state->r[5] = 0u;
    state->r[6] = length;
    state->r[7] = length;
    state->sr |= 1u;
    state->pc = original_pr;
}

