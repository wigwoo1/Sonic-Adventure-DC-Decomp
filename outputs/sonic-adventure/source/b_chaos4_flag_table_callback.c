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

SA_EXPORT void sa_b_chaos4_flag_table_callback_0c900280(
    uint8_t *ram, struct sa_cpu_state *state)
{
    uint32_t context = state->r[4];
    uint32_t context_data = read32(ram, context + 0x38u);
    uint32_t object = read32(ram, context_data + 0x0cu);
    uint32_t table = read32(ram, 0x0c900400u) + state->r[5] * 7u;

    for (uint32_t i = 0; i < 7u; ++i) {
        uint32_t record_address = object + i * 0x34u;
        uint32_t flags = read32(ram, record_address);
        if (ram[ram_index(table + i)] == 0u) {
            flags |= 0x10u;
        } else {
            flags &= ~0x10u;
        }
        write32(ram, record_address, flags);
    }
    state->pc = state->pr;
}
