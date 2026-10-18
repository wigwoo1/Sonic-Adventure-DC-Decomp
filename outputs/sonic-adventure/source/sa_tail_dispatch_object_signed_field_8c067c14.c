#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

static uint32_t offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static int16_t read16(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = offset(address);
    const uint16_t value = (uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8);
    return (int16_t)value;
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_tail_dispatch_object_signed_field_8c067c14(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t object = cpu->r[4];
    const uint32_t stack_slot = cpu->r[15] - 4u;
    cpu->r[15] = stack_slot;
    write32(ram, stack_slot, object);
    cpu->r[4] = read32(ram, object + 0x20u);
    cpu->r[0] = (uint32_t)(int32_t)read16(ram, cpu->r[4] + 14u);
    cpu->r[4] = cpu->r[0];
    cpu->r[15] += 4u;
    cpu->pc = 0x8c067d84u;
}
