#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu;

#define RAM_BASE 0x0c000000u
#define TABLE_POINTER_LITERAL 0x0c9187ccu

static uint32_t offset(uint32_t address)
{
    return (address & 0x1fffffffu) - RAM_BASE;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

__declspec(dllexport)
void sa_stg09_signed_byte_word_lookup_0c918790(const uint8_t *ram, sa_cpu *cpu)
{
    int32_t index = (int8_t)cpu->r[4];
    cpu->r[2] = 5u;
    cpu->r[3] = (uint32_t)index;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(index >= 5);
    if (index < 5) {
        uint32_t table = read32(ram, TABLE_POINTER_LITERAL);
        cpu->r[1] = table;
        cpu->r[0] = (uint32_t)index << 2;
        cpu->r[0] = read32(ram, table + cpu->r[0]);
    } else {
        cpu->r[0] = 0u;
    }
    cpu->pc = cpu->pr;
}
