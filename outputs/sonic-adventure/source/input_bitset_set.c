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

typedef void (*sa_input_dispatch)(uint8_t *, struct sa_cpu_state *,
                                  uint32_t, void *);

static uint32_t ram_offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void set_input_bit(uint8_t *ram, uint32_t base, uint32_t bit_index)
{
    uint32_t adjusted = bit_index;
    int32_t signed_index = (int32_t)bit_index;
    if (signed_index < 0)
        adjusted += 7u;
    uint32_t remainder;
    if (signed_index < 0) {
        uint32_t magnitude_remainder = (~bit_index + 1u) & 7u;
        remainder = ~magnitude_remainder + 1u;
    } else {
        remainder = bit_index & 7u;
    }
    uint32_t mask;
    if ((int32_t)remainder < 0) {
        uint32_t shift = ((~remainder & 0x1fu) + 1u);
        mask = 1u >> shift;
    } else {
        mask = 1u << (remainder & 0x1fu);
    }
    int32_t byte_offset = (int32_t)adjusted >> 3;
    uint32_t target = base + 0x240u + (uint32_t)byte_offset;
    uint32_t at = ram_offset(target);
    ram[at] |= (uint8_t)mask;
}

SA_EXPORT void sa_input_bitset_set_8c0a6918(uint8_t *ram,
                                              struct sa_cpu_state *state,
                                              sa_input_dispatch dispatch,
                                              void *context)
{
    set_input_bit(ram, state->r[4], state->r[5]);
    uint32_t mode_address = read32(ram, 0x8c0a69e0u);
    if (read32(ram, mode_address) == 5u) {
        uint32_t callback = read32(ram, 0x8c0a69e4u);
        state->pc = callback;
        if (dispatch != 0)
            dispatch(ram, state, callback, context);
    }
    state->pc = state->pr;
}
