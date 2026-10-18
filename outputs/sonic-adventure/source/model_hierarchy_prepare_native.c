#include "fpu_cleanup_transfer.h"
typedef int (*sa_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
typedef int (*sa_walk)(void *,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t);
int sa_model_hierarchy_prepare(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t,void *,sa_walk);
int sa_model_hierarchy_generator(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t,void *,sa_instruction);
struct context{void *user;sa_instruction instruction;uint32_t *fpul;};
static int walk(void *v,uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t pr){struct context *c=v;return sa_model_hierarchy_generator(ram,f,sr,r,c->fpul,pr,c->user,c->instruction);}
SA_STAGE_EXPORT int sa_model_hierarchy_prepare_native(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,uint32_t pr,void *user,sa_instruction instruction){struct context c={user,instruction,fpul};return sa_model_hierarchy_prepare(ram,f,sr,r,pr,&c,walk);}
