#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
static uint32_t idx(uint32_t a){return (a&0x1fffffffu)-0x0c000000u;}
static uint32_t rd32(const uint8_t*m,uint32_t a){uint32_t i=idx(a);return (uint32_t)m[i]|((uint32_t)m[i+1]<<8)|((uint32_t)m[i+2]<<16)|((uint32_t)m[i+3]<<24);}
static uint8_t rd8(const uint8_t*m,uint32_t a){return m[idx(a)];}
SA_EXPORT void sa_byte_table_bit_test_8c067d84(const uint8_t*ram,struct sa_cpu_state*s){
 uint32_t table=rd32(ram,0x8c067ddcu);
 uint8_t value=rd8(ram,table+s->r[4]);
 uint32_t set=(uint32_t)(value&1u);
 s->r[0]=set;s->sr=(s->sr&~1u)|(set^1u);s->pc=s->pr;
}
