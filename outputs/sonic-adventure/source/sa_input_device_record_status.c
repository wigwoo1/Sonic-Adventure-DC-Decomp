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

static void record_index_2c(const uint8_t *ram, struct sa_cpu_state *state,
                            uint32_t *macl)
{
    const uint32_t product = state->r[4] * 0x2cu;
    const uint32_t pointer_address = read32(ram, 0x8c65f3d8u);
    const uint32_t table_address = read32(ram, pointer_address);

    state->r[2] = 0x2cu;
    state->r[3] = pointer_address;
    *macl = product;
    state->r[4] = product;
    state->r[0] = table_address + 0x28u + product;
}

SA_EXPORT void sa_input_device_record_matches_8c65f3f4(
    uint8_t *ram, struct sa_cpu_state *state, uint32_t *macl)
{
    const uint32_t original_pr = state->pr;
    const uint32_t original_r5 = state->r[5];

    state->r[15] -= 4u;
    write32(ram, state->r[15], original_pr);
    state->r[15] -= 4u;
    write32(ram, state->r[15], original_r5);
    state->pr = 0x8c65f3fcu;
    record_index_2c(ram, state, macl);

    state->r[4] = state->r[0];
    state->r[3] = read32(ram, state->r[4] + 0x20u);
    state->r[2] = read32(ram, state->r[15]);
    state->sr = (state->sr & ~1u) | (uint32_t)(state->r[2] == state->r[3]);
    state->r[0] = state->sr & 1u;
    state->r[15] += 4u;
    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->pc = state->pr;
}

SA_EXPORT void sa_input_device_record_status_get_8c65f40e(
    uint8_t *ram, struct sa_cpu_state *state, uint32_t *macl)
{
    const uint32_t original_pr = state->pr;

    state->r[15] -= 4u;
    write32(ram, state->r[15], original_pr);
    state->pr = 0x8c65f414u;
    record_index_2c(ram, state, macl);

    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->r[4] = state->r[0];
    state->r[0] = ram[ram_index(state->r[4] + 0x0cu)];
    state->pc = state->pr;
}

SA_EXPORT void sa_input_device_record_status_set_8c65f41e(
    uint8_t *ram, struct sa_cpu_state *state, uint32_t *macl)
{
    const uint32_t original_pr = state->pr;

    state->r[15] -= 4u;
    write32(ram, state->r[15], original_pr);
    state->pr = 0x8c65f424u;
    record_index_2c(ram, state, macl);

    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->r[4] = state->r[0];
    state->r[0] = 1u;
    ram[ram_index(state->r[4] + 0x0du)] = 1u;
    state->pc = state->pr;
}
