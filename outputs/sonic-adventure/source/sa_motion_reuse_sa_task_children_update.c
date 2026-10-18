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
/* Static C recovery of retail Tikal helper 8c098e98..8c098eba.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_task_children_update(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c098e98;
L8c098e98: sa_motion_publish_stop(ctx,0x8c098e98u); r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c098e9a;
L8c098e9a: sa_motion_publish_stop(ctx,0x8c098e9au); r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c098e9c;
L8c098e9c: sa_motion_publish_stop(ctx,0x8c098e9cu); r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c098e9e;
L8c098e9e: sa_motion_publish_stop(ctx,0x8c098e9eu); r[14]=rd(m,r[4]+0xcu,4); /* mov.l @(0xc,r4),r14 */
 goto L8c098ea0;
L8c098ea0: sa_motion_publish_stop(ctx,0x8c098ea0u); *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c098ea2;
L8c098ea2: sa_motion_publish_stop(ctx,0x8c098ea2u); if((*sr&1u))goto L8c098eb2; /* bt 0x8c098eb2 */
 goto L8c098ea4;
L8c098ea4: sa_motion_publish_stop(ctx,0x8c098ea4u); r[2]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r2 */
 goto L8c098ea6;
L8c098ea6: sa_motion_publish_stop(ctx,0x8c098ea6u); r[13]=rd(m,r[14],4); /* mov.l @r14,r13 */
 goto L8c098ea8;
L8c098ea8: sa_motion_publish_stop(ctx,0x8c098eaau); target=r[2];*pr=0x8c098eacu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c098eac;
L8c098eac: sa_motion_publish_stop(ctx,0x8c098eacu); r[14]=r[13]; /* mov r13,r14 */
 goto L8c098eae;
L8c098eae: sa_motion_publish_stop(ctx,0x8c098eaeu); *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c098eb0;
L8c098eb0: sa_motion_publish_stop(ctx,0x8c098eb0u); if(!(*sr&1u))goto L8c098ea4; /* bf 0x8c098ea4 */
 goto L8c098eb2;
L8c098eb2: sa_motion_publish_stop(ctx,0x8c098eb2u); *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c098eb4;
L8c098eb4: sa_motion_publish_stop(ctx,0x8c098eb4u); r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c098eb6;
L8c098eb6: sa_motion_publish_stop(ctx,0x8c098eb8u); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
