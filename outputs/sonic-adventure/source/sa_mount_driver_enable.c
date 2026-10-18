#include "fpu_cleanup_transfer.h"
/* Retail 8c658c20: RTS; MOV #1,R0 in the delay slot. The adjacent entry at
 * 8c658c24 has the same bytes. Preserves the remaining integer/FPU state. */
SA_STAGE_EXPORT int sa_mount_driver_enable(uint8_t *m,struct sa_fpu_banks *f,
    uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
    void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *))
{
    (void)m;(void)f;(void)sr;(void)pr;(void)fpul;(void)macl;(void)ctx;(void)call;
    r[0]=1;return 0;
}
