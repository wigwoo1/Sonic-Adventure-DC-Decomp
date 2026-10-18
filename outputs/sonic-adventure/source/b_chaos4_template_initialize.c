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

SA_EXPORT void sa_b_chaos4_template_initialize_0c900348(
    uint8_t *ram, struct sa_cpu_state *state)
{
    static const uint8_t source_groups[7] = {0, 9, 27, 18, 45, 36, 54};
    static const uint16_t destination_offsets[7] = {
        0x08, 0x3c, 0xa4, 0x70, 0x10c, 0xd8, 0x140
    };
    uint32_t context = state->r[4];
    uint32_t object = read32(ram, context + 0x0cu);
    uint32_t template = read32(ram, 0x0c900404u);

    for (uint32_t group = 0; group < 7u; ++group) {
        for (uint32_t word = 0; word < 3u; ++word) {
            uint32_t value = read32(ram, template +
                                    ((uint32_t)source_groups[group] + word) * 4u);
            write32(ram, object + destination_offsets[group] + word * 4u, value);
        }
    }
    state->pc = state->pr;
}
