#include <stdint.h>

typedef struct { uint32_t r[16], sr, pr, pc; } sa_cpu;
typedef struct { uint32_t banks[2][16], fpscr, fpul; } sa_fpu;
typedef int (*sa_cleanup_service_fn)(void *, uint32_t, uint8_t *, sa_cpu *, sa_fpu *);

#define RAM_BASE 0x0c000000u
#define TARGET_SELECTOR 0x0c900c50u
#define ARG_SELECTOR 0x0c900c54u
#define TARGET_GLOBAL_FREE 0x0c902e4cu
#define TARGET_ENTITY_FREE 0x0c902e50u
#define TARGET_FIELD_1C 0x0c902e54u
#define TARGET_FIELD_18 0x0c902e58u
#define TARGET_FIELD_14 0x0c902e5cu
#define TARGET_RELEASE 0x0c902e60u
#define RELEASE_FLAG_OFF_NONZERO 0x0c902e64u
#define RELEASE_FLAG_OFF_ZERO 0x0c902e68u
#define RELEASE_FLAG_ON_NONZERO 0x0c902e6cu
#define RELEASE_FLAG_ON_ZERO 0x0c902e70u
#define TARGET_TAIL 0x0c902e74u

static uint32_t offset(uint32_t a) { return (a & 0x1fffffffu) - RAM_BASE; }
static uint32_t read32(const uint8_t *m, uint32_t a)
{
    uint32_t o=offset(a);
    return (uint32_t)m[o] | ((uint32_t)m[o+1]<<8) |
           ((uint32_t)m[o+2]<<16) | ((uint32_t)m[o+3]<<24);
}
static uint16_t read16(const uint8_t *m, uint32_t a)
{
    uint32_t o=offset(a);
    return (uint16_t)((uint32_t)m[o] | ((uint32_t)m[o+1]<<8));
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
void sa_stg09_callback_cleanup_0c902d58(uint8_t *ram, sa_cpu *cpu,
                                        sa_fpu *fpu,
                                        sa_cleanup_service_fn service,
                                        void *context)
{
    uint32_t entity, value, target, flags, release_arg;
    int8_t reference_count;

    push32(ram,cpu,cpu->r[14]);
    push32(ram,cpu,cpu->r[13]);
    push32(ram,cpu,cpu->pr);
    cpu->r[15]-=4u;
    entity=read32(ram,cpu->r[4]+0x20u);
    cpu->r[14]=entity;

    /* Inline the recovered 0x0c900c14 selector's 0x22 tail-dispatch. */
    cpu->r[0]=0x22u;
    set_t(cpu,cpu->r[0]-12u);
    cpu->pc=0x0c900c20u;
    set_t(cpu,cpu->r[0]^0x22u);
    target=read32(ram,TARGET_SELECTOR);
    cpu->r[3]=target;
    cpu->r[4]=read32(ram,ARG_SELECTOR);
    service_call(ram,cpu,fpu,service,context,target,0x0c902d68u);

    target=read32(ram,TARGET_GLOBAL_FREE);
    cpu->r[2]=target;
    cpu->r[4]=0;
    service_call(ram,cpu,fpu,service,context,target,0x0c902d6eu);

    target=read32(ram,TARGET_ENTITY_FREE);
    cpu->r[3]=target;
    cpu->r[4]=0;
    cpu->r[5]=entity+0x20u;
    service_call(ram,cpu,fpu,service,context,target,0x0c902d78u);

    value=read32(ram,entity+0x1cu);
    cpu->r[2]=value;
    set_t(cpu,cpu->r[2]);
    write32(ram,cpu->r[15],cpu->r[2]);
    if (value!=0u) {
        target=read32(ram,TARGET_FIELD_1C);
        cpu->r[3]=target;
        cpu->r[4]=0;
        cpu->r[5]=read32(ram,cpu->r[15]);
        service_call(ram,cpu,fpu,service,context,target,0x0c902d88u);
    }

    value=read32(ram,entity+0x18u);
    cpu->r[3]=value;
    set_t(cpu,value);
    write32(ram,cpu->r[15],cpu->r[3]);
    if (value!=0u) {
        target=read32(ram,TARGET_FIELD_18);
        cpu->r[3]=target;
        cpu->r[4]=0;
        cpu->r[5]=read32(ram,cpu->r[15]);
        service_call(ram,cpu,fpu,service,context,target,0x0c902d98u);
    }

    value=read32(ram,entity+0x14u);
    cpu->r[3]=value;
    set_t(cpu,value);
    write32(ram,cpu->r[15],cpu->r[3]);
    if (value!=0u) {
        target=read32(ram,TARGET_FIELD_14);
        cpu->r[3]=target;
        cpu->r[4]=0;
        cpu->r[5]=read32(ram,cpu->r[15]);
        service_call(ram,cpu,fpu,service,context,target,0x0c902da8u);
    }

    cpu->r[3]=2u;
    cpu->r[0]=(uint32_t)(int32_t)(int16_t)read16(ram,entity+4u);
    cpu->r[3]<<=8;
    flags=cpu->r[0];
    set_t(cpu,cpu->r[3]&flags);
    cpu->r[13]=read32(ram,TARGET_RELEASE);
    reference_count=(int8_t)ram[offset(entity+1u)];
    cpu->r[0]=(uint32_t)(int32_t)reference_count;
    set_t(cpu,cpu->r[0]);

    if ((flags&0x200u)==0u) {
        if (reference_count==0) {
            release_arg=read32(ram,RELEASE_FLAG_OFF_ZERO);
            cpu->r[4]=release_arg;
            service_call(ram,cpu,fpu,service,context,cpu->r[13],0x0c902dc6u);
        } else {
            release_arg=read32(ram,RELEASE_FLAG_OFF_NONZERO);
            cpu->r[4]=release_arg;
            service_call(ram,cpu,fpu,service,context,cpu->r[13],0x0c902dd6u);
            cpu->r[0]=(uint32_t)(int32_t)(int8_t)ram[offset(entity+1u)];
            cpu->r[0]=(cpu->r[0]-1u)&0xffffffffu;
            write8(ram,entity+1u,cpu->r[0]);
        }
    } else if (reference_count==0) {
        release_arg=read32(ram,RELEASE_FLAG_ON_ZERO);
        cpu->r[4]=release_arg;
        write8(ram,entity+1u,cpu->r[0]);
        service_call(ram,cpu,fpu,service,context,cpu->r[13],0x0c902de4u);
    } else {
        release_arg=read32(ram,RELEASE_FLAG_ON_NONZERO);
        cpu->r[4]=release_arg;
        service_call(ram,cpu,fpu,service,context,cpu->r[13],0x0c902dd6u);
        cpu->r[0]=(uint32_t)(int32_t)(int8_t)ram[offset(entity+1u)];
        cpu->r[0]=(cpu->r[0]-1u)&0xffffffffu;
        write8(ram,entity+1u,cpu->r[0]);
    }

    cpu->r[4]=1u;
    cpu->r[15]+=4u;
    cpu->pr=pop32(ram,cpu);
    cpu->r[13]=pop32(ram,cpu);
    target=read32(ram,TARGET_TAIL);
    cpu->r[2]=target;
    cpu->r[14]=pop32(ram,cpu);
    cpu->pc=target;
    (void)service(context,target,ram,cpu,fpu);
    cpu->pc=cpu->pr;
}
