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

static void refresh_field(uint8_t *ram, struct sa_cpu_state *state,
                          uint32_t record_stride, uint32_t record_field,
                          uint32_t object_field)
{
    int32_t count = (int32_t)state->r[5];
    uint32_t record = state->r[4];
    while (count > 0) {
        uint32_t object = read32(ram, record);
        uint32_t value = read32(ram, object + object_field);
        write32(ram, record + record_field, value);
        record += record_stride;
        --count;
    }
    state->pc = state->pr;
}

SA_EXPORT void sa_stg09_refresh_field14_from_object_0c90bd42(
    uint8_t *ram, struct sa_cpu_state *state)
{
    uint32_t count = state->r[5];
    state->r[6] = 0;
    state->sr = (state->sr & ~1u) | (uint32_t)((int32_t)count > 0);
    if ((int32_t)count > 0) {
        while (state->r[6] < count) {
            uint32_t record = state->r[4];
            uint32_t object = read32(ram, record);
            state->r[2] = object;
            state->r[6] += 1u;
            state->sr = (state->sr & ~1u) | (uint32_t)(state->r[6] >= count);
            state->r[3] = read32(ram, object + 20u);
            write32(ram, record + 12u, state->r[3]);
            state->r[4] += 16u;
        }
    }
    state->pc = state->pr;
}

SA_EXPORT void sa_stg09_refresh_field08_from_object_0c90bf34(
    uint8_t *ram, struct sa_cpu_state *state)
{
    refresh_field(ram, state, 12u, 8u, 12u);
}
