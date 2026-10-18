#include "fpu_cleanup_transfer.h"
typedef int (*sa_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
int sa_model_render_prepare(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_hierarchy_prepare_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t,void *,sa_instruction);
int sa_model_render_frame_setup(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
/* Rendering a740 through a800/aeda, native hierarchy generation. */
SA_STAGE_EXPORT int sa_model_render_hierarchy_native(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,void *ctx,sa_instruction instruction,uint32_t *next){int status=sa_model_render_prepare(ram,f,sr,r);if(status)return status;status=sa_model_hierarchy_prepare_native(ram,f,sr,r,fpul,0x8c07a786u,ctx,instruction);if(status)return status;return sa_model_render_frame_setup(ram,f,sr,r,next);}
