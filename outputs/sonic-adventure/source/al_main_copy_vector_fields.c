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

static void copy_vector_fields(uint8_t *ram, struct sa_cpu_state *state,
                               uint32_t destination_offset, int destination_register)
{
    const uint32_t source = state->r[4];
    const uint32_t destination = state->r[5] + destination_offset;
    const uint32_t first = read32(ram, source + 0x20u);
    const uint32_t second = read32(ram, source + 0x24u);
    uint32_t third;

    if (destination_register == 0) {
        state->r[0] = destination;
    } else {
        state->r[1] = destination;
    }
    state->r[2] = second;
    state->r[3] = first;
    write32(ram, destination, first);
    write32(ram, destination + 4u, second);
    third = read32(ram, source + 0x28u);
    state->r[3] = third;
    write32(ram, destination + 8u, third);
    state->pc = state->pr;
}

SA_EXPORT void sa_al_main_copy_vector_fields_a_0c90bab4(
    uint8_t *ram, struct sa_cpu_state *state)
{
    copy_vector_fields(ram, state, 0x8cu, 0);
}

SA_EXPORT void sa_al_main_copy_vector_fields_b_0c90bac8(
    uint8_t *ram, struct sa_cpu_state *state)
{
    copy_vector_fields(ram, state, 0xa4u, 1);
}
