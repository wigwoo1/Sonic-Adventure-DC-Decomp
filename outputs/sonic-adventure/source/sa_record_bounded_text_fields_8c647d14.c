#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

static uint32_t offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static uint8_t read8(const uint8_t *ram, uint32_t address)
{
    return ram[offset(address)];
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static void write8(uint8_t *ram, uint32_t address, uint8_t value)
{
    ram[offset(address)] = value;
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_record_bounded_text_fields_8c647d14(uint8_t *ram, sa_cpu_state *cpu)
{
    const uint32_t index = cpu->r[4];
    const uint32_t table = read32(ram, 0x8c647da4u);
    const uint32_t record = table + (index << 7);
    const uint32_t source_one = cpu->r[5];
    const uint32_t source_two = cpu->r[6];
    uint32_t copied_one = 0;
    uint32_t copied_two = 0;
    uint8_t last = 0;

    write32(ram, cpu->r[15] - 4u, cpu->r[14]);
    for (uint32_t i = 0; i < 13u; ++i) {
        write8(ram, record + 0x2cu + i, 0);
        write8(ram, record + 0x3cu + i, 0);
    }
    while (copied_one < 12u) {
        const uint8_t value = read8(ram, source_one + copied_one);
        last = value;
        if (value == 0) break;
        write8(ram, record + 0x2cu + copied_one, value);
        ++copied_one;
    }
    while (copied_two < 12u) {
        const uint8_t value = read8(ram, source_two + copied_two);
        if (value == 0) break;
        last = value;
        write8(ram, record + 0x3cu + copied_two, value);
        ++copied_two;
    }

    cpu->r[0] = source_one + copied_one;
    cpu->r[2] = copied_two == 12u ? (uint32_t)(int32_t)(int8_t)last : 0u;
    cpu->r[3] = (uint32_t)(int32_t)(int8_t)last;
    cpu->r[4] = source_two + copied_two;
    cpu->r[5] = record + 0x3cu + copied_two;
    cpu->r[6] = source_two + 12u;
    cpu->r[7] = record;
    cpu->sr = (cpu->sr & ~1u) | 1u;
    cpu->pc = cpu->pr;
}
