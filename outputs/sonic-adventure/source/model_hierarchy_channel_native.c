#include "fpu_cleanup_transfer.h"
typedef int (*sa_vector_instruction)(void *,uint32_t,struct sa_fpu_banks *);
typedef int (*sa_transform)(void *,uint32_t,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_matrix_scale(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_matrix_translate(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,void *,sa_vector_instruction);
int sa_model_hierarchy_channel_fallback(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,unsigned,void *,sa_transform,uint32_t *);
struct context{void *user;sa_vector_instruction vector;sa_transform rotation;};
static int transform(void *v,uint32_t target,uint32_t pr,uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r){struct context *c=v;if(target==0x8c63a5ecu)return sa_model_matrix_scale(ram,f,sr,r);if(target==0x8c63a804u)return sa_model_matrix_translate(ram,f,sr,r,c->user,c->vector);if(!c->rotation)return -1;return c->rotation(c->user,target,pr,ram,f,sr,r);}
SA_STAGE_EXPORT int sa_model_hierarchy_channel_native(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,unsigned mode,void *user,sa_vector_instruction vector,sa_transform rotation,uint32_t *next){struct context c={user,vector,rotation};return sa_model_hierarchy_channel_fallback(ram,f,sr,r,mode,&c,transform,next);}
