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
    uint32_t at = ram_index(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_b_chaos4_template_copy_0c900348(
    uint8_t *ram, struct sa_cpu_state *state)
{
    static const uint8_t source_indices[21] = {
        0, 1, 2, 9, 10, 11, 0x1b, 0x1c, 0x1d, 0x12, 0x13, 0x14,
        0x2d, 0x2e, 0x2f, 0x24, 0x25, 0x26, 0x36, 0x37, 0x38
    };
    static const uint16_t destination_offsets[21] = {
        0x08, 0x0c, 0x10, 0x3c, 0x40, 0x44, 0xa4, 0xa8, 0xac,
        0x70, 0x74, 0x78, 0x10c, 0x110, 0x114, 0xd8, 0xdc, 0xe0,
        0x140, 0x144, 0x148
    };
    uint32_t parameter = state->r[4];
    uint32_t destination = read32(ram, parameter + 0x0cu);
    uint32_t source = read32(ram, 0x0c900404u);

    for (uint32_t i = 0; i < 21u; ++i) {
        uint32_t value = read32(ram, source + (uint32_t)source_indices[i] * 4u);
        write32(ram, destination + destination_offsets[i], value);
    }
    state->pc = state->pr;
}
