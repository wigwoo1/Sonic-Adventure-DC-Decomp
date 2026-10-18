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

SA_EXPORT void sa_b_chaos6_sync_global_bytes_0c9030a4(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t source_slot = read32(ram, 0x0c90310cu);
    const uint32_t source = read32(ram, source_slot);
    const uint32_t source_byte = (uint32_t)(int32_t)(int8_t)ram[ram_index(source)];
    const uint32_t parameter = state->r[4];
    const uint32_t mirrored_slot = read32(ram, 0x0c903114u);
    const uint32_t requested_slot = read32(ram, 0x0c903110u);

    state->r[3] = source_slot;
    state->r[5] = source;
    state->r[2] = source_byte;
    state->sr = (state->sr & ~1u) | (uint32_t)(parameter == source_byte);
    if (parameter != source_byte) {
        state->r[0] = mirrored_slot;
        state->r[2] = requested_slot;
        ram[ram_index(requested_slot)] = (uint8_t)parameter;
        state->r[1] = (uint32_t)(int32_t)(int8_t)ram[ram_index(source)];
        ram[ram_index(mirrored_slot)] = (uint8_t)state->r[1];
    }
    state->pc = state->pr;
}
