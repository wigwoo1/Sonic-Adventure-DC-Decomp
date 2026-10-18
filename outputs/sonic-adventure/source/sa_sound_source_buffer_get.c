#include "fpu_cleanup_transfer.h"
/* Retail65e798..65e79c: RTS000b; delay-slot MOV.L@(0x1c,R4),R0 5047. */
SA_STAGE_EXPORT int sa_sound_source_buffer_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)f;(void)sr;(void)pr;(void)fpul;(void)macl;(void)ctx;(void)call;
 r[0]=0;for(unsigned i=0;i<4;i++)r[0]|=(uint32_t)m[((r[4]+0x1cu)&0xffffffu)+i]<<(i*8);
 return 0;
}
