#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu_state;

typedef void (*sa_callback_service)(uint8_t *, sa_cpu_state *);

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
void sa_callback_service_call_wrapper_8c645218(
    uint8_t *ram, sa_cpu_state *cpu, sa_callback_service service)
{
    const uint32_t stack_slot = cpu->r[15] - 4u;
    cpu->r[15] = stack_slot;
    write32(ram, stack_slot, cpu->pr);
    cpu->r[3] = read32(ram, 0x8c645334u);
    cpu->pr = 0x8c645220u;
    cpu->pc = cpu->r[3];
    service(ram, cpu);

    /* A returning callback resumes after the JSR delay slot. */
    cpu->pc = cpu->pr;
    cpu->pr = read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->pc = cpu->pr;
}
