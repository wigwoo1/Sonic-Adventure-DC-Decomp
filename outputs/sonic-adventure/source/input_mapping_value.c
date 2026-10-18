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

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static uint32_t read16(const uint8_t *ram, uint32_t address)
{
    uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8);
}

static uint32_t population_count16(uint32_t value)
{
    uint32_t count = 0;
    value &= 0xffffu;
    while (value != 0) {
        count += value & 1u;
        value >>= 1;
    }
    return count;
}

static uint32_t input_mapping_value(const uint8_t *ram, int32_t row,
                                    int32_t code, int32_t bank)
{
    uint32_t result = 0;
    if (code == 0) {
        code = 100;
    } else if (code < 15) {
        int32_t shift = code - 15;
        uint32_t mask = read32(ram, 0x8c0a6b00u);
        if (shift < 0) {
            uint32_t right = ((~(uint32_t)shift & 0x1fu) + 1u);
            mask >>= right;
        } else {
            mask <<= (uint32_t)shift & 0x1fu;
        }

        uint32_t table = read32(ram, 0x8c0a6b04u);
        int32_t limit = row + 1;
        for (int32_t i = 0; i < limit; ++i) {
            uint32_t entry = (uint32_t)(int32_t)(int16_t)
                read16(ram, table + (uint32_t)i * 2u);
            if (i == row)
                entry &= mask;
            result += population_count16(entry);
        }
        result = result - 1u + (uint32_t)bank * 0x20u;
        return result;
    } else if ((code < 0x23 || code > 0x26) && code != 0x2a) {
        return 0;
    } else if (code == 0x2a) {
        return (uint32_t)bank + 0x6au;
    } else {
        code += 0x3d;
    }
    return (uint32_t)code + (uint32_t)bank * 5u;
}

SA_EXPORT void sa_input_mapping_value_8c0a69a8(uint8_t *ram,
                                                struct sa_cpu_state *state)
{
    state->r[0] = input_mapping_value(ram, (int32_t)state->r[4],
                                      (int32_t)state->r[5],
                                      (int32_t)state->r[6]);
    state->pc = state->pr;
}
