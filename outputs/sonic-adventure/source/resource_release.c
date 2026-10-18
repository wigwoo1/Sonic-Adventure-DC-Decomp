#include "resource_release.h"
#include "resource_type_classifier.h"
static uint8_t *ptr(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ptr(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)ptr(ram,a)[i]=(uint8_t)(v>>(8*i));}
static uint32_t half(uint8_t *ram,uint32_t a){uint8_t *p=ptr(ram,a);return p[0]|((uint32_t)p[1]<<8);}
static void wh(uint8_t *ram,uint32_t a,uint32_t v){uint8_t *p=ptr(ram,a);p[0]=(uint8_t)v;p[1]=(uint8_t)(v>>8);}
static void t(uint32_t *sr,uint32_t yes){*sr=(*sr&~1u)|(yes?1u:0u);}
uint32_t sa_resource_release_8c64ddc0(uint8_t *ram,uint32_t *sr,uint32_t resource,void *ctx,sa_resource_release_invoke invoke)
{
 uint32_t flags=rd(ram,resource+0x10u);t(sr,(flags&2u)==0u);
 if((flags&2u)==0u){
  flags=rd(ram,resource+0x10u);t(sr,(flags&1u)==0u);
  if(flags&1u)goto error;
  t(sr,rd(ram,resource)==0xffffffffu);
  if(rd(ram,resource)==0xffffffffu)goto error;
  goto destroy;
 }
 uint32_t type=sa_resource_type_classifier_8c64f206(sr,(rd(ram,resource+0x18u)>>16)&0xff00u);
 t(sr,type==1u);
 if(type!=1u){
  uint32_t count=half(ram,resource+0x40u);t(sr,count==0u);
  if(count)wh(ram,resource+0x40u,count-1u);
  count=half(ram,resource+0x40u);t(sr,count==0u);
  if(count)return 1u;
  (void)invoke(ctx,0x8c653d68u,resource+0x18u,sr);
  wr(ram,resource+0x10u,rd(ram,resource+0x10u)&~2u);
  uint32_t identifier=rd(ram,resource);t(sr,rd(ram,0x8c67348cu)==identifier);
  if(rd(ram,0x8c67348cu)==identifier)wr(ram,0x8c67348cu,0xffffffffu);
  flags=rd(ram,resource+0x10u);t(sr,flags==0u);
  if(flags)return 1u;
  goto destroy;
 }
 uint32_t matches=0u;
 for(uint32_t i=0;i<rd(ram,0x8c673488u);i++){
  t(sr,rd(ram,rd(ram,0x8c673484u)+i*0x44u)==rd(ram,resource));
  if(*sr&1u)matches++;
 }
 t(sr,1u); /* Terminal unsigned table-bound compare. */
 t(sr,(int32_t)matches>1);
 if((int32_t)matches>1){
  flags=rd(ram,resource+0x10u);t(sr,(flags&1u)==0u);
  if(flags&1u)return 1u;
  wh(ram,resource+0x40u,half(ram,resource+0x40u)-1u);
  uint32_t count=half(ram,resource+0x40u);t(sr,count==0u);
  if(count)return 1u;
  wr(ram,resource+0x10u,rd(ram,resource+0x10u)&~2u);
  goto destroy;
 }
 wh(ram,resource+0x40u,half(ram,resource+0x40u)-1u);
 uint32_t count=half(ram,resource+0x40u);t(sr,count==0u);
 if(count)return 1u;
 (void)invoke(ctx,0x8c653d68u,resource+0x18u,sr);
 flags=rd(ram,resource+0x10u)&~2u;t(sr,(flags&1u)==0u);
 wr(ram,resource+0x10u,flags);
 if(flags&1u)return 1u;
 destroy:
 (void)invoke(ctx,0x8c64e720u,resource,sr);
 return 1u;
 error:
 wr(ram,0x8c88f7b0u,1u);
 return 0xffffffffu;
}
