#include "stage00_task.h"
uint32_t sa_heap_allocate_ram_8c098a80(uint8_t *,uint32_t *,uint32_t);
uint32_t sa_model_descriptor_lookup_8c056712(uint8_t *,uint32_t *,uint32_t,uint32_t,uint32_t);
uint32_t sa_model_vector_save(uint8_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static int zero(uint32_t *sr,uint32_t v){*sr=(*sr&~1u)|(v==0u);return v==0u;}
/* Original state-0 body 8c07a5a0..8c07a702; caller provides working stack.
 * Output is the record cursor at common epilogue, return r0 is always 1. */
SA_STAGE_EXPORT uint32_t sa_model_vector_initialize(uint8_t *ram,uint32_t *sr,uint32_t config,uint32_t record,uint32_t stack,uint32_t *last){
 if(zero(sr,rd(ram,record)))goto done;
 do{
  uint32_t type=at(ram,record)[13];
  if(type>=4u && type<=9u){
   wr(ram,record+16u,0u);uint32_t result=sa_model_descriptor_lookup_8c056712(ram,sr,rd(ram,record),rd(ram,record+4u),stack);
   at(ram,record)[14]=(uint8_t)result;at(ram,record)[15]=0u;
  }else if(type!=0u){
   uint32_t count=at(ram,record)[12];wr(ram,record+16u,sa_heap_allocate_ram_8c098a80(ram,sr,count*48u));
   uint32_t second=rd(ram,rd(ram,record+8u)+4u),first=rd(ram,rd(ram,record+4u)+4u);
   count=at(ram,record)[12];*sr=(*sr&~1u)|(count>0u);
   if(count)(void)sa_model_vector_save(ram,sr,record,first,second,0u);
   uint32_t result=sa_model_descriptor_lookup_8c056712(ram,sr,rd(ram,record),rd(ram,record+4u),stack);at(ram,record)[14]=(uint8_t)result;
   result=sa_model_descriptor_lookup_8c056712(ram,sr,rd(ram,record),rd(ram,record+8u),stack);at(ram,record)[15]=(uint8_t)result;
  }
  record+=24u;
 }while(!zero(sr,rd(ram,record)));
 done:at(ram,config)[2]=1u;at(ram,config)[3]=0u;*last=record;return 1u;
}
