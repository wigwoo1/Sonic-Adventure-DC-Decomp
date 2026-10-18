#include "helper_record_release.h"
#include "heap_free_ram.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static uint32_t half(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(i*8));}
static void flag(uint32_t *sr,int v){*sr=(*sr&~1u)|(uint32_t)v;}
uint32_t sa_helper_record_release_8c07a49c(uint8_t *ram,uint32_t *sr,uint32_t config)
{
 uint32_t holder=rd(ram,config+0x28u),list=rd(ram,holder+4u);
 uint32_t mode=half(ram,list+10u),node=rd(ram,list);
 uint32_t descriptor=rd(ram,config+0x20u)+(half(ram,config+4u)<<4);
 uint32_t count=at(ram,descriptor)[4],bank_offset=mode<<2;
 uint32_t r0=count<128u?count:count|0xffffff00u;
 flag(sr,count>0u);
 if(count==0u)return r0;
 do {
  uint32_t pointer=rd(ram,node);flag(sr,pointer==0u);
  if(pointer!=0u){(void)sa_heap_free_ram_8c098b42(ram,sr,pointer,r0);r0=bank_offset;wr(ram,node,0u);wr(ram,node+r0,0u);}
  node+=4u;pointer=rd(ram,node);flag(sr,pointer==0u);
  if(pointer!=0u){(void)sa_heap_free_ram_8c098b42(ram,sr,pointer,r0);r0=bank_offset;wr(ram,node,0u);wr(ram,node+r0,0u);}
  r0=mode;flag(sr,r0==3u);node+=4u;
  if(r0==3u){
   pointer=rd(ram,node);flag(sr,pointer==0u);
   if(pointer!=0u){(void)sa_heap_free_ram_8c098b42(ram,sr,pointer,r0);r0=bank_offset;wr(ram,node,0u);wr(ram,node+r0,0u);}
   r0=mode;flag(sr,r0==3u);node+=r0==3u?16u:8u;
  }else node+=8u;
  count--;flag(sr,count>0u);
 }while(count>0u);
 return r0;
}
