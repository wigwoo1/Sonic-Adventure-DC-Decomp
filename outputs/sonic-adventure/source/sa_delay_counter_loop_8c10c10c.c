#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
struct sa_cpu_state { uint32_t r[16], sr, pr, pc; };
static uint32_t idx(uint32_t a){return (a&0x1fffffffu)-0x0c000000u;}
static uint32_t rd32(const uint8_t*m,uint32_t a){uint32_t i=idx(a);return (uint32_t)m[i]|((uint32_t)m[i+1]<<8)|((uint32_t)m[i+2]<<16)|((uint32_t)m[i+3]<<24);}
/* Preserve the retail decrement-to-zero delay loop and its final CPU state. */
SA_EXPORT void sa_delay_counter_loop_8c10c10c(const uint8_t*ram,struct sa_cpu_state*s){
 volatile uint32_t count=rd32(ram,0x8c10c1bcu);
 do { count-=1u; } while(count!=0u);
 s->r[4]=count;s->sr|=1u;s->pc=s->pr;
}
