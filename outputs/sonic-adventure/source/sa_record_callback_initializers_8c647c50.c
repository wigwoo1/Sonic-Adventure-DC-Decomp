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

static int16_t read16(const uint8_t *ram, uint32_t address)
{
    const uint32_t at = ram_offset(address);
    const uint16_t value = (uint16_t)ram[at] | ((uint16_t)ram[at + 1u] << 8);
    return (int16_t)value;
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    const uint32_t at = ram_offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

static void initialize_record_callbacks(uint8_t *ram, sa_cpu_state *cpu, uint32_t offset)
{
    const uint32_t initial_sp = cpu->r[15];
    cpu->r[15] = initial_sp - 4u;
    cpu->r[5] = read32(ram, 0x8c647d10u);
    cpu->r[3] = (uint32_t)(int32_t)read16(ram, 0x8c647d0eu);
    cpu->r[7] = cpu->r[5];
    cpu->r[6] = (uint32_t)(int32_t)read16(ram, 0x8c647d0cu);
    cpu->r[7] += cpu->r[3];
    write32(ram, cpu->r[15], cpu->r[5]);
    cpu->r[0] = offset;

    do {
        write32(ram, cpu->r[5] + cpu->r[0], cpu->r[4]);
        cpu->r[5] += cpu->r[6];
        const uint32_t done = cpu->r[5] >= cpu->r[7];
        cpu->sr = (cpu->sr & ~1u) | done;
        if (done != 0u) break;
    } while (1);

    cpu->r[15] = initial_sp;
    cpu->pc = cpu->pr;
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_record_callback_arg_initialize_8c647c50(uint8_t *ram, sa_cpu_state *cpu)
{
    initialize_record_callbacks(ram, cpu, 0x74u);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
void sa_record_callback_initialize_8c647c6c(uint8_t *ram, sa_cpu_state *cpu)
{
    initialize_record_callbacks(ram, cpu, 0x78u);
}