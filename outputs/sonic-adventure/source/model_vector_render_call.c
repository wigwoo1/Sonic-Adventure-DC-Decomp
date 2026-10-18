#include "fpu_cleanup_transfer.h"
typedef int (*sa_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t *);
int sa_model_vector_frame_enter(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t,uint32_t *);
int sa_model_vector_frame_leave(uint8_t *,struct sa_fpu_banks *,uint32_t *);
int sa_model_render_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t,void *,sa_instruction,uint32_t *,uint32_t *);
/* Complete state1 call behavior. Volatile integer scratch registers excluded
 * from this API's fidelity claim; init/cleanup modes return unsupported. */
SA_STAGE_EXPORT int sa_model_vector_render_call(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *sr,uint32_t *r,uint32_t pr,void *ctx,sa_instruction instruction){uint32_t next;int status=sa_model_vector_frame_enter(ram,f,sr,r,pr,&next);if(status)return status;if(next!=0x8c07a740u)return -2;uint32_t result,end;status=sa_model_render_native(ram,f,fpul,sr,r[8],r[14],r[15],r[10],ctx,instruction,&result,&end);if(status)return status;r[0]=result;r[14]=end;return sa_model_vector_frame_leave(ram,f,r);}
