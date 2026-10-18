#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
static uint32_t idx(uint32_t a){return (a&0x1fffffff)-0x0c000000u;}
static uint8_t rd8(const uint8_t*m,uint32_t a){return m[idx(a)];}
SA_EXPORT void sa_nullable_signed_byte_get_8c10c970(const uint8_t*ram,struct sa_cpu_state*s){uint32_t object=s->r[4];s->r[0]=object?(uint32_t)(int32_t)(int8_t)rd8(ram,object+1u):0xfffffffdu;s->sr=(s->sr&~1u)|(object==0u);s->pc=s->pr;}
