#include "stage00_task.h"
uint32_t sa_heap_allocate_ram_8c098a80(uint8_t *,uint32_t *,uint32_t);
/* 8c098b0c: element size rounds to four bytes before the wrapping product.
 * Exposes allocation, zero writes and SR; MACL/callee-save scratch stay in ABI. */
SA_STAGE_EXPORT uint32_t sa_heap_zero_allocate_ram_8c098b0c(uint8_t *ram,uint32_t *sr,uint32_t count,uint32_t size){
 uint32_t bytes=((size+3u)&0xfffffffcu)*count;
 uint32_t result=sa_heap_allocate_ram_8c098a80(ram,sr,bytes);
 *sr=(*sr&~1u)|(result==0u);if(!result)return 0;
 uint32_t words=bytes>>2;*sr=(*sr&~1u)|(words==0u);
 uint32_t address=result;
 while(words){--words;*sr=(*sr&~1u)|(words==0u);uint8_t *p=ram+((address&0x1fffffffu)-0x0c000000u);for(unsigned i=0;i<4;i++)p[i]=0;address+=4u;}
 return result;
}
