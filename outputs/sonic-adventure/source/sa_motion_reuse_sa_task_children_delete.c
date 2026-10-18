#include "sa_draw_motion_stop.h"
#include "fpu_cleanup_transfer.h"
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v,unsigned n){for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|((a==b)||(!(a&0x7fffffffu)&&!(b&0x7fffffffu)));return 0;
}
/* Static C recovery of retail Tikal helper 8c098e60..8c098e98.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_task_children_delete(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c098e60;
L8c098e60: sa_motion_publish_stop(ctx,0x8c098e60u); r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c098e62;
L8c098e62: sa_motion_publish_stop(ctx,0x8c098e62u); r[14]=r[4]; /* mov r4,r14 */
 goto L8c098e64;
L8c098e64: sa_motion_publish_stop(ctx,0x8c098e64u); r[3]=rd(m,0x8c098ec0u,4); /* mov.l 0x8c098ec0,r3 */
 goto L8c098e66;
L8c098e66: sa_motion_publish_stop(ctx,0x8c098e66u); r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c098e68;
L8c098e68: sa_motion_publish_stop(ctx,0x8c098e68u); r[2]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r2 */
 goto L8c098e6a;
L8c098e6a: sa_motion_publish_stop(ctx,0x8c098e6au); *sr=(*sr&~1u)|(r[3]==r[2]); /* cmp/eq r3,r2 */
 goto L8c098e6c;
L8c098e6c: sa_motion_publish_stop(ctx,0x8c098e6cu); if(!(*sr&1u))goto L8c098e82; /* bf 0x8c098e82 */
 goto L8c098e6e;
L8c098e6e: sa_motion_publish_stop(ctx,0x8c098e70u); *pr=0x8c098e72u;r[4]=r[14];status=call(ctx,0x8c098e98u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098e98 */
 goto L8c098e72;
L8c098e72: sa_motion_publish_stop(ctx,0x8c098e72u); r[3]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r3 */
 goto L8c098e74;
L8c098e74: sa_motion_publish_stop(ctx,0x8c098e74u); *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c098e76;
L8c098e76: sa_motion_publish_stop(ctx,0x8c098e76u); if(!(*sr&1u))goto L8c098e92; /* bf 0x8c098e92 */
 goto L8c098e78;
L8c098e78: sa_motion_publish_stop(ctx,0x8c098e78u); r[3]=rd(m,0x8c098ec4u,4); /* mov.l 0x8c098ec4,r3 */
 goto L8c098e7a;
L8c098e7a: sa_motion_publish_stop(ctx,0x8c098e7au); wr(m,r[14]+0x10u,r[3],4); /* mov.l r3,@(0x10,r14) */
 goto L8c098e7c;
L8c098e7c: sa_motion_publish_stop(ctx,0x8c098e7cu); *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c098e7e;
L8c098e7e: sa_motion_publish_stop(ctx,0x8c098e80u); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c098e82: sa_motion_publish_stop(ctx,0x8c098e82u); r[2]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r2 */
 goto L8c098e84;
L8c098e84: sa_motion_publish_stop(ctx,0x8c098e84u); *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098e86;
L8c098e86: sa_motion_publish_stop(ctx,0x8c098e86u); if((*sr&1u))goto L8c098e92; /* bt 0x8c098e92 */
 goto L8c098e88;
L8c098e88: sa_motion_publish_stop(ctx,0x8c098e8au); *pr=0x8c098e8cu;r[4]=rd(m,r[14]+0xcu,4);status=call(ctx,0x8c09865cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c09865c */
 goto L8c098e8c;
L8c098e8c: sa_motion_publish_stop(ctx,0x8c098e8cu); r[2]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r2 */
 goto L8c098e8e;
L8c098e8e: sa_motion_publish_stop(ctx,0x8c098e8eu); *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098e90;
L8c098e90: sa_motion_publish_stop(ctx,0x8c098e90u); if(!(*sr&1u))goto L8c098e88; /* bf 0x8c098e88 */
 goto L8c098e92;
L8c098e92: sa_motion_publish_stop(ctx,0x8c098e92u); *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c098e94;
L8c098e94: sa_motion_publish_stop(ctx,0x8c098e96u); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
