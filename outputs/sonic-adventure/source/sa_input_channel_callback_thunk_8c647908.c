#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

static uint32_t ram_offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_input_channel_callback_thunk_8c647908(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t target = read32(ram, 0x8c64797cu);
    cpu->r[3] = target;
    cpu->pc = target;
}