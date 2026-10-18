#include "fpu_cleanup_transfer.h"
typedef int (*sa_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
typedef int (*sa_child)(void *,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t);
int sa_model_hierarchy_generator_entry(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t,uint32_t *);
int sa_model_hierarchy_channel_dispatch(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
int sa_model_hierarchy_channel_loop(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,unsigned,void *,sa_instruction);
int sa_model_hierarchy_generator_tail(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,void *,sa_child);
struct context{void *user;sa_instruction instruction;uint32_t *fpul;};
static int walk(void *v,uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t pr){struct context *c=v;uint32_t next;int status=sa_model_hierarchy_generator_entry(ram,f,sr,r,pr,&next);if(status||!next)return status;status=sa_model_hierarchy_channel_dispatch(ram,f,sr,r,&next);if(status)return status;
 /* Original unsupported layout spins at809c; report unresolved execution. */
 if(next==0x8c05809cu)return -2;unsigned mode=r[0];status=sa_model_hierarchy_channel_loop(ram,f,sr,r,c->fpul,mode,c->user,c->instruction);if(status)return status;return sa_model_hierarchy_generator_tail(ram,f,sr,r,c,walk);}
SA_STAGE_EXPORT int sa_model_hierarchy_generator(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,uint32_t pr,void *user,sa_instruction instruction){struct context c={user,instruction,fpul};return walk(&c,ram,f,sr,r,pr);}
