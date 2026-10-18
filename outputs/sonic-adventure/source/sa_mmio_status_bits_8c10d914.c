#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
static uint32_t idx(uint32_t a){return (a&0x1fffffff)-0x0c000000u;}
static uint32_t rd32(const uint8_t*m,uint32_t a){uint32_t i=idx(a);return (uint32_t)m[i]|((uint32_t)m[i+1]<<8)|((uint32_t)m[i+2]<<16)|((uint32_t)m[i+3]<<24);}
static void wr32(uint8_t*m,uint32_t a,uint32_t v){uint32_t i=idx(a);m[i]=(uint8_t)v;m[i+1]=(uint8_t)(v>>8);m[i+2]=(uint8_t)(v>>16);m[i+3]=(uint8_t)(v>>24);}
typedef uint32_t (*sa_mmio_read16_fn)(void*,uint32_t);
SA_EXPORT void sa_mmio_status_bits_8c10d914(uint8_t*ram,struct sa_cpu_state*s,void*context,sa_mmio_read16_fn read_register){
 uint32_t saved_pr=s->pr;wr32(ram,s->r[15]-4u,saved_pr);
 uint32_t address=rd32(ram,0x8c10d934u);uint32_t raw=(uint32_t)(int32_t)(int16_t)read_register(context,address);uint32_t masked=((uint32_t)(int32_t)(int16_t)0x0300u & 0xffffu)&(raw&0xffffu);
 s->r[2]=0x0300u;s->r[3]=address;s->r[4]=masked;s->pr=saved_pr;s->r[4]=3u;s->r[0]=((masked>>8)&3u);s->r[4]=s->r[0];s->pc=s->pr;
}
