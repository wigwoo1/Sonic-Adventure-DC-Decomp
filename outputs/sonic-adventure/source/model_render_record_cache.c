#include "stage00_task.h"
int sa_model_render_record_gate(uint8_t *,uint32_t *,uint32_t *,uint32_t *);
uint32_t sa_model_descriptor_lookup_8c056712(uint8_t *,uint32_t *,uint32_t,uint32_t,uint32_t);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+(a&0xffffffu);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
/* 8c07a800..8c07a832 cache integration, meaningful outputs r0/SR/cache
 * byte/branch. Native lookup; volatile callee registers/stack saves excluded. */
SA_STAGE_EXPORT int sa_model_render_record_cache(uint8_t *ram,uint32_t *sr,uint32_t config,uint32_t record,uint32_t frame,uint32_t *result,uint32_t *next){
 uint32_t r[16]={0};r[8]=config;r[14]=record;r[15]=frame;
 int status=sa_model_render_record_gate(ram,sr,r,next);if(status)return status;
 if(*next==0x8c07a826u){r[0]=sa_model_descriptor_lookup_8c056712(ram,sr,rd(ram,record),rd(ram,record+4u),frame);at(ram,record)[14]=(uint8_t)r[0];*next=0x8c07ade6u;}
 *result=r[0];return 0;
}
