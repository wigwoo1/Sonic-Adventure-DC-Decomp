#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu;

typedef void (*sa_release_service_fn)(void *, uint32_t, uint8_t *, sa_cpu *);

#define RAM_BASE 0x0c000000u
#define GLOBAL_DATA_LITERAL 0x0c918a10u
#define REFCOUNT_LITERAL 0x0c918a14u
#define RELEASE_SERVICE_LITERAL 0x0c918a20u
#define SERVICE_RETURN_PC 0x0c918962u

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

static void push32(uint8_t *ram, sa_cpu *cpu, uint32_t value)
{
    cpu->r[15] -= 4u;
    write32(ram, cpu->r[15], value);
}

static uint32_t pop32(const uint8_t *ram, sa_cpu *cpu)
{
    uint32_t value = read32(ram, cpu->r[15]);
    cpu->r[15] += 4u;
    return value;
}

__declspec(dllexport)
void sa_stg09_global_reference_release_0c918944(
    uint8_t *ram, sa_cpu *cpu, sa_release_service_fn service, void *context)
{
    uint32_t saved_r14 = cpu->r[14];
    uint32_t global_data = read32(ram, GLOBAL_DATA_LITERAL);
    uint32_t saved_pr = cpu->pr;
    uint32_t object;

    push32(ram, cpu, saved_r14);
    cpu->r[14] = global_data;
    push32(ram, cpu, saved_pr);
    object = read32(ram, global_data);
    cpu->r[3] = object;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(object == 0u);

    if (object != 0u) {
        uint32_t refcount_address = read32(ram, REFCOUNT_LITERAL);
        uint32_t count = read32(ram, refcount_address) - 1u;
        cpu->r[3] = refcount_address;
        cpu->r[2] = count;
        cpu->sr = (cpu->sr & ~1u) | (uint32_t)(count == 0u);
        write32(ram, refcount_address, count);
        if (count == 0u) {
            uint32_t target = read32(ram, RELEASE_SERVICE_LITERAL);
            cpu->r[2] = target;
            cpu->r[4] = object;
            cpu->pr = SERVICE_RETURN_PC;
            cpu->pc = target;
            service(context, target, ram, cpu);
            cpu->pc = cpu->pr;
            cpu->r[3] = 0u;
            write32(ram, global_data, 0u);
        }
    }

    cpu->pr = pop32(ram, cpu);
    cpu->r[14] = pop32(ram, cpu);
    cpu->pc = cpu->pr;
}
