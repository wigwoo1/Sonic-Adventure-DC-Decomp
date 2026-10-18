#include "fpu_cleanup_transfer.h"
int sa_model_hierarchy_frame_convert(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,unsigned);
int sa_model_hierarchy_keyframe_select(uint8_t *,uint32_t *,uint32_t *,uint32_t,unsigned,uint32_t *);
int sa_model_hierarchy_factor(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,unsigned);
/* Nonempty channel c80..d46 / ed0..f6a, before value dispatch. */
SA_STAGE_EXPORT int sa_model_hierarchy_channel_prepare(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,unsigned mode){int status=sa_model_hierarchy_frame_convert(ram,f,sr,r,fpul,mode);if(status)return status;uint32_t next;status=sa_model_hierarchy_keyframe_select(ram,sr,r,*fpul,mode,&next);if(status)return status;return sa_model_hierarchy_factor(ram,f,sr,r,fpul,*sr&1u);}
