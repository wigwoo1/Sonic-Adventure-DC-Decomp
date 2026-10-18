#include "table_reference_release.h"
static uint32_t read_word(const uint8_t *ram,uint32_t address)
{
 const uint8_t *p=ram+((address&0x1fffffffu)-0x0c000000u);
 return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
static void test(uint32_t *sr,int condition){*sr=(*sr&~1u)|(condition?1u:0u);}
uint32_t sa_table_reference_release_8c098ffc(uint8_t *ram,uint32_t *sr,uint32_t index,
 void *context,uint32_t (*release)(void *,uint32_t,uint32_t *))
{
 uint32_t result=0x8c1c2584u;
 uint32_t row=read_word(ram,result+(index<<2));
 test(sr,row==0u);
 if(row==0u)return result;
 uint32_t counter_offset=((0x8c1c24b4u+index)&0x1fffffffu)-0x0c000000u;
 uint32_t count=ram[counter_offset];
 result=count|((count&128u)?0xffffff00u:0u);
 test(sr,result==1u);
 if(result==1u){
  uint32_t argument=read_word(ram,row+4u);
  test(sr,argument==0u);
  while(argument!=0u){
   result=release(context,argument,sr);
   row+=8u;
   argument=read_word(ram,row+4u);
   test(sr,argument==0u);
  }
 }
 count=ram[counter_offset];
 test(sr,count>0u && count<128u);
 if(count>0u && count<128u)ram[counter_offset]=(uint8_t)(count-1u);
 return result;
}
