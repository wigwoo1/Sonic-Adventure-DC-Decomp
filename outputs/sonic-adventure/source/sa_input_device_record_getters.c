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

static uint32_t signed_word(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_index(address);
    const uint16_t value = (uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8);
    return (uint32_t)(int32_t)(int16_t)value;
}

static void get_field(uint8_t *ram, struct sa_cpu_state *state,
                      uint32_t pointer_literal_address,
                      uint32_t offset_literal_address)
{
    const uint32_t pointer_address = read32(ram, pointer_literal_address);
    const uint32_t field_offset = signed_word(ram, offset_literal_address);
    const uint32_t record_address = read32(ram, pointer_address);

    state->r[3] = pointer_address;
    state->r[1] = field_offset;
    state->r[0] = read32(ram, record_address + field_offset);
    state->pc = state->pr;
}

SA_EXPORT void sa_input_device_record_get_8c65f458(
    uint8_t *ram, struct sa_cpu_state *state)
{ get_field(ram, state, 0x8c65f530u, 0x8c65f518u); }

SA_EXPORT void sa_input_device_record_get_8c65f46c(
    uint8_t *ram, struct sa_cpu_state *state)
{ get_field(ram, state, 0x8c65f530u, 0x8c65f51au); }

SA_EXPORT void sa_input_device_record_get_8c65f480(
    uint8_t *ram, struct sa_cpu_state *state)
{ get_field(ram, state, 0x8c65f530u, 0x8c65f51cu); }

SA_EXPORT void sa_input_device_record_get_8c65f4ac(
    uint8_t *ram, struct sa_cpu_state *state)
{ get_field(ram, state, 0x8c65f530u, 0x8c65f51eu); }

SA_EXPORT void sa_input_device_record_get_8c65f4c0(
    uint8_t *ram, struct sa_cpu_state *state)
{ get_field(ram, state, 0x8c65f530u, 0x8c65f520u); }

SA_EXPORT void sa_input_device_record_get_8c65f4da(
    uint8_t *ram, struct sa_cpu_state *state)
{ get_field(ram, state, 0x8c65f530u, 0x8c65f524u); }

SA_EXPORT void sa_input_device_record_get_8c65f4f8(
    uint8_t *ram, struct sa_cpu_state *state)
{ get_field(ram, state, 0x8c65f530u, 0x8c65f528u); }

SA_EXPORT void sa_input_device_record_get_8c65f50c(
    uint8_t *ram, struct sa_cpu_state *state)
{ get_field(ram, state, 0x8c65f530u, 0x8c65f52cu); }

SA_EXPORT void sa_input_device_record_get_8c65f546(
    uint8_t *ram, struct sa_cpu_state *state)
{ get_field(ram, state, 0x8c65f554u, 0x8c65f550u); }
