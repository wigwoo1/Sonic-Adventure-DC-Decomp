#include "tikal_heap_resource_runtime.h"
#include "fpu_cleanup_transfer.h"
uint32_t sa_owner_helper_terminal_native(uint8_t *,struct sa_fpu_banks *,uint32_t,uint32_t *);
typedef int (*sa_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t *);
int sa_model_vector_call(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t,void *,sa_instruction);
struct context{uint8_t *ram;struct sa_fpu_banks *f;uint32_t *fpul;uint32_t stack;int status;};
static uint32_t nested(void *v,uint32_t target,uint32_t argc,uint32_t a,uint32_t b,uint32_t d,uint32_t *sr){struct context *c=v;(void)argc;if(target==0x8c079476u)return sa_owner_helper_terminal_native(c->ram,c->f,a,sr);if(target==0x8c07a55cu){uint32_t r[16]={0};r[4]=a;r[5]=b;r[6]=d;r[15]=c->stack-64u;c->status=sa_model_vector_call(c->ram,c->f,c->fpul,sr,r,0x8c000100u,0,0);return r[0];}c->status=-3;return 0;}
SA_STAGE_EXPORT uint32_t sa_tikal_cleanup_native(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *sr,uint32_t task,uint32_t stack,int *status){struct context c={ram,f,fpul,stack,0};uint32_t result=sa_tikal_cleanup_with_heap_resources(ram,sr,task,stack,&c,nested);*status=c.status;return result;}
