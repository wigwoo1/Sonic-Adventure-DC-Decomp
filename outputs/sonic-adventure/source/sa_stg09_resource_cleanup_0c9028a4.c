#include <stdint.h>

typedef struct { uint32_t r[16], sr, pr, pc; } sa_cpu;
typedef struct { uint32_t banks[2][16], fpscr, fpul; } sa_fpu;
typedef int (*sa_cleanup_service_fn)(void *, uint32_t, uint8_t *, sa_cpu *, sa_fpu *);

#define RAM_BASE 0x0c000000u
#define ENTRY 0x0c9028a4u
#define SVC_SELECTOR_TARGET 0x0c900c50u
#define SVC_SELECTOR_ARG 0x0c900c54u
#define SVC_FREE_GLOBAL 0x0c902a44u
#define SVC_FREE_ENTITY 0x0c902a48u
#define SVC_FREE_FIELD_1C 0x0c902a4cu
#define SVC_FREE_FIELD_14 0x0c902a50u
#define SVC_RELEASE 0x0c902a58u
#define RELEASE_ZERO_ARG 0x0c902a5cu
#define RELEASE_NONZERO_ARG 0x0c902a54u
#define SVC_TAIL 0x0c902a60u


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
static void write8(uint8_t *m, uint32_t a, uint32_t v)
{ m[offset(a)]=(uint8_t)v; }
static void push32(uint8_t *m, sa_cpu *c, uint32_t v)
{ c->r[15]-=4u; write32(m,c->r[15],v); }
static uint32_t pop32(const uint8_t *m, sa_cpu *c)
{ uint32_t v=read32(m,c->r[15]); c->r[15]+=4u; return v; }
static void set_t(sa_cpu *c, uint32_t value)
{ c->sr=(c->sr&~1u)|(uint32_t)(value==0u); }
static void service_call(uint8_t *ram, sa_cpu *cpu, sa_fpu *fpu,
                         sa_cleanup_service_fn service, void *context,
                         uint32_t target, uint32_t return_pc)
{
    cpu->pr=return_pc; cpu->pc=target;
    (void)service(context,target,ram,cpu,fpu);
    cpu->pc=cpu->pr;
}

__declspec(dllexport)
void sa_stg09_resource_cleanup_0c9028a4(uint8_t *ram, sa_cpu *cpu,
                                        sa_fpu *fpu,
                                        sa_cleanup_service_fn service,
                                        void *context)
{
    uint32_t entity, value, target;
    uint32_t selector_target, selector_arg;
    int8_t reference_count;

    push32(ram,cpu,cpu->r[14]);
    push32(ram,cpu,cpu->pr);
    cpu->r[15]-=4u;
    entity=read32(ram,cpu->r[4]+0x20u);
    cpu->r[14]=entity;

    /* The internal 0x22 selector tail-dispatches through these module literals. */
    cpu->r[0]=0x22u;
    set_t(cpu,cpu->r[0]-12u);
    cpu->pc=0x0c900c20u;
    set_t(cpu,cpu->r[0]^0x22u);
    selector_target=read32(ram,SVC_SELECTOR_TARGET);
    selector_arg=read32(ram,SVC_SELECTOR_ARG);
    cpu->r[3]=selector_target;
    cpu->r[4]=selector_arg;
    service_call(ram,cpu,fpu,service,context,selector_target,0x0c9028b2u);

    target=read32(ram,SVC_FREE_GLOBAL);
    cpu->r[2]=target;
    cpu->r[4]=0;
    service_call(ram,cpu,fpu,service,context,target,0x0c9028b8u);

    target=read32(ram,SVC_FREE_ENTITY);
    cpu->r[3]=target;
    cpu->r[4]=0;
    cpu->r[5]=entity+0x20u;
    service_call(ram,cpu,fpu,service,context,target,0x0c9028c2u);

    value=read32(ram,entity+0x1cu);
    cpu->r[2]=value;
    set_t(cpu,cpu->r[2]);
    write32(ram,cpu->r[15],cpu->r[2]);
    if (value!=0u) {
        target=read32(ram,SVC_FREE_FIELD_1C);
        cpu->r[3]=target;
        cpu->r[4]=0;
        cpu->r[5]=read32(ram,cpu->r[15]);
        service_call(ram,cpu,fpu,service,context,target,0x0c9028d2u);
    }
    value=read32(ram,entity+0x14u);
    cpu->r[3]=value;
    set_t(cpu,value);
    write32(ram,cpu->r[15],cpu->r[3]);
    if (value!=0u) {
        target=read32(ram,SVC_FREE_FIELD_14);
        cpu->r[3]=target;
        cpu->r[4]=0;
        cpu->r[5]=read32(ram,cpu->r[15]);
        service_call(ram,cpu,fpu,service,context,target,0x0c9028e2u);
    }
    reference_count=(int8_t)ram[offset(entity)];
    cpu->r[3]=(uint32_t)(int32_t)reference_count;
    set_t(cpu,cpu->r[3]);
    if (reference_count==0) {
        target=read32(ram,SVC_RELEASE);
        cpu->r[2]=target;
        cpu->r[4]=read32(ram,RELEASE_ZERO_ARG);
        service_call(ram,cpu,fpu,service,context,target,0x0c902900u);
    } else {
        target=read32(ram,SVC_RELEASE);
        cpu->r[4]=read32(ram,RELEASE_NONZERO_ARG);
        cpu->r[3]=target;
        service_call(ram,cpu,fpu,service,context,target,0x0c9028f0u);
        cpu->r[2]=(uint32_t)(int32_t)(int8_t)ram[offset(entity)];
        cpu->r[2]=(cpu->r[2]-1u)&0xffffffffu;
        write8(ram,entity,cpu->r[2]);
    }

    cpu->r[4]=1u;
    cpu->r[15]+=4u;
    cpu->pr=pop32(ram,cpu);
    target=read32(ram,SVC_TAIL);
    cpu->r[3]=target;
    cpu->r[14]=pop32(ram,cpu);
    cpu->pc=target;
    (void)service(context,target,ram,cpu,fpu);
    cpu->pc=cpu->pr;
}
