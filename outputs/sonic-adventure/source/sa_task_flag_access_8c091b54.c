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

static uint16_t read16(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_index(address);
    return (uint16_t)((uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8));
}

static void write16(uint8_t *ram, uint32_t address, uint16_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
}

static uint32_t task_flag_address(const uint8_t *ram, struct sa_cpu_state *state)
{
    state->r[3] = (uint32_t)ram_index(state->r[4] + 0x1cu);
    const uint32_t at = state->r[3];
    state->r[3] = (uint32_t)ram[at] |
                  ((uint32_t)ram[at + 1u] << 8) |
                  ((uint32_t)ram[at + 2u] << 16) |
                  ((uint32_t)ram[at + 3u] << 24);
    return state->r[3] + 2u;
}

static void set_task_flag(uint8_t *ram, struct sa_cpu_state *state, uint16_t mask)
{
    const uint32_t field = task_flag_address(ram, state);
    const uint16_t value = (uint16_t)(read16(ram, field) | mask);
    state->r[0] = (uint32_t)(int32_t)(int16_t)value;
    write16(ram, field, value);
    state->pc = state->pr;
}

SA_EXPORT void sa_task_flag_set_0x04_8c091b54(
    uint8_t *ram, struct sa_cpu_state *state)
{
    set_task_flag(ram, state, 0x0004u);
}

SA_EXPORT void sa_task_flag_set_0x10_8c091b5e(
    uint8_t *ram, struct sa_cpu_state *state)
{
    set_task_flag(ram, state, 0x0010u);
}

SA_EXPORT void sa_task_flag_test_0x04_8c091b68(
    const uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t saved_r3 = state->r[3];
    const uint32_t field = task_flag_address(ram, state);
    state->r[3] = saved_r3;
    state->r[0] = read16(ram, field) & 0x0004u;
    state->pc = state->pr;
}
