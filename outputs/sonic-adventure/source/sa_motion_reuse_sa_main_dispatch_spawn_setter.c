#include "sa_draw_motion_stop.h"
/* State-gate spawn/callback setter; descriptive names inferred.
 * Static C from checked retail bytes. Integer instructions, no FPU mode guard.
 * Original guest-memory addressing/stack/SR/callee ordering is preserved.
 * Host 0 completes (guest results in registers); missing service -2 at reached
 * boundary. Requires valid original pointer/table preconditions; no MMIO.
 */
#include "fpu_cleanup_transfer.h"
typedef int (*sa_shared_service)(void *,uint32_t,uint8_t *,
    struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=0;for(unsigned i=0;i<n;i++)value|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return value;
}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)value:(uint32_t)(int32_t)(int16_t)value;
}
static void wr(uint8_t *m,uint32_t a,uint32_t value,unsigned n){
 for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(value>>(8*i));
}
SA_STAGE_EXPORT int sa_main_dispatch_spawn_8c07b320(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c07b320;
L8c07b320: sa_motion_publish_stop(ctx,0x8c07b320u); r[15]-=4;wr(m,r[15],*pr,4);goto L8c07b322; /* sts.l PR,@-r15 */
L8c07b322: sa_motion_publish_stop(ctx,0x8c07b322u); r[15]+=4294967292u;goto L8c07b324; /* add #-0x4,r15 */
L8c07b324: sa_motion_publish_stop(ctx,0x8c07b324u); r[3]=rd(m,0x8c07b4c4u,4);goto L8c07b326; /* mov.l 0x8c07b4c4,r3 */
L8c07b326: sa_motion_publish_stop(ctx,0x8c07b326u); r[5]=6u;goto L8c07b328; /* mov #0x6,r5 */
L8c07b328: sa_motion_publish_stop(ctx,0x8c07b328u); r[6]=rd(m,0x8c07b4c0u,4);goto L8c07b32a; /* mov.l 0x8c07b4c0,r6 */
L8c07b32a: sa_motion_publish_stop(ctx,0x8c07b32au); wr(m,r[15],r[4],4);goto L8c07b32c; /* mov.l r4,@r15 */
L8c07b32c: sa_motion_publish_stop(ctx,0x8c07b32eu); target=r[3];*pr=0x8c07b330u;r[4]=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07b330; /* jsr @r3 */
L8c07b330: sa_motion_publish_stop(ctx,0x8c07b330u); *sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c07b332; /* tst r0,r0 */
L8c07b332: sa_motion_publish_stop(ctx,0x8c07b334u); take=(*sr&1u);r[4]=r[0];if(take)goto L8c07b33e;goto L8c07b336; /* bt/s 0x8c07b33e */
L8c07b336: sa_motion_publish_stop(ctx,0x8c07b336u); r[0]=rd(m,r[15],4);goto L8c07b338; /* mov.l @r15,r0 */
L8c07b338: sa_motion_publish_stop(ctx,0x8c07b338u); r[3]=rd(m,r[4]+0x20u,4);goto L8c07b33a; /* mov.l @(0x20,r4),r3 */
L8c07b33a: sa_motion_publish_stop(ctx,0x8c07b33au); r[0]=sx(m,r[0]+0x8u,1);goto L8c07b33c; /* mov.b @(0x8,r0),r0 */
L8c07b33c: sa_motion_publish_stop(ctx,0x8c07b33cu); wr(m,r[3]+0x8u,r[0],1);goto L8c07b33e; /* mov.b r0,@(0x8,r3) */
L8c07b33e: sa_motion_publish_stop(ctx,0x8c07b33eu); r[15]+=4u;goto L8c07b340; /* add #0x4,r15 */
L8c07b340: sa_motion_publish_stop(ctx,0x8c07b340u); *pr=rd(m,r[15],4);r[15]+=4;goto L8c07b342; /* lds.l @r15+,PR */
L8c07b342: sa_motion_publish_stop(ctx,0x8c07b344u); return 0; /* rts */
}

SA_STAGE_EXPORT int sa_main_dispatch_callback_set_8c07b2b8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c07b2b8;
L8c07b2b8: sa_motion_publish_stop(ctx,0x8c07b2b8u); r[3]=rd(m,0x8c07b300u,4);goto L8c07b2ba; /* mov.l 0x8c07b300,r3 */
L8c07b2ba: sa_motion_publish_stop(ctx,0x8c07b2bau); r[6]=r[4]&255u;goto L8c07b2bc; /* extu.b r4,r6 */
L8c07b2bc: sa_motion_publish_stop(ctx,0x8c07b2bcu); r[6]<<=2;goto L8c07b2be; /* shll2 r6 */
L8c07b2be: sa_motion_publish_stop(ctx,0x8c07b2beu); r[6]+=r[3];goto L8c07b2c0; /* add r3,r6 */
L8c07b2c0: sa_motion_publish_stop(ctx,0x8c07b2c0u); r[2]=rd(m,r[6],4);goto L8c07b2c2; /* mov.l @r6,r2 */
L8c07b2c2: sa_motion_publish_stop(ctx,0x8c07b2c2u); *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c07b2c4; /* tst r2,r2 */
L8c07b2c4: sa_motion_publish_stop(ctx,0x8c07b2c4u); if((*sr&1u))goto L8c07b2d8;goto L8c07b2c6; /* bt 0x8c07b2d8 */
L8c07b2c6: sa_motion_publish_stop(ctx,0x8c07b2c6u); r[4]=rd(m,r[6],4);goto L8c07b2c8; /* mov.l @r6,r4 */
L8c07b2c8: sa_motion_publish_stop(ctx,0x8c07b2c8u); r[0]=112u;goto L8c07b2ca; /* mov #0x70,r0 */
L8c07b2ca: sa_motion_publish_stop(ctx,0x8c07b2cau); r[4]=rd(m,r[4]+r[0],4);goto L8c07b2cc; /* mov.l @(r0,r4),r4 */
L8c07b2cc: sa_motion_publish_stop(ctx,0x8c07b2ccu); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c07b2ce; /* tst r4,r4 */
L8c07b2ce: sa_motion_publish_stop(ctx,0x8c07b2ceu); if((*sr&1u))goto L8c07b2d8;goto L8c07b2d0; /* bt 0x8c07b2d8 */
L8c07b2d0: sa_motion_publish_stop(ctx,0x8c07b2d0u); r[0]=rd(m,0x8c07b304u,4);goto L8c07b2d2; /* mov.l 0x8c07b304,r0 */
L8c07b2d2: sa_motion_publish_stop(ctx,0x8c07b2d2u); r[5]<<=2;goto L8c07b2d4; /* shll2 r5 */
L8c07b2d4: sa_motion_publish_stop(ctx,0x8c07b2d4u); r[1]=rd(m,r[5]+r[0],4);goto L8c07b2d6; /* mov.l @(r0,r5),r1 */
L8c07b2d6: sa_motion_publish_stop(ctx,0x8c07b2d6u); wr(m,r[4]+0xcu,r[1],4);goto L8c07b2d8; /* mov.l r1,@(0xc,r4) */
L8c07b2d8: sa_motion_publish_stop(ctx,0x8c07b2dau); return 0; /* rts */
}
