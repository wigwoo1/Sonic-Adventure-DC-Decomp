#include "linked_payload_release.h"
#include "payload_metadata.h"
#include "paired_allocation_release.h"
#include "block_unlink.h"
#include "block_pool_release.h"
static uint8_t *p(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *b=p(ram,a);return b[0]|((uint32_t)b[1]<<8)|((uint32_t)b[2]<<16)|((uint32_t)b[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)p(ram,a)[i]=(uint8_t)(v>>(8*i));}
static void t(uint32_t *sr,int yes){*sr=(*sr&~1u)|(yes?1u:0u);}
uint32_t sa_linked_payload_release_8c663950(uint8_t *ram,uint32_t *sr,uint32_t config,uint32_t scratch)
{
 (void)sa_payload_metadata_8c6652c0(ram,sr,scratch,config);
 uint32_t current=rd(ram,0x8c67c308u),found=0u,slot=0u;
 uint32_t count=rd(ram,scratch+16u);
 while(current){
  uint32_t header=p(ram,current)[0]|((uint32_t)p(ram,current)[1]<<8);
  if((header&0xf000u)==rd(ram,scratch)){
   for(uint32_t index=0;index<count;index++){
    if(rd(ram,current+12u+index*4u)!=0u){
     uint32_t displacement=((rd(ram,scratch+4u)>>2)*index)<<2;
     if(rd(ram,current+8u)+displacement==rd(ram,config+0x1cu)){
      found=current;slot=index;break;
     }
    }
   }
  }
  if(found)break;
  current=rd(ram,current+4u);
 }
 t(sr,found==0u);if(found==0u)return 7u;
 wr(ram,found+12u+slot*4u,0u);
 uint32_t occupied=0u;count=rd(ram,scratch+16u);
 for(uint32_t index=0;index<count;index++)if(rd(ram,found+12u+index*4u)!=0u)occupied=1u;
 t(sr,occupied==0u);if(occupied)return 0u;
 uint32_t result=sa_paired_allocation_release_8c66385a(ram,sr,rd(ram,found+8u),scratch-20u);
 t(sr,result==0u);if(result)return result;
 uint32_t predecessor=sa_block_unlink_8c665478(ram,sr,0x8c67c308u,found);
 (void)sa_block_pool_release_8c665440(ram,sr,found,predecessor);
 return 0u;
}
