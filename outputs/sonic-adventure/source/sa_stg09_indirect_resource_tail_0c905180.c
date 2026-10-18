#include <stdint.h>

typedef struct { uint32_t r[16], sr, pr, pc; } sa_cpu;
typedef struct { uint32_t banks[2][16], fpscr, fpul; } sa_fpu;
typedef int (*sa_tail_service_fn)(void *, uint32_t, uint8_t *, sa_cpu *, sa_fpu *);

#define RAM_BASE 0x0c000000u
#define FIRST_TARGET_LITERAL 0x0c905300u
#define TAIL_TARGET_LITERAL 0x0c905304u

static uint32_t offset(uint32_t a) { return (a & 0x1fffffffu) - RAM_BASE; }
static uint32_t read32(const uint8_t *m, uint32_t a)
{
    uint32_t o=offset(a);
    return (uint32_t)m[o] | ((uint32_t)m[o+1]<<8) |
           ((uint32_t)m[o+2]<<16) | ((uint32_t)m[o+3]<<24);
}
static void write32(uint8_t *m, uint32_t a, uint32_t v)
{
    uint32_t o=offset(a); m[o]=(uint8_t)v; m[o+1]=(uint8_t)(v>>8);
    m[o+2]=(uint8_t)(v>>16); m[o+3]=(uint8_t)(v>>24);
}
static void push32(uint8_t *m, sa_cpu *c, uint32_t v)
{ c->r[15]-=4u; write32(m,c->r[15],v); }
static uint32_t pop32(const uint8_t *m, sa_cpu *c)
{ uint32_t v=read32(m,c->r[15]); c->r[15]+=4u; return v; }
static void call_service(uint8_t *ram, sa_cpu *cpu, sa_fpu *fpu,
                         sa_tail_service_fn service, void *context,
                         uint32_t target, uint32_t return_pc)
{
    cpu->pr=return_pc; cpu->pc=target;
    (void)service(context,target,ram,cpu,fpu);
    cpu->pc=cpu->pr;
}

__declspec(dllexport)
void sa_stg09_indirect_resource_tail_0c905180(uint8_t *ram, sa_cpu *cpu,
                                              sa_fpu *fpu,
                                              sa_tail_service_fn service,
                                              void *context)
{
    uint32_t object=cpu->r[4], entity, target;
    push32(ram,cpu,cpu->r[14]);
    push32(ram,cpu,cpu->pr);
    entity=read32(ram,object+0x20u);
    cpu->r[14]=entity;
    target=read32(ram,FIRST_TARGET_LITERAL);
    cpu->r[3]=target;
    cpu->r[14]=read32(ram,entity+8u);
    cpu->r[5]=cpu->r[14];
    call_service(ram,cpu,fpu,service,context,target,0x0c90518eu);

    cpu->pr=pop32(ram,cpu);
    target=read32(ram,TAIL_TARGET_LITERAL);
    cpu->r[2]=target;
    cpu->r[4]=cpu->r[14];
    cpu->r[14]=pop32(ram,cpu);
    cpu->pc=target;
    (void)service(context,target,ram,cpu,fpu);
    cpu->pc=cpu->pr;
}
