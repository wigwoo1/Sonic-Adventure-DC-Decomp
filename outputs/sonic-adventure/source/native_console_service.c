#include "native_console_service.h"
#include <stddef.h>
#include <limits.h>
static int guest_span(uint32_t address,uint32_t count,uint32_t *offset){
 uint32_t alias=address&UINT32_C(0xe0000000),physical=address&UINT32_C(0x1fffffff);
 if(alias!=0u&&alias!=UINT32_C(0x80000000)&&alias!=UINT32_C(0xa0000000))return 0;
 if(physical<UINT32_C(0x0c000000)||physical>UINT32_C(0x0d000000))return 0;
 uint32_t at=physical-UINT32_C(0x0c000000);
 if(count>UINT32_C(0x1000000)-at)return 0;
 *offset=at;return 1;
}
static int overlaps(const uint8_t *input,uint32_t size,const uint8_t *ram){
 if(!size)return 0;
 uintptr_t a=(uintptr_t)input,b=(uintptr_t)ram;
 if(a>UINTPTR_MAX-size||b>UINTPTR_MAX-UINT32_C(0x1000000))return 1;
 return a<b+UINT32_C(0x1000000)&&b<a+size;
}
static void write_word(uint8_t *m,uint32_t at,uint32_t value){for(unsigned i=0;i<4;i++)m[at+i]=(uint8_t)(value>>(i*8));}
uint32_t sa_console_service_size(void){return (uint32_t)sizeof(struct sa_console_service);}
int sa_console_service_init(struct sa_console_service *s){
 if(!s)return -2;
 volatile uint8_t *out=(volatile uint8_t*)s;for(unsigned i=0;i<sizeof(*s);i++)out[i]=0;
 return 0;
}
int sa_console_set_info(struct sa_console_service *s,const uint8_t *input,uint32_t size){
 if(!s||!input)return -2;
 if(size!=24u)return -3;
 s->info=input;s->info_size=size;s->info_configured=1;s->initialized=0;return 0;
}
int sa_console_set_flash(struct sa_console_service *s,const uint8_t *input,uint32_t size){
 if(!s||(!input&&size))return -2;
 s->flash=input;s->flash_size=size;s->flash_configured=1;return 0;
}
int sa_console_bind_vectors(const struct sa_console_service *s,uint8_t *ram){
 if(!s||!ram||(!s->info_configured&&!s->flash_configured))return -2;
 if(s->info_configured)write_word(ram,0xb0u,SA_NATIVE_SYSINFO_TARGET);
 if(s->flash_configured)write_word(ram,0xb8u,SA_NATIVE_FLASH_TARGET);
 return 0;
}
int sa_console_service(struct sa_console_service *s,uint8_t *ram,uint32_t channel,uint32_t selector,uint32_t first,uint32_t second,uint32_t count,uint32_t *result,uint32_t *write_result){
 if(!s||!ram||!result||!write_result)return -2;
 if(channel==0u){
  if(selector!=0u&&selector!=3u)return -8;
  if(!s->info_configured)return -2;
  if(s->info_size!=24u||!s->info||overlaps(s->info,s->info_size,ram))return -3;
  if(selector==3u&&!s->initialized)return -4;
  if(selector==0u){
   volatile uint8_t *out=ram+0x68u;const volatile uint8_t *input=s->info;
   for(unsigned i=0;i<24;i++)out[i]=input[i];
   s->initialized=1;*write_result=0;
  }else{*result=SA_NATIVE_SYSINFO_ADDRESS;*write_result=1;}
  s->info_calls++;return 0;
 }
 if(channel!=1u||selector!=1u)return -8;
 if(!s->flash_configured)return -2;
 if((!s->flash&&s->flash_size)||overlaps(s->flash,s->flash_size,ram))return -3;
 if(count>INT32_MAX||first>s->flash_size||count>s->flash_size-first){
  s->flash_calls++;*result=UINT32_MAX;*write_result=1;return 0;
 }
 uint32_t offset;
 if(!guest_span(second,count,&offset))return -3;
 volatile uint8_t *out=ram+offset;const volatile uint8_t *input=s->flash;
 for(uint32_t i=0;i<count;i++)out[i]=input[first+i];
 s->flash_calls++;s->flash_bytes+=count;*result=count;*write_result=1;return 0;
}
