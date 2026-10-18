#include <stdint.h>

#define RAM_BASE 0x0c000000u
#define SERVICE_LITERAL 0x0c902840u
#define SERVICE_DATA_LITERAL 0x0c90283cu
#define ENTITY_GLOBAL_LITERAL 0x0c902858u
#define CALLBACK_LITERAL 0x0c90285cu
#define PARAM_CALLBACK_A_LITERAL 0x0c90284cu
#define PARAM_CALLBACK_B_LITERAL 0x0c902850u
#define ENTITY_CONSTANT_LITERAL 0x0c902854u

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

typedef int (*sa_initializer_service_fn)(void *context, uint32_t target,
                                         uint8_t *ram, sa_cpu *cpu,
                                         sa_fpu *fpu);

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

static void write16(uint8_t *ram, uint32_t address, uint32_t value)
{
    uint32_t at = offset(address);
    ram[at] = (uint8_t)value;
    ram[at + 1u] = (uint8_t)(value >> 8);
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
void sa_stg09_callback_initializer_0c9027c4(uint8_t *ram, sa_cpu *cpu,
                                           sa_fpu *fpu,
                                           sa_initializer_service_fn service,
                                           void *context)
{
    uint32_t parameter;
    uint32_t entity;
    uint32_t service_target;
    uint32_t value;

    push32(ram, cpu, cpu->r[14]);
    parameter = cpu->r[4];
    cpu->r[14] = parameter;
    push32(ram, cpu, cpu->r[13]);
    cpu->r[3] = 2u;
    push32(ram, cpu, cpu->pr);
    cpu->r[5] = read32(ram, SERVICE_DATA_LITERAL);
    cpu->r[6] = 4u;
    service_target = read32(ram, SERVICE_LITERAL);
    cpu->r[2] = service_target;
    cpu->r[7] = 1u;

    /* JSR delay slot loads the entity from parameter + 0x20. */
    entity = read32(ram, cpu->r[14] + 0x20u);
    cpu->r[13] = entity;
    push32(ram, cpu, cpu->r[3]);

    /* The external service receives parameter in R4; R3=2 is also stack-passed. */
    cpu->pr = 0x0c9027deu;
    cpu->pc = service_target;
    cpu->r[4] = cpu->r[14];
    (void)service(context, service_target, ram, cpu, fpu);
    cpu->pc = cpu->pr;

    cpu->r[2] = read32(ram, ENTITY_CONSTANT_LITERAL);
    cpu->r[3] = 0u;
    write32(ram, cpu->r[13] + 0x0cu, cpu->r[3]);
    cpu->r[0] = (uint32_t)-128;
    write32(ram, cpu->r[13] + 0x10u, cpu->r[2]);
    /* SHLL8 shifts the register without changing T (Renesas SH-4 manual, table 7.6). */
    cpu->r[0] <<= 8;
    cpu->r[2] = read32(ram, ENTITY_GLOBAL_LITERAL);
    cpu->r[1] = read32(ram, CALLBACK_LITERAL);
    write16(ram, cpu->r[13] + 4u, cpu->r[0]);
    value = read32(ram, cpu->r[2]);
    cpu->r[3] = value;
    write32(ram, cpu->r[13] + 8u, cpu->r[3]);
    write32(ram, parameter + 0x10u, cpu->r[1]);
    cpu->r[3] = read32(ram, PARAM_CALLBACK_A_LITERAL);
    cpu->r[1] = read32(ram, PARAM_CALLBACK_B_LITERAL);
    write32(ram, parameter + 0x14u, cpu->r[3]);
    write32(ram, parameter + 0x18u, cpu->r[1]);

    cpu->r[15] += 4u; /* discard the stack-passed value 2 */
    cpu->pr = pop32(ram, cpu);
    cpu->r[13] = pop32(ram, cpu);
    cpu->pc = cpu->pr;
    cpu->r[14] = pop32(ram, cpu); /* RTS delay slot */
}
