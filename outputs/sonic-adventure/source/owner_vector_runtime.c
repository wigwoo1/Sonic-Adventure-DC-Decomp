#include "owner_heap_runtime.h"
#include "fpu_cleanup_transfer.h"
typedef uint32_t (*sa_owner_helper)(void *,uint32_t,uint32_t *);
typedef int (*sa_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t *);
int sa_model_vector_call(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t,void *,sa_instruction);
struct context{uint8_t *ram;struct sa_fpu_banks *f;uint32_t *fpul;uint32_t stack;void *user;sa_owner_helper helper;int status;};
static uint32_t helper(void *v,uint32_t arg,uint32_t *sr){struct context *c=v;return c->helper(c->user,arg,sr);}
static uint32_t detach(void *v,uint32_t first,uint32_t resource,uint32_t entity,uint32_t *sr){struct context *c=v;uint32_t r[16]={0};r[4]=first;r[5]=resource;r[6]=entity;r[15]=c->stack;c->status=sa_model_vector_call(c->ram,c->f,c->fpul,sr,r,0x8c000100u,0,0);return r[0];}
SA_STAGE_EXPORT uint32_t sa_owner_cleanup_with_vector(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *sr,uint32_t owner,uint32_t stack,void *user,sa_owner_helper external_helper,int *status){struct context c={ram,f,fpul,stack,user,external_helper,0};struct sa_owner_cleanup_services services={&c,0,helper,detach,0};uint32_t result=sa_owner_cleanup_with_heap(ram,sr,owner,&services);*status=c.status;return result;}
