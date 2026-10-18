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

static void resolve_task_object(uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t is_null = state->r[4] == 0u;
    state->sr = (state->sr & ~1u) | is_null;
    if (is_null != 0u) {
        state->r[2] = 0x8c7a0a08u;
        state->r[4] = read32(ram, state->r[2]);
    }
}

SA_EXPORT void sa_task_work_address_get_8c099d2a(
    uint8_t *ram, struct sa_cpu_state *state)
{
    resolve_task_object(ram, state);
    state->r[0] = state->r[4] + 0x24u;
    state->pc = state->pr;
}

SA_EXPORT void sa_task_work_set_8c099d38(
    uint8_t *ram, struct sa_cpu_state *state)
{
    resolve_task_object(ram, state);
    write32(ram, state->r[4] + 0x20u, state->r[5]);
    state->pc = state->pr;
}

SA_EXPORT void sa_task_work_aux_set_8c099d80(
    uint8_t *ram, struct sa_cpu_state *state)
{
    resolve_task_object(ram, state);
    write32(ram, state->r[4] + 0x18u, state->r[5]);
    state->pc = state->pr;
}

SA_EXPORT void sa_task_work_get_8c099d44(
    uint8_t *ram, struct sa_cpu_state *state)
{
    resolve_task_object(ram, state);
    state->r[0] = read32(ram, state->r[4] + 0x20u);
    state->pc = state->pr;
}
