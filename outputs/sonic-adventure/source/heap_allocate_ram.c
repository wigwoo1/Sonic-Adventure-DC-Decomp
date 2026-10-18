#include "stage00_task.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static int zero(uint32_t *sr,uint32_t v){*sr=(*sr&~1u)|(v==0u);return v==0u;}
SA_STAGE_EXPORT uint32_t sa_heap_allocate_ram_8c098a80(uint8_t *ram,uint32_t *sr,uint32_t size){
 if(zero(sr,size))return 0;
 uint32_t block=rd(ram,0x8c79f03cu);if(zero(sr,block))return 0;
 if(zero(sr,rd(ram,block+8u)))return 0;
 uint32_t total=(size+31u)&0xfffffff0u;zero(sr,block);
 for(;;){
  if(zero(sr,rd(ram,block+8u)))return 0;
  uint32_t available=rd(ram,block+8u)-block;*sr=(*sr&~1u)|(available==total);
  if(available==total){
   uint32_t next=rd(ram,block);int nz=zero(sr,next);uint32_t previous=rd(ram,block+4u);
   if(!nz)wr(ram,next+4u,previous);
   if(!zero(sr,previous))wr(ram,previous,next);else wr(ram,0x8c79f03cu,next);
  }else{
   *sr=(*sr&~1u)|(available>total);
   if(available<=total){block=rd(ram,block);if(zero(sr,block))return 0;if(zero(sr,rd(ram,block+8u)))return 0;continue;}
   uint32_t previous=rd(ram,block+4u),split=block+total,next=rd(ram,block);int nz=zero(sr,next);
   wr(ram,split,next);wr(ram,split+4u,previous);wr(ram,split+8u,rd(ram,block+8u));wr(ram,split+12u,block);
   wr(ram,rd(ram,block+8u)+12u,split);wr(ram,block+8u,split);
   if(!nz)wr(ram,next+4u,split);
   if(!zero(sr,previous))wr(ram,previous,split);else wr(ram,0x8c79f03cu,split);
  }
  wr(ram,block,0u);wr(ram,block+4u,0u);return block+16u;
 }
}
