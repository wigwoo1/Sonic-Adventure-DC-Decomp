#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
static uint32_t idx(uint32_t a) { return (a & 0x1fffffffu) - 0x0c000000u; }
static uint32_t rd32(const uint8_t *m, uint32_t a) { uint32_t i=idx(a); return (uint32_t)m[i]|((uint32_t)m[i+1]<<8)|((uint32_t)m[i+2]<<16)|((uint32_t)m[i+3]<<24); }
static void wr32(uint8_t *m, uint32_t a, uint32_t v) { uint32_t i=idx(a); m[i]=(uint8_t)v; m[i+1]=(uint8_t)(v>>8); m[i+2]=(uint8_t)(v>>16); m[i+3]=(uint8_t)(v>>24); }
/* Relocate the nonzero pointer at +4 in each 8-byte record. */
SA_EXPORT void sa_pair_field_relocate_8c09f9da(uint8_t *ram, struct sa_cpu_state *s)
{
    uint32_t base=s->r[4], cursor=s->r[5], end=s->r[6], processed=0u;
    s->r[7]=cursor;
    while (cursor < end) {
        uint32_t value=rd32(ram,cursor+4u);
        if (value) { value+=base; wr32(ram,cursor+4u,value); s->r[3]=value; }
        processed+=1u; s->r[2]=processed;
        cursor+=8u; s->r[7]=cursor;
    }
    s->sr=(s->sr & ~1u) | (cursor >= end);
    s->pc=s->pr;
}
