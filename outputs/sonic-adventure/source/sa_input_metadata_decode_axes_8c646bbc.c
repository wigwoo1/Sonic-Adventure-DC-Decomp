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

static void write16(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
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
void sa_input_metadata_decode_axes_8c646bbc(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t initial_sp = cpu->r[15];
    const uint32_t original_r14 = cpu->r[14];

    cpu->r[15] = initial_sp - 4u;
    write32(ram, cpu->r[15], original_r14);
    cpu->r[15] = initial_sp - 8u;
    write32(ram, cpu->r[15], cpu->pr);
    cpu->r[15] = initial_sp - 12u;
    write32(ram, cpu->r[15], cpu->r[5]);
    cpu->r[14] = cpu->r[4];
    cpu->r[5] = 2u;
    cpu->r[4] = read32(ram, cpu->r[15]);
    cpu->pr = 0x8c646bccu;
    cpu->pc = 0x8c646bc8u;
    sa_input_metadata_bit_locator_8c646b42(ram, cpu);

    cpu->r[4] = cpu->r[0];
    cpu->sr = (cpu->sr & ~1u) | (cpu->r[4] == 0u);
    if (cpu->r[4] == 0u) {
        cpu->r[0] = 0xffffffffu;
        cpu->r[15] += 4u;
        cpu->pr = read32(ram, cpu->r[15]);
        cpu->r[15] += 4u;
        cpu->r[14] = read32(ram, cpu->r[15]);
        cpu->r[15] += 4u;
        cpu->pc = cpu->pr;
        return;
    }

    const uint32_t entry = cpu->r[4];
    cpu->r[5] = entry + 2u;
    cpu->r[3] = (uint32_t)read8(ram, entry) + 1u;
    write16(ram, cpu->r[14], cpu->r[3]);
    cpu->r[3] = 0xfffffff9u;
    cpu->r[0] = ((uint32_t)read8(ram, entry + 1u) + 1u) << 5;
    write16(ram, cpu->r[14] + 2u, cpu->r[0]);
    cpu->r[0] = (uint32_t)read8(ram, entry + 2u) >> 4;
    cpu->r[0] &= 0x0fu;
    write16(ram, cpu->r[14] + 4u, cpu->r[0]);
    cpu->r[0] = (uint32_t)read8(ram, entry + 2u) & 0x0fu;
    write16(ram, cpu->r[14] + 6u, cpu->r[0]);
    cpu->r[0] = ((uint32_t)read8(ram, entry + 3u) >> 7) & 1u;
    write16(ram, cpu->r[14] + 8u, cpu->r[0]);
    cpu->r[0] = 0u;
    cpu->r[15] += 4u;
    cpu->pr = read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->r[14] = read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->pc = cpu->pr;
}
