#include "fpu_cleanup_transfer.h"
typedef int (*sa_vector)(void *,uint32_t,struct sa_fpu_banks *);
typedef int (*sa_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
typedef int (*sa_transform)(void *,uint32_t,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_matrix_rotate(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,void *,sa_instruction);
int sa_model_hierarchy_channel_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,unsigned,void *,sa_vector,sa_transform,uint32_t *);
struct context{void *user;sa_instruction instruction;uint32_t *fpul;};
static int vector(void *v,uint32_t word,struct sa_fpu_banks *f){struct context *c=v;return c->instruction(c->user,word,f,*c->fpul);}
static int rotation(void *v,uint32_t target,uint32_t pr,uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r){struct context *c=v;(void)pr;if(target!=0x8c639cf4u)return -1;return sa_model_matrix_rotate(ram,f,sr,r,c->fpul,c->user,c->instruction);}
SA_STAGE_EXPORT int sa_model_hierarchy_channel_transforms(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,unsigned mode,void *user,sa_instruction instruction,uint32_t *next){if(!instruction)return -1;struct context c={user,instruction,fpul};return sa_model_hierarchy_channel_native(ram,f,sr,r,mode,&c,vector,rotation,next);}
