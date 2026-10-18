#include "fpu_cleanup_transfer.h"
typedef int (*sa_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
typedef int (*sa_vector)(void *,uint32_t,struct sa_fpu_banks *);
int sa_model_hierarchy_channel_prepare(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,unsigned);
int sa_model_hierarchy_position_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,void *,sa_vector);
int sa_model_hierarchy_angle_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,unsigned,void *,sa_instruction);
int sa_model_hierarchy_scale_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
struct context{void *user;sa_instruction instruction;uint32_t *fpul;};
static int vector(void *v,uint32_t word,struct sa_fpu_banks *f){struct context *c=v;return c->instruction(c->user,word,f,*c->fpul);}
/* c80..dfe / ed0..808a, through per-channel increment boundary. */
SA_STAGE_EXPORT int sa_model_hierarchy_channel_nonempty(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,unsigned mode,void *user,sa_instruction instruction){
 int status=sa_model_hierarchy_channel_prepare(ram,f,sr,r,fpul,mode);if(status)return status;r[0]=r[9];*sr=(*sr&~1u)|(r[0]==0);if(!r[0]){if(!instruction)return -1;struct context c={user,instruction,fpul};return sa_model_hierarchy_position_native(ram,f,sr,r,&c,vector);}
 *sr=(*sr&~1u)|(r[0]==1);if(r[0]==1)return sa_model_hierarchy_angle_native(ram,f,sr,r,fpul,mode,user,instruction);if(mode==3){*sr=(*sr&~1u)|(r[0]==2);if(r[0]==2)return sa_model_hierarchy_scale_native(ram,f,sr,r);}return 0;
}
