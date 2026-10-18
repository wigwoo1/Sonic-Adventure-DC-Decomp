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

static uint8_t read8(const uint8_t *ram, uint32_t address)
{
    return ram[ram_index(address)];
}

static void write8(uint8_t *ram, uint32_t address, uint8_t value)
{
    ram[ram_index(address)] = value;
}

static void update_counter(uint8_t *ram, struct sa_cpu_state *state,
                           uint32_t literal_address, int32_t delta)
{
    state->r[4] = read32(ram, literal_address);
    state->r[3] = (uint32_t)(int32_t)(int8_t)read8(ram, state->r[4]);
    state->r[3] += (uint32_t)delta;
    write8(ram, state->r[4], (uint8_t)state->r[3]);
    state->pc = state->pr;
}

static void read_counter(const uint8_t *ram, struct sa_cpu_state *state,
                         uint32_t literal_address)
{
    state->r[2] = read32(ram, literal_address);
    state->r[0] = read8(ram, state->r[2]);
    state->pc = state->pr;
}

SA_EXPORT void sa_b_chaos4_counter_pair_initialize_0c905a80(
    uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[3] = read32(ram, 0x0c905ae0u);
    state->r[4] = 0;
    state->r[2] = read32(ram, 0x0c905ae4u);
    write8(ram, state->r[3], 0);
    write8(ram, state->r[2], 0);
    state->pc = state->pr;
}

SA_EXPORT void sa_b_chaos4_counter_secondary_increment_0c905a8c(
    uint8_t *ram, struct sa_cpu_state *state)
{
    update_counter(ram, state, 0x0c905ae4u, 1);
}

SA_EXPORT void sa_b_chaos4_counter_secondary_decrement_0c905a96(
    uint8_t *ram, struct sa_cpu_state *state)
{
    update_counter(ram, state, 0x0c905ae4u, -1);
}

SA_EXPORT void sa_b_chaos4_counter_secondary_read_0c905aa0(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    read_counter(ram, state, 0x0c905ae4u);
}

SA_EXPORT void sa_b_chaos4_counter_primary_increment_0c905aa8(
    uint8_t *ram, struct sa_cpu_state *state)
{
    update_counter(ram, state, 0x0c905ae0u, 1);
}

SA_EXPORT void sa_b_chaos4_counter_primary_decrement_0c905ab2(
    uint8_t *ram, struct sa_cpu_state *state)
{
    update_counter(ram, state, 0x0c905ae0u, -1);
}

SA_EXPORT void sa_b_chaos4_counter_primary_read_0c905abc(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    read_counter(ram, state, 0x0c905ae0u);
}
