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
/* Push node R5 onto the singly linked list whose head pointer is R4. */
SA_EXPORT void sa_linked_list_push_8c665466(uint8_t*ram,struct sa_cpu_state*s){
 uint32_t head_address=s->r[4],node=s->r[5],old_head=rd32(ram,head_address);
 s->r[3]=old_head;wr32(ram,node+4u,old_head);wr32(ram,head_address,node);s->sr=(s->sr&~1u)|(old_head==0u);s->pc=s->pr;
}

