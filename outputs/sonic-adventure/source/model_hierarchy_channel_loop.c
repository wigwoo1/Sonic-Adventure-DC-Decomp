#include "fpu_cleanup_transfer.h"
typedef int (*sa_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
int sa_model_hierarchy_channel_nonempty(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,unsigned,void *,sa_instruction);
int sa_model_hierarchy_channel_transforms(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,unsigned,void *,sa_instruction,uint32_t *);
int sa_model_matrix_scale(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)ram[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
/* Layout channel loops c30/e1c through80a0. Original signed loop tests. */
SA_STAGE_EXPORT int sa_model_hierarchy_channel_loop(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,unsigned mode,void *ctx,sa_instruction instruction){
 if(mode!=2&&mode!=3)return -1;r[8]=rd(ram,r[14]);r[9]=0;
 for(;;){if(mode==3)r[3]=3;uint32_t limit=mode==2?r[10]:r[3];*sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)limit);if(*sr&1u)break;
 r[7]=r[9];unsigned ptrreg=mode==2?2:3;r[ptrreg]=r[8];r[7]<<=2;r[ptrreg]+=r[7];unsigned valuereg=mode==2?3:2;r[valuereg]=rd(ram,r[ptrreg]);*sr=(*sr&~1u)|(r[valuereg]==0);int status;
 if(r[valuereg])status=sa_model_hierarchy_channel_nonempty(ram,f,sr,r,fpul,mode,ctx,instruction);else{uint32_t next;status=sa_model_hierarchy_channel_transforms(ram,f,sr,r,fpul,mode,ctx,instruction,&next);}if(status)return status;r[9]++;
 }
 if(mode==2){r[0]=rd(ram,r[13]);*sr=(*sr&~1u)|((r[0]&4u)==0);if(*sr&1u){r[2]=0x8c63a5ecu;r[5]=r[13]+32u;r[4]=0;int status=sa_model_matrix_scale(ram,f,sr,r);if(status)return status;}r[8]+=16u;}else r[8]+=24u;wr(ram,r[14],r[8]);return 0;
}
