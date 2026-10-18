#include "stage00_task.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
SA_STAGE_EXPORT uint32_t sa_model_descriptor_walk_8c0567d4(uint8_t *ram,uint32_t *sr,uint32_t list,uint32_t count_address){
 uint32_t count=rd(ram,count_address),node=rd(ram,list+(count<<2));wr(ram,count_address,count+1u);
 uint32_t child=rd(ram,node+44u);*sr=(*sr&~1u)|(child==0u);
 if(child!=0u){*sr=(*sr&~1u)|(list==0u);if(list!=0u)wr(ram,list+(rd(ram,count_address)<<2),child);(void)sa_model_descriptor_walk_8c0567d4(ram,sr,list,count_address);}
 uint32_t sibling=rd(ram,node+48u);*sr=(*sr&~1u)|(sibling==0u);
 if(sibling!=0u){*sr=(*sr&~1u)|(list==0u);if(list!=0u)wr(ram,list+(rd(ram,count_address)<<2),sibling);(void)sa_model_descriptor_walk_8c0567d4(ram,sr,list,count_address);}
 return rd(ram,count_address);
}
SA_STAGE_EXPORT uint32_t sa_model_descriptor_collect_8c0567b0(uint8_t *ram,uint32_t *sr,uint32_t root,uint32_t list,uint32_t caller_stack){
 uint32_t counter=caller_stack-12u;*sr=(*sr&~1u)|(list==0u);wr(ram,counter,0u);if(list!=0u)wr(ram,list,root);
 return sa_model_descriptor_walk_8c0567d4(ram,sr,list,counter);
}
