#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

extern void sa_input_metadata_bit_locator_8c646b42(uint8_t *ram,
                                                     sa_cpu_state *cpu);

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

static uint8_t read8(const uint8_t *ram, uint32_t address)
{
    return ram[ram_offset(address)];
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_input_metadata_read_two_bytes_8c646d7a(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t initial_sp = cpu->r[15];
    const uint32_t frame = initial_sp - 12u;

    cpu->r[15] = initial_sp - 4u;
    write32(ram, cpu->r[15], cpu->pr);
    cpu->r[15] = frame;
    write32(ram, frame, cpu->r[4]);
    write32(ram, frame + 4u, cpu->r[5]);
    cpu->r[5] = 0x100u;
    cpu->pr = 0x8c646d88u;
    cpu->r[4] = read32(ram, frame + 4u);
    cpu->pc = 0x8c646d84u;
    sa_input_metadata_bit_locator_8c646b42(ram, cpu);

    cpu->r[4] = cpu->r[0];
    cpu->sr = (cpu->sr & ~1u) | (cpu->r[4] == 0u);
    if (cpu->r[4] == 0u) {
        cpu->r[15] = frame + 8u;
        cpu->pr = read32(ram, cpu->r[15]);
        cpu->r[15] += 4u;
        cpu->pc = cpu->pr;
        cpu->r[0] = 0xffffffffu;
        return;
    }

    const uint32_t entry = cpu->r[4];
    cpu->r[2] = read8(ram, entry);
    cpu->r[3] = read32(ram, frame);
    cpu->r[2] &= 0xffu;
    write32(ram, cpu->r[3], cpu->r[2]);
    cpu->r[0] = read8(ram, entry + 1u);
    cpu->r[3] = read32(ram, frame);
    cpu->r[0] &= 0xffu;
    write32(ram, cpu->r[3] + 4u, cpu->r[0]);
    cpu->r[0] = 0u;
    cpu->r[15] = frame + 8u;
    cpu->pr = read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->pc = cpu->pr;
}
