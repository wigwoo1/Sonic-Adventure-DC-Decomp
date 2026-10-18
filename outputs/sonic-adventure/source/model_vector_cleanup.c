#include "heap_free_ram.h"
#include "stage00_task.h"
uint32_t sa_model_vector_restore(uint8_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(i*8));}
static int zero(uint32_t *sr,uint32_t v){*sr=(*sr&~1u)|(v==0u);return v==0u;}
/* 8c07adf6..8c07aeda: restore saved vectors and release each buffer.
 * Output record pointer and r0 at the common epilogue. */
SA_STAGE_EXPORT uint32_t sa_model_vector_cleanup(uint8_t *ram,uint32_t *sr,uint32_t record,uint32_t incoming,uint32_t *last_record){
 uint32_t r0=incoming;if(zero(sr,rd(ram,record)))goto done;
 do{
  if(!zero(sr,rd(ram,record+16u))){
   uint32_t second=rd(ram,rd(ram,record+8u)+4u),first=rd(ram,rd(ram,record+4u)+4u);
   r0=at(ram,record)[12];*sr=(*sr&~1u)|(r0>0u);
   if(r0)r0=sa_model_vector_restore(ram,sr,record,first,second,0u);
   r0=sa_heap_free_ram_8c098b42(ram,sr,rd(ram,record+16u),r0);wr(ram,record+16u,0u);
  }
  record+=24u;
 }while(!zero(sr,rd(ram,record)));
 done:*last_record=record;return r0;
}
