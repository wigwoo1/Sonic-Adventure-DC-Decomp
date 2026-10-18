#include "sa_draw_motion_stop.h"
/* Stored context callback thunk; descriptive name inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * Integer instructions, no FPU-mode restriction. Valid RAM/table preconditions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
typedef int (*sa_shared_service)(void *,uint32_t,uint8_t *,
    struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=0;for(unsigned i=0;i<n;i++)value|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return value;
}
SA_STAGE_EXPORT int sa_main_dispatch_context_callback_8c01b358(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01b358;
L8c01b358: sa_motion_publish_stop(ctx,0x8c01b358u); r[3]=rd(m,0x8c01b3bcu,4);goto L8c01b35a; /* mov.l 0x8c01b3bc,r3 */
L8c01b35a: sa_motion_publish_stop(ctx,0x8c01b35cu); target=r[3];if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}
