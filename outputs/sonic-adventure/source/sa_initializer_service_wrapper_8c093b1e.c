#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

typedef void (*sa_initializer_service)(uint8_t *, sa_cpu_state *);

static uint32_t offset(uint32_t address)
{
    return address & 0x00ffffffu;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] | ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) | ((uint32_t)ram[at + 3u] << 24);
}

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

__declspec(dllexport)
void sa_initializer_service_wrapper_8c093b1e(
    uint8_t *ram, sa_cpu_state *cpu, sa_initializer_service service)
{
    uint32_t saved_pr;

    cpu->r[15] -= 4u;
    write32(ram, cpu->r[15], cpu->pr);
    cpu->pr = 0x8c093b24u;
    cpu->pc = 0x8c093ce0u;
    service(ram, cpu);

    /* The called initializer returns through the wrapper's saved PR. */
    cpu->pc = cpu->pr;
    saved_pr = read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->pr = saved_pr;
    cpu->r[0] = 0u;
    cpu->pc = cpu->pr;
}
