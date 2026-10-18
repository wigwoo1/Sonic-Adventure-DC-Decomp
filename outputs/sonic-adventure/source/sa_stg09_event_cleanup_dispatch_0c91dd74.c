#include <stdint.h>

#define RAM_BASE 0x0c000000u
#define STATUS_SERVICE_LITERAL 0x0c91de48u
#define NOTIFY_SERVICE_LITERAL 0x0c91de60u
#define REMOVE_SERVICE_LITERAL 0x0c91de64u
#define NOTIFY_DATA_LITERAL 0x0c91de50u

typedef struct {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
} sa_cpu;

typedef struct {
    uint32_t banks[2][16];
    uint32_t fpscr;
    uint32_t fpul;
} sa_fpu;

typedef int (*sa_event_service_fn)(void *context, uint32_t target,
                                   uint8_t *ram, sa_cpu *cpu, sa_fpu *fpu);

static uint32_t offset(uint32_t address)
{
    return (address & 0x1fffffffu) - RAM_BASE;
}

static uint32_t read32(const uint8_t *ram, uint32_t address)
{
    uint32_t at = offset(address);
    return (uint32_t)ram[at] |
           ((uint32_t)ram[at + 1u] << 8) |
           ((uint32_t)ram[at + 2u] << 16) |
           ((uint32_t)ram[at + 3u] << 24);
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

static void call_service(uint8_t *ram, sa_cpu *cpu, sa_fpu *fpu,
                         sa_event_service_fn service, void *context,
                         uint32_t target, uint32_t return_pc)
{
    cpu->pr = return_pc;
    cpu->pc = target;
    (void)service(context, target, ram, cpu, fpu);
    cpu->pc = cpu->pr;
}

static void set_t(sa_cpu *cpu, uint32_t value)
{
    cpu->sr = (cpu->sr & ~1u) | (uint32_t)(value == 0u);
}

__declspec(dllexport)
void sa_stg09_event_cleanup_dispatch_0c91dd74(uint8_t *ram, sa_cpu *cpu,
                                             sa_fpu *fpu,
                                             sa_event_service_fn service,
                                             void *context)
{
    uint32_t original_sp = cpu->r[15];
    uint32_t original_r4 = cpu->r[4];
    uint32_t entity;
    uint32_t field_10;
    uint32_t field_c;
    uint32_t target;

    push32(ram, cpu, cpu->r[14]);
    push32(ram, cpu, cpu->pr);

    /* The JSR delay slot reads *(R4 + 0x20) before entering the status service. */
    target = read32(ram, STATUS_SERVICE_LITERAL);
    cpu->r[3] = target;
    cpu->pr = 0x0c91dd7eu;
    cpu->pc = target;
    cpu->r[14] = read32(ram, original_r4 + 0x20u);
    (void)service(context, target, ram, cpu, fpu);
    cpu->pc = cpu->pr;

    /* BF is not delayed; preserve the status service's volatile R3 on early return. */
    set_t(cpu, cpu->r[0]);
    if (cpu->r[0] != 0u) {
        cpu->pr = pop32(ram, cpu);
        cpu->r[14] = pop32(ram, cpu);
        cpu->pc = cpu->pr;
        return;
    }

    entity = cpu->r[14];
    field_10 = read32(ram, entity + 0x10u);
    cpu->r[3] = field_10;
    field_c = read32(ram, entity + 0x0cu);
    set_t(cpu, field_10);
    if (field_10 != 0u) {
        cpu->r[3] = field_c;
        set_t(cpu, field_c);
    }
    if (field_10 != 0u && field_c != 0u) {
        /* Load the optional notification target and data, then call with field_10 in R4. */
        cpu->r[5] = field_c;
        target = read32(ram, NOTIFY_SERVICE_LITERAL);
        cpu->r[3] = target;
        cpu->r[6] = read32(ram, NOTIFY_DATA_LITERAL);
        fpu->banks[(fpu->fpscr >> 21) & 1u][4] = 0;
        cpu->r[4] = field_10;
        call_service(ram, cpu, fpu, service, context, target, 0x0c91dd9au);
    }

    /* Ordinary JSR to the shared field-removal service with field_10. */
    target = read32(ram, REMOVE_SERVICE_LITERAL);
    cpu->r[2] = target;
    cpu->r[4] = field_10;
    call_service(ram, cpu, fpu, service, context, target, 0x0c91dda0u);

    /* Restore saved PR, then tail-transfer with field_c; the delay slot restores R14. */
    cpu->pr = pop32(ram, cpu);
    cpu->r[3] = read32(ram, REMOVE_SERVICE_LITERAL);
    cpu->r[4] = field_c;
    cpu->r[14] = pop32(ram, cpu);
    cpu->pc = cpu->r[3];
    (void)service(context, cpu->r[3], ram, cpu, fpu);
    cpu->pc = cpu->pr;

    (void)original_sp;
}
