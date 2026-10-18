#include "fpu_cleanup_transfer.h"
/* Retail667696..6676a0: 9092/0456/7004/000b/0466.
 * The callback/user-data fields come from the signed16-bit literal at6677be.
 * The user-data write occupies the RTS delay slot. */
SA_STAGE_EXPORT int sa_sound_stream_output_callback_set(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)f;(void)sr;(void)pr;(void)fpul;(void)macl;(void)ctx;(void)call;
 r[0]=(uint32_t)(int32_t)(int16_t)((uint16_t)m[0x6677be]|((uint16_t)m[0x6677bf]<<8));
 for(unsigned i=0;i<4;i++)m[((r[4]+r[0])&0xffffffu)+i]=(uint8_t)(r[5]>>(i*8));
 r[0]+=4;
 for(unsigned i=0;i<4;i++)m[((r[4]+r[0])&0xffffffu)+i]=(uint8_t)(r[6]>>(i*8));
 return 0;
}
