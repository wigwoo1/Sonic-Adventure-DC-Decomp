#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
static uint32_t idx(uint32_t a){return (a&0x1fffffff)-0x0c000000u;}
static uint8_t rd8(const uint8_t*m,uint32_t a){return m[idx(a)];}
SA_EXPORT void sa_string_length_8c10d3f0(const uint8_t*ram,struct sa_cpu_state*s){
 uint32_t cursor=s->r[4],length=0u;uint8_t byte;
 do{byte=rd8(ram,cursor);cursor+=1u;if(byte)length+=1u;}while(byte!=0u);
 s->r[0]=length;s->r[3]=0u;s->r[4]=cursor;s->r[5]=length;s->sr|=1u;s->pc=s->pr;
}
