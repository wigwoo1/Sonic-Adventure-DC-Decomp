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

typedef uint32_t (*sa_mmio_read32_fn)(void *context, uint32_t address);

static uint32_t ram_index(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_index(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

SA_EXPORT void sa_mmio_complement_sub_8c60260e(
    const uint8_t *ram, struct sa_cpu_state *state,
    void *context, sa_mmio_read32_fn read_register)
{
    const uint32_t address = read32(ram, 0x8c60271cu);
    state->r[2] = address;
    state->r[3] = read_register(context, address);
    state->r[0] = 0xffffffffu - state->r[3];
    state->pc = state->pr;
}
