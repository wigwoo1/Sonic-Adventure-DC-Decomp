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

static uint8_t read8(const uint8_t *ram, uint32_t address)
{
    return ram[ram_index(address)];
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_index(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void write8(uint8_t *ram, uint32_t address, uint8_t value)
{
    ram[ram_index(address)] = value;
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_index(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

/* First-free allocator for 0x100 records of 0x34 bytes each. */
SA_EXPORT void sa_record_pool_allocate_8c052a1e(
    uint8_t *ram, struct sa_cpu_state *state)
{
    const uint32_t allocation_bytes = read32(ram, 0x8c052b60u);
    const uint32_t record_pool = read32(ram, 0x8c052b64u);
    int32_t index;

    state->r[3] = record_pool;

    for (index = 0xff; index >= 0; --index) {
        if (read8(ram, allocation_bytes + (uint32_t)index) == 0u)
            break;
    }

    state->r[2] = (uint32_t)index;
    state->sr = (state->sr & ~1u) | (index >= 0 ? 1u : 0u);
    if (index >= 0) {
        const uint32_t slot = (uint32_t)index;
        const uint32_t record = record_pool + slot * 0x34u;
        write8(ram, allocation_bytes + slot, 1u);
        write32(ram, record + 4u, 0u);
        write32(ram, record + 0x2cu, 0u);
        write32(ram, record + 0x30u, 0u);
        state->r[0] = record;
        state->r[5] = record;
        state->r[6] = 0u;
    } else {
        state->r[0] = 0u;
        state->r[3] = 1u;
        state->r[5] = allocation_bytes;
    }

    state->pc = state->pr;
}
