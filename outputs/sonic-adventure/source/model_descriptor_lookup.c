#include "stage00_task.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static int flag(uint32_t *sr,int v){*sr=(*sr&~1u)|(uint32_t)v;return v;}
SA_STAGE_EXPORT uint32_t sa_model_descriptor_search_8c056738(uint8_t *ram,uint32_t *sr,uint32_t node,uint32_t target,uint32_t counter){
 if(flag(sr,node==target))return target;
 wr(ram,counter,rd(ram,counter)+1u);
 uint32_t child=rd(ram,node+44u);
 if(!flag(sr,child==0u)){
  uint32_t found=sa_model_descriptor_search_8c056738(ram,sr,child,target,counter);
  if(flag(sr,found==target))return target;
 }
 uint32_t sibling=rd(ram,node+48u);
 if(flag(sr,sibling==0u))return 0;
 uint32_t found=sa_model_descriptor_search_8c056738(ram,sr,sibling,target,counter);
 if(flag(sr,found==target))return target;
 return 0;
}
SA_STAGE_EXPORT uint32_t sa_model_descriptor_lookup_8c056712(uint8_t *ram,uint32_t *sr,uint32_t root,uint32_t target,uint32_t stack){
 uint32_t counter=stack-12u;wr(ram,counter,0u);wr(ram,stack-8u,root);
 uint32_t result=sa_model_descriptor_search_8c056738(ram,sr,root,target,counter);
 return flag(sr,result==0u)?0xffffffffu:rd(ram,counter);
}
