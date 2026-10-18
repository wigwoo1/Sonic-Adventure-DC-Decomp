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

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

SA_EXPORT void sa_input_device_record_set_pair_8c65f4ca(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t pointer_address = read32(ram, 0x8c65f530u);
    const uint32_t field_offset = read32(ram, 0x8c65f522u) & 0xffffu;
    const uint32_t record_address = read32(ram, pointer_address);

    state->r[5] = pointer_address;
    state->r[0] = field_offset + 4u;
    state->r[3] = record_address;
    write32(ram, record_address + field_offset, state->r[4]);
    write32(ram, record_address + field_offset + 4u, state->r[4]);
    state->pc = state->pr;
}
