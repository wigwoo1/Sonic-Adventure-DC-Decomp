#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu;

typedef void (*sa_release_service_fn)(void *, uint32_t, uint8_t *, sa_cpu *);

void sa_stg09_global_reference_release_0c918944(
    uint8_t *, sa_cpu *, sa_release_service_fn, void *);

#define RAM_BASE 0x0c000000u
#define HELPER_ENTRY 0x0c918944u
#define HELPER_RETURN 0x0c9189c0u

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

static void write32(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
    ram[at + 2u] = (uint8_t)(value >> 16);
    ram[at + 3u] = (uint8_t)(value >> 24);
}

__declspec(dllexport)
void sa_stg09_resource_release_wrapper_0c9189ae(
    uint8_t *ram, sa_cpu *cpu, sa_release_service_fn service, void *context)
{
    uint32_t saved_pr = cpu->pr;
    uint32_t object = cpu->r[4];
    uint32_t field;

    cpu->r[15] -= 4u;
    write32(ram, cpu->r[15], saved_pr);
    cpu->r[15] -= 4u;
    write32(ram, cpu->r[15], object);

    cpu->r[3] = object;
    cpu->r[2] = read32(ram, object + 8u);
    field = cpu->r[2];
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(field == 0u);
    if (field != 0u) {
        cpu->pr = HELPER_RETURN;
        cpu->pc = HELPER_ENTRY;
        sa_stg09_global_reference_release_0c918944(ram, cpu, service, context);

        cpu->r[1] = read32(ram, cpu->r[15]);
        cpu->r[3] = 0u;
        write32(ram, cpu->r[1] + 8u, cpu->r[3]);
    }

    cpu->r[15] += 4u;
    cpu->pr = read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    cpu->pc = cpu->pr;
}
