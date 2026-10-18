#include <stdint.h>

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_stg09_cpu;

/* The comparison worker at 0x0c903a7c is recovered separately. */
extern void sa_stg09_tikal_compare_worker_0c903a7c(uint8_t *ram,
                                                   sa_stg09_cpu *cpu);

static uint32_t offset(uint32_t address)
{
    return (address & 0x1fffffffu) - 0x0c000000u;
}

__declspec(dllexport)
void sa_stg09_tikal_compare_counter_0c903be0(uint8_t *ram, sa_stg09_cpu *cpu)
{
    uint32_t object = cpu->r[4];
    uint32_t object_offset = offset(object);
    uint8_t flags = ram[object_offset + 3u];
    uint8_t active = ram[object_offset + 10u];

    cpu->r[0] = flags;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)((flags & 3u) == 0u);
    if ((flags & 3u) != 0u) {
        cpu->pc = cpu->pr;
        return;
    }
    cpu->r[0] = active;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(active == 0u);
    if (active == 0u) {
        cpu->pc = cpu->pr;
        return;
    }

    uint8_t count = (uint8_t)(ram[object_offset + 19u] + 1u);
    ram[object_offset + 19u] = count;
    cpu->r[0] = count == 3u ? 0u : (uint32_t)(int32_t)(int8_t)count;
    cpu->r[1] = object + 19u;
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(count == 3u);
    if (count == 3u) {
        ram[object_offset + 10u] = 0;
    }

    /* Retail tail-transfers here with the caller's PR still live. */
    cpu->pc = 0x0c903a7cu;
    sa_stg09_tikal_compare_worker_0c903a7c(ram, cpu);
}
