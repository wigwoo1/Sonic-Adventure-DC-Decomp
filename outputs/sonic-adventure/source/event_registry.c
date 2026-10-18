#include "event_registry.h"
static uint32_t read_word(uint8_t *ram,uint32_t address)
{
 uint8_t *p=ram+((address&0x1fffffffu)-0x0c000000u);
 return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
static void write_word(uint8_t *ram,uint32_t address,uint32_t value)
{
 uint8_t *p=ram+((address&0x1fffffffu)-0x0c000000u);
 for(unsigned i=0;i<4;i++)p[i]=(uint8_t)(value>>(i*8));
}
static uint32_t asr(uint32_t value,unsigned count)
{
 return (value>>count)|((value&0x80000000u)?(~0u<<(32u-count)):0u);
}
static void test(uint32_t *sr,int condition){*sr=(*sr&~1u)|(condition?1u:0u);}
uint32_t sa_event_register_8c64594c(uint8_t *ram,uint32_t *sr,uint32_t vbr,
 uint32_t *mask_a,uint32_t *mask_b,uint32_t event,uint32_t callback,
 uint32_t priority,uint32_t argument,const struct sa_event_registry_services *services)
{
 uint32_t saved_mask=*sr&0xf0u;
 /* Retail MOV.W makes the event a signed 16-bit value. */
 event=(event&0xffffu)|((event&0x8000u)?0xffff0000u:0u);
 *sr=(*sr&0xffffff0fu)|0xf0u;
 uint32_t head_address=0x8c893840u+(asr(event,4)<<2);
 uint32_t node=read_word(ram,head_address),previous=node;
 priority|=0x80000000u;
 while(node!=0u && read_word(ram,node+12u)<priority){previous=node;node=read_word(ram,node+4u);}
 uint32_t index=1u;
 while(index<256u && (read_word(ram,0x8c892840u+index*16u+12u)&0x80000000u))index++;
 test(sr,index>=256u);
 if(index>=256u){*sr=(*sr&0xffffff0fu)|saved_mask;return 0u;}
 uint32_t record=0x8c892840u+index*16u;
 test(sr,node==0u);
 if(node==0u){test(sr,previous==0u);}
 if(node==0u && previous==0u){
  test(sr,(event&0x80000000u)==0u && event>=0x900u);
  if((event&0x80000000u)!=0u || event<0x900u){
   uint32_t vector=vbr+0x200u+asr(event-0x200u,3);
   /* Three arithmetic single shifts supply T from bit two. */
   test(sr,((event-0x200u)&4u)!=0u);
   write_word(ram,vector,0x8c645afau);
  }else{
   test(sr,event==0x900u);
   if(event==0x900u)services->install(services->context,0x8c64135au,0x8c645b1eu,0u,sr);
   else{
    test(sr,event==0x920u);
    if(event==0x920u)services->install(services->context,0x8c64137cu,0x8c645b68u,0u,sr);
    else write_word(ram,vbr+0x22cu,0x8c645b8eu);
   }
  }
  write_word(ram,head_address,record);write_word(ram,record+4u,0u);
  write_word(ram,record,callback);write_word(ram,record+8u,argument);write_word(ram,record+12u,priority);
  uint32_t group=asr(event,8),bit=(event&255u)>>5;
  /* SHLR supplies T, then each CMP overwrites it. */
  test(sr,group==10u);
  if(group==10u)*mask_a|=1u<<((bit+12u)&31u);
  else{test(sr,group==11u);if(group==11u)*mask_b|=1u<<(bit&31u);}
 }else{
  uint32_t current_head=read_word(ram,head_address);
  test(sr,node==current_head);
  if(node==current_head){write_word(ram,record+4u,node);write_word(ram,head_address,record);}
  else{write_word(ram,previous+4u,record);write_word(ram,record+4u,node);}
  write_word(ram,record,callback);write_word(ram,record+8u,argument);write_word(ram,record+12u,priority);
 }
 *sr=(*sr&0xffffff0fu)|saved_mask;
 return index;
}
uint32_t sa_event_remove_8c645c54(uint8_t *ram,uint32_t *sr,uint32_t vbr,
 uint32_t *mask_a,uint32_t *mask_b,uint32_t handle,
 const struct sa_event_registry_services *services)
{
 uint32_t saved_mask=*sr&0xf0u;
 *sr=(*sr&0xffffff0fu)|0xf0u;
 uint32_t target=0x8c892840u+(handle<<4),predecessor=1u;
 while(predecessor<256u){
  uint32_t record=0x8c892840u+predecessor*16u;
  if(read_word(ram,record+4u)==target && (read_word(ram,record+12u)&0x80000000u))break;
  predecessor++;
 }
 test(sr,predecessor==256u);
 if(predecessor<256u){
  write_word(ram,0x8c892840u+predecessor*16u+4u,read_word(ram,target+4u));
  write_word(ram,target+12u,0u);
 }else{
  uint32_t head=1u;
  while(head<192u && read_word(ram,0x8c893840u+head*4u)!=target)head++;
  if(head<192u){
   uint32_t address=0x8c893840u+head*4u;
   write_word(ram,address,read_word(ram,target+4u));
   write_word(ram,target+12u,0u);
   if(read_word(ram,address)==0u){
    test(sr,head==0x90u);
    if(head==0x90u)services->install(services->context,0x8c64135au,0u,0u,sr);
    else{
     test(sr,head==0x92u);
     if(head==0x92u)services->install(services->context,0x8c64137cu,0u,0u,sr);
     else{
      uint32_t event=head<<4,bit=(event&255u)>>5;
      if((event>>8)==10u)*mask_a&=~(1u<<((bit+12u)&31u));
      else if((event>>8)==11u)*mask_b&=~(1u<<(bit&31u));
      /* Original removal uses logical SHR, unlike insertion's arithmetic SHR. */
      uint32_t vector=vbr+0x200u+((event-0x200u)>>3);
      write_word(ram,vector,0x8c65eaecu);
     }
    }
   }
  }
  test(sr,head==192u);
 }
 *sr=(*sr&0xffffff0fu)|saved_mask;
 return *sr; /* Retail leaves restored SR in r0. */
}
