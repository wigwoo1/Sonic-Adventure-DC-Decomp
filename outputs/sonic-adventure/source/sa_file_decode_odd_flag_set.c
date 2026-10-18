#include "fpu_cleanup_transfer.h"
/* Retail 644c90..644c98, including store in RTS delay slot.
 * d206/e301/000b/2232: load global pointer,set R3=1,return,store. */
SA_STAGE_EXPORT int sa_file_decode_odd_flag_set(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)f;(void)sr;(void)pr;(void)fpul;(void)macl;(void)ctx;(void)call;
 r[2]=0;for(unsigned i=0;i<4;i++)r[2]|=(uint32_t)m[0x644cacu+i]<<(i*8);
 r[3]=1;for(unsigned i=0;i<4;i++)m[(r[2]&0xffffffu)+i]=(uint8_t)(r[3]>>(i*8));
 return 0;
}
