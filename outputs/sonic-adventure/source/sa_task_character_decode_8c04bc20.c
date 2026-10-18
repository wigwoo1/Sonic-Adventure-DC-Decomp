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

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_task_character_decode_8c04bc20(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t work = read32(ram, state->r[4] + 0x3cu);
    const uint32_t cursor_field = work + 0x64u;
    state->r[5] = work + 0x5cu;
    state->r[3] = read32(ram, cursor_field);
    if (state->r[3] == 0u) {
        state->r[0] = 0;
        state->sr |= 1u;
        state->pc = state->pr;
        return;
    }

    state->r[4] = state->r[3];
    state->r[4] += 1u;
    write32(ram, cursor_field, state->r[4]);
    state->r[4] -= 1u;
    state->r[4] = (uint32_t)(int32_t)(int8_t)read8(ram, state->r[4]);
    const int32_t character = (int32_t)state->r[4];
    if (character == 0) {
        state->r[2] = 0;
        write32(ram, cursor_field, 0);
        state->r[0] = 0;
        state->sr |= 1u;
    } else {
        if (character > 0x40) {
            state->r[1] = 0x5au;
        }
        state->r[2] = 0x41u;
        if (character >= 0x41 && character <= 0x5a) {
            state->r[1] = 0x5au;
            state->r[0] = (uint32_t)(character - 0x41);
            state->sr &= ~1u;
        } else {
            state->r[3] = 0x61u;
            if (character > 0x60) {
                state->r[1] = 0x7au;
            }
            if (character >= 0x61 && character <= 0x7a) {
                state->r[0] = (uint32_t)(character - 0x47);
                state->sr &= ~1u;
            } else if (character == 0x30) {
                state->r[0] = 0xffffffffu;
                state->sr |= 1u;
            } else {
                state->r[0] = (uint32_t)character;
                state->sr &= ~1u;
            }
        }
    }
    state->pc = state->pr;
}
