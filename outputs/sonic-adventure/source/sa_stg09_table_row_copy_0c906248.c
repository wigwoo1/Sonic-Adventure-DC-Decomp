#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_stg09_cpu;

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t offset = (address & 0x1fffffffu) - 0x0c000000u;
    return (uint32_t)ram[offset] |
           ((uint32_t)ram[offset + 1] << 8) |
           ((uint32_t)ram[offset + 2] << 16) |
           ((uint32_t)ram[offset + 3] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t offset = (address & 0x1fffffffu) - 0x0c000000u;
    ram[offset] = (uint8_t)value;
    ram[offset + 1] = (uint8_t)(value >> 8);
    ram[offset + 2] = (uint8_t)(value >> 16);
    ram[offset + 3] = (uint8_t)(value >> 24);
}

__declspec(dllexport)
void sa_stg09_table_row_copy_0c906248(uint8_t *ram, sa_stg09_cpu *cpu)
{
    uint32_t destination = cpu->r[4];
    uint32_t selector = cpu->r[5];
    uint32_t table = read32(ram, 0x0c906320u);
    uint32_t row = table + selector * 12u;
    uint32_t second = read32(ram, row + 4u);
    uint32_t third = read32(ram, row + 8u);

    write32(ram, destination + 8u, read32(ram, row));
    write32(ram, destination + 12u, second);
    write32(ram, destination + 16u, third);
    ram[(destination & 0x1fffffffu) - 0x0c000000u] = (uint8_t)selector;
    cpu->r[1] = second;
    cpu->r[2] = table;
    cpu->r[3] = third;
    cpu->r[6] = row;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(cpu->r[1] == cpu->r[2]);
    cpu->pc = cpu->pr;
}
