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

static int32_t read_signed16(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_index(address);
    const uint16_t value = (uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8);
    return (int16_t)value;
}

SA_EXPORT void sa_input_device_record_index_8c65f390(
    uint8_t *ram, struct sa_cpu_state *state, uint32_t *macl)
{
    const uint32_t product = state->r[4] * 0x2cu;
    const uint32_t pointer_address = read32(ram, 0x8c65f3d8u);
    const uint32_t table_address = read32(ram, pointer_address);

    state->r[2] = 0x2cu;
    state->r[3] = pointer_address;
    *macl = product;
    state->r[4] = product;
    state->r[0] = table_address + 0x28u + product;
    state->pc = state->pr;
}

SA_EXPORT void sa_input_device_record_index_8c65f3a0(
    uint8_t *ram, struct sa_cpu_state *state, uint32_t *macl)
{
    const uint32_t product = state->r[4] * 0x78u;
    const uint32_t pointer_address = read32(ram, 0x8c65f3d8u);
    const uint32_t table_address = read32(ram, pointer_address);
    const uint32_t field_offset = (uint32_t)read_signed16(ram, 0x8c65f3cau);

    state->r[2] = 0x78u;
    state->r[3] = pointer_address;
    state->r[1] = field_offset;
    *macl = product;
    state->r[4] = product;
    state->r[0] = table_address + field_offset + product;
    state->pc = state->pr;
}

SA_EXPORT void sa_input_device_record_index_8c65f3b2(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t pointer_address = read32(ram, 0x8c65f3d8u);
    const uint32_t table_address = read32(ram, pointer_address);
    const uint32_t original_index = state->r[4];

    state->r[3] = pointer_address;
    state->r[4] <<= 3;
    state->r[0] = table_address + 4u + state->r[4];
    state->sr = (state->sr & ~1u) | ((original_index >> 29) & 1u);
    state->pc = state->pr;
}
