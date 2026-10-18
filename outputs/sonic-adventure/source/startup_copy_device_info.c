#include <stdint.h>

#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif

struct sa_startup_state {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
};

typedef void (*sa_startup_dispatch)(uint8_t *, struct sa_startup_state *,
                                    uint32_t, void *);

static uint32_t offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
}

static uint16_t read16(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint16_t)((uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8));
}

static uint8_t read8(const uint8_t *ram, uint32_t address)
{
    return ram[offset(address)];
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static void write16(uint8_t *ram, uint32_t address, uint16_t value)
{
    uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
}

SA_EXPORT void sa_startup_copy_device_info_8c010f9a(
        uint8_t *ram, struct sa_startup_state *state,
        sa_startup_dispatch dispatch, void *context)
{
    uint32_t destination = state->r[4];
    state->r[15] -= 4u;
    write32(ram, state->r[15], state->r[14]);
    state->r[15] -= 4u;
    write32(ram, state->r[15], state->pr);
    state->r[15] -= 12u;

    state->r[3] = 0x8c10baaaU;
    state->r[14] = destination;
    state->pr = 0x8c010fa8U;
    state->r[4] = state->r[15];
    dispatch(ram, state, state->r[3], context);
    state->pc = state->pr;

    state->r[2] = (uint32_t)(int32_t)(int16_t)read16(ram, state->r[15]);
    write16(ram, state->r[14], (uint16_t)state->r[2]);
    state->r[0] = (uint32_t)(int32_t)(int8_t)read8(ram, state->r[15] + 2u);
    ram[offset(state->r[14] + 2u)] = (uint8_t)state->r[0];
    state->r[0] = (uint32_t)(int32_t)(int8_t)read8(ram, state->r[15] + 3u);
    ram[offset(state->r[14] + 3u)] = (uint8_t)state->r[0];
    state->r[0] = (uint32_t)(int32_t)(int8_t)read8(ram, state->r[15] + 4u);
    ram[offset(state->r[14] + 4u)] = (uint8_t)state->r[0];
    state->r[0] = (uint32_t)(int32_t)(int8_t)read8(ram, state->r[15] + 5u);
    ram[offset(state->r[14] + 5u)] = (uint8_t)state->r[0];
    state->r[0] = (uint32_t)(int32_t)(int8_t)read8(ram, state->r[15] + 6u);
    ram[offset(state->r[14] + 6u)] = (uint8_t)state->r[0];
    state->r[0] = (uint32_t)(int32_t)(int8_t)read8(ram, state->r[15] + 7u);
    ram[offset(state->r[14] + 7u)] = (uint8_t)state->r[0];

    state->r[15] += 12u;
    state->pr = read32(ram, state->r[15]);
    state->r[15] += 4u;
    state->pc = state->pr;
    state->r[14] = read32(ram, state->r[15]);
    state->r[15] += 4u;
}
