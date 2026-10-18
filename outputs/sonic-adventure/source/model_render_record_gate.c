#include "stage00_task.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+(a&0xffffffu);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static uint32_t half(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8);}
/* Ownership/cache gate8c07a800..8c07a826. No repairs to mismatched records
 * or cache255: returns the original next branch instruction. */
SA_STAGE_EXPORT int sa_model_render_record_gate(uint8_t *ram,uint32_t *sr,uint32_t *r,uint32_t *next){
 r[0]=half(ram,r[8]+4u);r[3]=rd(ram,r[8]+32u);r[1]=rd(ram,r[14]);r[0]=r[3]+r[0]*16u;r[2]=rd(ram,r[0]);r[3]=rd(ram,r[2]);*sr=(*sr&~1u)|(r[1]==r[3]);
 if(!(*sr&1u)){*next=0x8c07ade6u;return 0;}
 r[0]=at(ram,r[14])[14];r[3]=255u;*sr=(*sr&~1u)|(r[0]==r[3]);*next=(*sr&1u)?0x8c07a826u:0x8c07a832u;return 0;
}
