#include <stdint.h>

typedef struct { uint32_t r[16], sr, pr, pc; } sa_cpu;
typedef struct { uint32_t banks[2][16], fpscr, fpul; } sa_fpu;
typedef int (*sa_initializer_service_fn)(void *, uint32_t, uint8_t *, sa_cpu *, sa_fpu *);

extern void sa_stg09_callback_initializer_0c90277c(
    uint8_t *ram, sa_cpu *cpu, sa_fpu *fpu,
    sa_initializer_service_fn service, void *context);

static uint32_t offset(uint32_t a) { return (a & 0x1fffffffu) - 0x0c000000u; }
static uint32_t read16(const uint8_t *m, uint32_t a)
{ uint32_t o = offset(a); return (uint32_t)m[o] | ((uint32_t)m[o+1] << 8); }
static uint32_t read32(const uint8_t *m, uint32_t a)
{
    uint32_t o = offset(a);
    return (uint32_t)m[o] | ((uint32_t)m[o+1] << 8) |
           ((uint32_t)m[o+2] << 16) | ((uint32_t)m[o+3] << 24);
}
static void write16(uint8_t *m, uint32_t a, uint32_t v)
{ uint32_t o = offset(a); m[o] = (uint8_t)v; m[o+1] = (uint8_t)(v >> 8); }
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
void sa_stg09_callback_initializer_flag_0c902860(uint8_t *ram, sa_cpu *cpu,
                                                  sa_fpu *fpu,
                                                  sa_initializer_service_fn service,
                                                  void *context)
{
    uint32_t parameter;
    uint32_t entity;
    uint32_t word;

    push32(ram, cpu, cpu->pr);
    cpu->r[15] -= 4; /* BSR argument slot */
    parameter = cpu->r[4];
    cpu->pr = 0x0c902868;
    cpu->pc = 0x0c90277c;
    write32(ram, cpu->r[15], parameter); /* BSR delay-slot MOV.L R4,@R15 */
    sa_stg09_callback_initializer_0c90277c(ram, cpu, fpu, service, context);

    cpu->r[3] = read32(ram, cpu->r[15]); /* MOV.L @R15,R3 has no postincrement. */
    cpu->r[2] = 2;
    cpu->r[2] <<= 8; /* SHLL8 does not affect SR.T. */
    entity = read32(ram, cpu->r[3] + 0x20u);
    cpu->r[3] = entity;
    word = read16(ram, entity + 4u);
    cpu->r[0] = word | cpu->r[2];
    write16(ram, entity + 4u, cpu->r[0]);

    cpu->r[15] += 4; /* release BSR argument slot */
    cpu->pr = pop32(ram, cpu);
    cpu->pc = cpu->pr;
    /* Retail RTS delay slot is NOP. */
}
