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

static uint32_t input_bit_test(const uint8_t *ram, uint32_t address,
                               uint32_t bit_index)
{
    int32_t signed_index = (int32_t)bit_index;
    uint32_t adjusted_index = bit_index;
    if (signed_index < 0)
        adjusted_index += 7u;
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
    int32_t byte_offset = (int32_t)adjusted_index >> 3;
    uint32_t byte_address = address + 0x240u + (uint32_t)byte_offset;
    int32_t byte_value = (int8_t)ram[ram_offset(byte_address)];
    return ((uint32_t)byte_value & mask) != 0;
}

static uint32_t input_bit_count(const uint8_t *ram, uint32_t address)
{
    uint32_t count = 0;
    uint32_t first = ram_offset(address + 0x240u);
    for (uint32_t i = 0; i < 0x11u; ++i) {
        uint32_t value = ram[first + i];
        while (value != 0) {
            count += value & 1u;
            value >>= 1;
        }
    }
    return count;
}

static uint32_t input_index_to_code(uint32_t input)
{
    int32_t value = (int32_t)input;
    if (value > 4)
        --value;
    if (value > 1)
        --value;
    return (uint32_t)value + 0x6fu;
}

SA_EXPORT void sa_input_bitset_test_8c0a68d8(uint8_t *ram,
                                             struct sa_cpu_state *state)
{
    state->r[0] = input_bit_test(ram, state->r[4], state->r[5]);
    state->pc = state->pr;
}

SA_EXPORT void sa_input_bitset_count_8c0a6960(uint8_t *ram,
                                               struct sa_cpu_state *state)
{
    state->r[0] = input_bit_count(ram, state->r[4]);
    state->pc = state->pr;
}

SA_EXPORT void sa_input_index_to_code_8c0a6992(uint8_t *ram,
                                               struct sa_cpu_state *state)
{
    (void)ram;
    state->r[0] = input_index_to_code(state->r[4]);
    state->pc = state->pr;
}
