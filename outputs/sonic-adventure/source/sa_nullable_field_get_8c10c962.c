#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
static uint32_t idx(uint32_t a){return (a&0x1fffffff)-0x0c000000u;}
static uint32_t rd32(const uint8_t*m,uint32_t a){uint32_t i=idx(a);return (uint32_t)m[i]|((uint32_t)m[i+1]<<8)|((uint32_t)m[i+2]<<16)|((uint32_t)m[i+3]<<24);}
SA_EXPORT void sa_nullable_field_get_8c10c962(const uint8_t*ram,struct sa_cpu_state*s){uint32_t object=s->r[4];s->r[0]=object?rd32(ram,object+0x24u):0xfffffffdu;s->sr=(s->sr&~1u)|(object==0u);s->pc=s->pr;}

