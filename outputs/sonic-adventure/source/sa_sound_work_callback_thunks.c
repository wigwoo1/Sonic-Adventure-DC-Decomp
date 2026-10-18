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

static void tail_call(uint8_t *ram, struct sa_cpu_state *state, uint32_t slot)
{
    const uint32_t callback = read32(ram, slot);
    const uint32_t object = state->r[4];
    const uint32_t stack = state->r[15];

    /* The delay slot restores R15; the saved R4 word remains in RAM. */
    write32(ram, stack - 4u, object);
    state->r[3] = callback;
    state->r[4] = read32(ram, object + 4u);
    state->pc = callback;
}

SA_EXPORT void sa_sound_work_callback_thunk_8c65bcb4(uint8_t *ram, struct sa_cpu_state *state)
{ tail_call(ram, state, 0x8c65bd1cu); }

SA_EXPORT void sa_sound_work_callback_thunk_8c65bcc0(uint8_t *ram, struct sa_cpu_state *state)
{ tail_call(ram, state, 0x8c65bd20u); }

SA_EXPORT void sa_sound_work_callback_thunk_8c65bccc(uint8_t *ram, struct sa_cpu_state *state)
{ tail_call(ram, state, 0x8c65bd24u); }

SA_EXPORT void sa_sound_work_callback_thunk_8c65bcd8(uint8_t *ram, struct sa_cpu_state *state)
{ tail_call(ram, state, 0x8c65bd28u); }

