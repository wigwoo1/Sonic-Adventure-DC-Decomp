#include "owner_helper_state0.h"
uint32_t sa_heap_allocate_ram_8c098a80(uint8_t *,uint32_t *,uint32_t);
int sa_owner_model_change_entry(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,const uint32_t *,uint32_t,uint32_t,void *,sa_owner_state0_service,uint32_t *);
static uint32_t allocate(void *context,uint32_t address,uint32_t argc,uint32_t size,uint32_t unused,uint32_t incoming,uint32_t *sr){
 (void)address;(void)argc;(void)unused;(void)incoming;
 return sa_heap_allocate_ram_8c098a80(context,sr,size);
}
SA_STAGE_EXPORT int sa_owner_model_change_with_native_heap(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t stack,const uint32_t *callee,uint32_t fr15,uint32_t pr,uint32_t *r0){
 return sa_owner_model_change_entry(ram,sr,fpscr,fpul,config,stack,callee,fr15,pr,ram,allocate,r0);
}
