#include "fpu_cleanup_transfer.h"
typedef int (*sa_vector_instruction)(void *,uint32_t,struct sa_fpu_banks *);
int sa_model_hierarchy_position_interpolate(uint8_t *,struct sa_fpu_banks *,uint32_t *);
/* Implicit branch of register-input translation8c63a878..8c63a88a. */
SA_STAGE_EXPORT int sa_model_matrix_translate_registers_implicit(struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,void *ctx,sa_vector_instruction vector){
 if(f->fpscr&0x180000u||r[4]||!vector)return -1;unsigned bank=(f->fpscr>>21)&1u;*sr=(*sr&~1u)|1u;f->bank[bank][7]=0x3f800000u;int status=vector(ctx,0xf5fdu,f);if(status)return status;for(unsigned i=0;i<4;i++)f->bank[bank^1u][12u+i]=f->bank[bank][4u+i];return 0;
}
/* Both interpolated position channels through common increment boundary. */
SA_STAGE_EXPORT int sa_model_hierarchy_position_native(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,void *ctx,sa_vector_instruction vector){int status=sa_model_hierarchy_position_interpolate(ram,f,r);if(status)return status;r[4]=0;return sa_model_matrix_translate_registers_implicit(f,sr,r,ctx,vector);}
