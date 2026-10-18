#include "fpu_cleanup_transfer.h"
typedef int (*sa_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t *);
typedef int (*sa_hierarchy_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
int sa_model_render_hierarchy_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,void *,sa_hierarchy_instruction,uint32_t *);
int sa_model_render_record_loop(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t,void *,sa_instruction,uint32_t *,uint32_t *);
struct context{void *user;sa_instruction instruction;};
static int hierarchy_instruction(void *v,uint32_t word,struct sa_fpu_banks *f,uint32_t fpul){struct context *c=v;return c->instruction(c->user,word,f,&fpul);}
/* Behavior API for rendering a740..aeda; volatile callee registers excluded. */
SA_STAGE_EXPORT int sa_model_render_native(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *sr,uint32_t config,uint32_t record,uint32_t frame,uint32_t entity,void *user,sa_instruction instruction,uint32_t *result,uint32_t *end_record){uint32_t r[16]={0},next;r[8]=config;r[14]=record;r[15]=frame;r[10]=entity;struct context c={user,instruction};int status=sa_model_render_hierarchy_native(ram,f,sr,r,fpul,&c,hierarchy_instruction,&next);if(status)return status;if(next==0x8c07aedau){*result=r[0];*end_record=r[14];return 0;}return sa_model_render_record_loop(ram,f,fpul,sr,config,record,frame,entity,user,instruction,result,end_record);}
