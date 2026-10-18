#include <stdint.h>

typedef struct { uint32_t r[16], sr, pr, pc; } sa_cpu;

#define RAM_BASE 0x0c000000u
#define THRESHOLD 0x0000c000u
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

__declspec(dllexport)
void sa_stg09_counter_step_0c905b06(uint8_t *ram, sa_cpu *cpu)
{
    uint32_t root=cpu->r[4];
    uint32_t flags_object=read32(ram,read32(ram,root+8u)+0x20u);
    uint32_t counter_object=read32(ram,root+0x20u);
    int32_t flags=(int16_t)read16(ram,flags_object+4u);

    cpu->r[2]=flags_object;
    cpu->r[3]=0x100u;
    cpu->r[0]=(uint32_t)flags;
    cpu->r[5]=counter_object;
    cpu->sr=(cpu->sr&~1u)|(uint32_t)(((uint32_t)flags&0x100u)==0u);
    if (cpu->sr&1u) {
        cpu->pc=cpu->pr;
        return;
    }

    cpu->r[1]=THRESHOLD;
    {
        uint32_t current=read32(ram,counter_object+0x10u);
        cpu->r[2]=current;
        cpu->sr=(cpu->sr&~1u)|(uint32_t)((int32_t)current>(int32_t)THRESHOLD);
        if (cpu->sr&1u) {
            cpu->r[2]=0x4000u;
            write32(ram,counter_object+0x10u,cpu->r[2]);
        } else {
            cpu->r[0]=current;
            cpu->r[2]=0x200u;
            cpu->r[0]=(cpu->r[0]+cpu->r[2])&0xffffffffu;
            write32(ram,counter_object+0x10u,cpu->r[0]);
        }
    }
    cpu->pc=cpu->pr;
}
