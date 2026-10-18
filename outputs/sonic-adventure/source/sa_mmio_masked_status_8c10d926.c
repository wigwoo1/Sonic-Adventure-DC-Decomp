#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
static uint32_t idx(uint32_t a){return (a&0x1fffffffu)-0x0c000000u;}
static uint32_t rd32(const uint8_t*m,uint32_t a){uint32_t i=idx(a);return (uint32_t)m[i]|((uint32_t)m[i+1]<<8)|((uint32_t)m[i+2]<<16)|((uint32_t)m[i+3]<<24);}
static uint16_t rd16(const uint8_t*m,uint32_t a){uint32_t i=idx(a);return (uint16_t)((uint16_t)m[i]|((uint16_t)m[i+1]<<8));}
typedef uint32_t (*sa_mmio_read16_fn)(void*,uint32_t);
SA_EXPORT void sa_mmio_masked_status_8c10d926(const uint8_t*ram,struct sa_cpu_state*s,void*context,sa_mmio_read16_fn read_register){
 uint32_t address=rd32(ram,0x8c10d934u);uint32_t mask=(uint32_t)(int32_t)(int16_t)rd16(ram,0x8c10d932u);uint32_t value=(uint32_t)(int32_t)(int16_t)read_register(context,address);
 s->r[2]=mask;s->r[3]=address;s->r[4]=value;s->r[0]=(value&0xffffu)&mask;s->pc=s->pr;
}
