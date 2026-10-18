#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
static uint32_t idx(uint32_t a){return (a&0x1fffffffu)-0x0c000000u;}
static uint32_t rd32(const uint8_t*m,uint32_t a){uint32_t i=idx(a);return (uint32_t)m[i]|((uint32_t)m[i+1]<<8)|((uint32_t)m[i+2]<<16)|((uint32_t)m[i+3]<<24);}
static void wr32(uint8_t*m,uint32_t a,uint32_t v){uint32_t i=idx(a);m[i]=(uint8_t)v;m[i+1]=(uint8_t)(v>>8);m[i+2]=(uint8_t)(v>>16);m[i+3]=(uint8_t)(v>>24);}
SA_EXPORT void sa_masked_array_store_8c6611a8(uint8_t*ram,struct sa_cpu_state*s){
 uint32_t mask=rd32(ram,0x8c661290u);uint32_t enabled=s->r[4]&mask;
 s->r[3]=mask;s->sr=(s->sr&~1u)|(enabled==0u);
 if(enabled){uint32_t offset=s->r[5]*4u;s->r[1]=offset;uint32_t base=rd32(ram,0x8c661298u);uint32_t address=base+offset;wr32(ram,address,s->r[6]);s->r[5]+=1u;}
 s->r[0]=s->r[5];s->pc=s->pr;
}

