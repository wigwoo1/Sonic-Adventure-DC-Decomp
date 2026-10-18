#include "fpu_cleanup_transfer.h"
typedef int (*sa_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
int sa_model_hierarchy_angle_interpolate(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,unsigned);
int sa_model_matrix_rotate(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,void *,sa_instruction);
/* Both angle channel bodies through their common increment boundary. */
SA_STAGE_EXPORT int sa_model_hierarchy_angle_native(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,unsigned mode,void *ctx,sa_instruction instruction){int status=sa_model_hierarchy_angle_interpolate(ram,f,r,fpul,mode);if(status)return status;r[4]=0;return sa_model_matrix_rotate(ram,f,sr,r,fpul,ctx,instruction);}
