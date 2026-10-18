#include <stdint.h>

#define RAM_BASE 0x0c000000u
#define SERVICE_LITERAL 0x0c902840u
#define SERVICE_DATA_LITERAL 0x0c90283cu
#define ENTITY_GLOBAL 0x0ca0e9b0u
#define CALLBACK_LITERAL 0x0c902848u
#define PARAM_CALLBACK_A_LITERAL 0x0c90284cu
#define PARAM_CALLBACK_B_LITERAL 0x0c902850u

typedef struct { uint32_t r[16], sr, pr, pc; } sa_cpu;
typedef struct { uint32_t banks[2][16], fpscr, fpul; } sa_fpu;
typedef int (*sa_initializer_service_fn)(void *, uint32_t, uint8_t *, sa_cpu *, sa_fpu *);

static uint32_t offset(uint32_t a) { return (a & 0x1fffffffu) - RAM_BASE; }
static uint32_t read32(const uint8_t *m, uint32_t a)
{
    uint32_t o = offset(a);
    return (uint32_t)m[o] | ((uint32_t)m[o+1] << 8) |
           ((uint32_t)m[o+2] << 16) | ((uint32_t)m[o+3] << 24);
}
static void write16(uint8_t *m, uint32_t a, uint32_t v)
{
    uint32_t o = offset(a); m[o] = (uint8_t)v; m[o+1] = (uint8_t)(v >> 8);
}
static void write32(uint8_t *m, uint32_t a, uint32_t v)
{
    uint32_t o = offset(a); m[o] = (uint8_t)v; m[o+1] = (uint8_t)(v >> 8);
    m[o+2] = (uint8_t)(v >> 16); m[o+3] = (uint8_t)(v >> 24);
}
static void push32(uint8_t *m, sa_cpu *c, uint32_t v)
{ c->r[15] -= 4; write32(m, c->r[15], v); }
static uint32_t pop32(const uint8_t *m, sa_cpu *c)
{ uint32_t v = read32(m, c->r[15]); c->r[15] += 4; return v; }

__declspec(dllexport)
void sa_stg09_callback_initializer_0c90277c(uint8_t *ram, sa_cpu *cpu,
                                            sa_fpu *fpu,
                                            sa_initializer_service_fn service,
                                            void *context)
{
    uint32_t parameter, entity, target, value;
    push32(ram, cpu, cpu->r[14]);
    parameter = cpu->r[4]; cpu->r[14] = parameter;
    push32(ram, cpu, cpu->r[13]);
    cpu->r[3] = 2; push32(ram, cpu, cpu->pr);
    cpu->r[5] = read32(ram, SERVICE_DATA_LITERAL); cpu->r[6] = 4;
    target = read32(ram, SERVICE_LITERAL); cpu->r[2] = target; cpu->r[7] = 1;
    entity = read32(ram, cpu->r[14] + 0x20); cpu->r[13] = entity;
    push32(ram, cpu, cpu->r[3]);
    cpu->pr = 0x0c902796; cpu->pc = target; cpu->r[4] = cpu->r[14];
    (void)service(context, target, ram, cpu, fpu); cpu->pc = cpu->pr;

    cpu->r[2] = (uint32_t)-27;
    cpu->r[1] = read32(ram, CALLBACK_LITERAL);
    cpu->r[2] <<= 8;
    cpu->r[2] += (uint32_t)-96;
    cpu->r[3] = 0; write32(ram, cpu->r[13] + 0x0c, cpu->r[3]);
    cpu->r[0] = 64; write32(ram, cpu->r[13] + 0x10, cpu->r[2]);
    cpu->r[0] <<= 8; /* SHLL8 leaves SR.T unchanged. */
    cpu->r[2] = ENTITY_GLOBAL;
    write16(ram, cpu->r[13] + 4, cpu->r[0]);
    value = read32(ram, cpu->r[2]); cpu->r[3] = value;
    write32(ram, cpu->r[13] + 8, cpu->r[3]);
    write32(ram, parameter + 0x10, cpu->r[1]);
    cpu->r[1] = read32(ram, PARAM_CALLBACK_B_LITERAL);
    cpu->r[3] = read32(ram, PARAM_CALLBACK_A_LITERAL);
    write32(ram, parameter + 0x14, cpu->r[3]);
    write32(ram, parameter + 0x18, cpu->r[1]);

    cpu->r[15] += 4; cpu->pr = pop32(ram, cpu); cpu->r[13] = pop32(ram, cpu);
    cpu->pc = cpu->pr; cpu->r[14] = pop32(ram, cpu);
}
