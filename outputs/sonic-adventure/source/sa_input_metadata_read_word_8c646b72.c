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
void sa_input_metadata_read_word_8c646b72(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t initial_sp = cpu->r[15];
    const uint32_t frame = initial_sp - 12u;
    const uint32_t original_pr = cpu->pr;

    cpu->r[15] = initial_sp - 4u;
    write32(ram, cpu->r[15], original_pr);
    cpu->r[15] = frame;
    write32(ram, frame, cpu->r[4]);
    write32(ram, frame + 4u, cpu->r[5]);
    cpu->r[5] = 1u;
    cpu->pr = 0x8c646b80u;
    cpu->r[4] = read32(ram, frame + 4u);
    cpu->pc = 0x8c646b7cu;
    sa_input_metadata_bit_locator_8c646b42(ram, cpu);
    cpu->r[4] = cpu->r[0];

    if (cpu->r[0] == 0u) {
        cpu->sr |= 1u;
        cpu->r[0] = 0xffffffffu;
        cpu->r[15] = frame + 8u;
        cpu->pr = read32(ram, cpu->r[15]);
        cpu->r[15] += 4u;
        cpu->pc = cpu->pr;
        return;
    }

    const uint32_t entry = cpu->r[0];
    const uint8_t byte0 = read8(ram, entry);
    const uint8_t byte1 = read8(ram, entry + 1u);
    const uint8_t byte2 = read8(ram, entry + 2u);
    const uint8_t byte3 = read8(ram, entry + 3u);
    const uint32_t destination = read32(ram, frame);
    const uint32_t value = ((uint32_t)byte0 << 24) |
                           ((uint32_t)byte1 << 16) |
                           ((uint32_t)byte2 << 8) | (uint32_t)byte3;

    cpu->r[0] = byte3;
    cpu->r[1] = (uint32_t)byte0 << 24;
    cpu->r[2] = value;
    cpu->r[3] = destination;
    cpu->r[4] = entry;
    cpu->sr &= ~1u;
    write32(ram, destination, value);
    cpu->r[0] = 0u;
    cpu->r[15] = frame + 8u;
    cpu->pr = read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->pc = cpu->pr;
}
