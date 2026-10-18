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

static int32_t read_signed_byte(const uint8_t *ram, uint32_t address)
{
    return (int32_t)(int8_t)ram[ram_index(address)];
}

static void get_status_byte(const uint8_t *ram, struct sa_cpu_state *state,
                            uint32_t byte_offset)
{
    const uint32_t global_slot = read32(ram, 0x0c90b108u);
    const uint32_t object = read32(ram, global_slot);
    int32_t result = 0;

    state->r[3] = global_slot;
    state->r[5] = object;
    if (object == 0u) {
        state->r[4] = 0u;
        state->sr |= 1u;
    } else {
        const uint32_t string = read32(ram, object + 0x24u);
        result = read_signed_byte(ram, string + byte_offset);
        state->r[5] = string;
        state->r[4] = (uint32_t)result;
        state->sr &= ~1u;
    }
    state->r[0] = (uint32_t)result;
    state->pc = state->pr;
}

SA_EXPORT void sa_minicart_get_status_byte_0c90b0d0(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    get_status_byte(ram, state, 0u);
}

SA_EXPORT void sa_minicart_get_status_byte_plus3_0c90b0e2(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    get_status_byte(ram, state, 3u);
}
