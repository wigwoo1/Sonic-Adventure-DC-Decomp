#include "fpu_cleanup_transfer.h"
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
/* Original cc4..cf6 / ee8..f1a; FPUL is the already converted frame.
 * Retains original lookahead read and unsigned comparisons. */
SA_STAGE_EXPORT int sa_model_hierarchy_keyframe_select(uint8_t *ram,uint32_t *sr,uint32_t *r,uint32_t fpul,unsigned mode,uint32_t *next){
 if(mode!=2&&mode!=3)return -1;r[4]=rd(ram,r[8]+r[7]);r[6]=fpul;r[5]=r[4];r[3]=rd(ram,r[4]);*sr=(*sr&~1u)|(r[6]>r[3]);r[5]+=16u;
 if(*sr&1u){r[1]=rd(ram,r[5]);*sr=(*sr&~1u)|(r[6]>=r[1]);if(*sr&1u)for(;;){r[4]+=16u;r[2]=rd(ram,r[4]);*sr=(*sr&~1u)|(r[6]>r[2]);r[5]+=16u;if(!(*sr&1u))break;r[1]=rd(ram,r[5]);*sr=(*sr&~1u)|(r[6]>=r[1]);if(!(*sr&1u))break;}}
 r[2]=rd(ram,r[14]+12u);r[3]=rd(ram,r[4]);r[2]--;*sr=(*sr&~1u)|(r[3]==r[2]);*next=mode==2?((*sr&1u)?0x8c057cf6u:0x8c057d16u):((*sr&1u)?0x8c057f1au:0x8c057f3au);return 0;
}
