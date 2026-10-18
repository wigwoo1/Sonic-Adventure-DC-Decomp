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
/* Static C recovery of retail Tikal helper 8c0985ec..8c09865c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_task_create_base(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c0985ec;
L8c0985ec: sa_motion_publish_stop(ctx,0x8c0985ecu); r[3]=rd(m,0x8c0986c0u,4); /* mov.l 0x8c0986c0,r3 */
 goto L8c0985ee;
L8c0985ee: sa_motion_publish_stop(ctx,0x8c0985eeu); r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c0985f0;
L8c0985f0: sa_motion_publish_stop(ctx,0x8c0985f0u); r[14]=rd(m,r[3],4); /* mov.l @r3,r14 */
 goto L8c0985f2;
L8c0985f2: sa_motion_publish_stop(ctx,0x8c0985f2u); *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c0985f4;
L8c0985f4: sa_motion_publish_stop(ctx,0x8c0985f4u); if(!(*sr&1u))goto L8c0985fc; /* bf 0x8c0985fc */
 goto L8c0985f6;
L8c0985f6: sa_motion_publish_stop(ctx,0x8c0985f6u); r[0]=0u; /* mov #0x0,r0 */
 goto L8c0985f8;
L8c0985f8: sa_motion_publish_stop(ctx,0x8c0985fau); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c0985fc: sa_motion_publish_stop(ctx,0x8c0985fcu); r[7]=rd(m,r[14],4); /* mov.l @r14,r7 */
 goto L8c0985fe;
L8c0985fe: sa_motion_publish_stop(ctx,0x8c0985feu); *sr=(*sr&~1u)|!(r[7]&r[7]); /* tst r7,r7 */
 goto L8c098600;
L8c098600: sa_motion_publish_stop(ctx,0x8c098602u); take=(*sr&1u);r[6]=0u;if(take)goto L8c098606;goto L8c098604; /* bt/s 0x8c098606 */
 goto L8c098604;
L8c098604: sa_motion_publish_stop(ctx,0x8c098604u); wr(m,r[7]+0x4u,r[6],4); /* mov.l r6,@(0x4,r7) */
 goto L8c098606;
L8c098606: sa_motion_publish_stop(ctx,0x8c098606u); r[2]=rd(m,0x8c0986c0u,4); /* mov.l 0x8c0986c0,r2 */
 goto L8c098608;
L8c098608: sa_motion_publish_stop(ctx,0x8c098608u); *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c09860a;
L8c09860a: sa_motion_publish_stop(ctx,0x8c09860cu); take=(*sr&1u);wr(m,r[2],r[7],4);if(take)goto L8c098612;goto L8c09860e; /* bt/s 0x8c098612 */
 goto L8c09860e;
L8c09860e: sa_motion_publish_stop(ctx,0x8c098610u); wr(m,r[14]+0x10u,r[5],4);goto L8c098616; /* bra 0x8c098616 */
 goto L8c098612;
L8c098612: sa_motion_publish_stop(ctx,0x8c098612u); r[3]=rd(m,0x8c0986c4u,4); /* mov.l 0x8c0986c4,r3 */
 goto L8c098614;
L8c098614: sa_motion_publish_stop(ctx,0x8c098614u); wr(m,r[14]+0x10u,r[3],4); /* mov.l r3,@(0x10,r14) */
 goto L8c098616;
L8c098616: sa_motion_publish_stop(ctx,0x8c098616u); r[0]=r[4]; /* mov r4,r0 */
 goto L8c098618;
L8c098618: sa_motion_publish_stop(ctx,0x8c098618u); *sr=(*sr&~1u)|(r[0]==0x7u); /* cmp/eq #0x7,r0 */
 goto L8c09861a;
L8c09861a: sa_motion_publish_stop(ctx,0x8c09861au); if((*sr&1u))goto L8c098644; /* bt 0x8c098644 */
 goto L8c09861c;
L8c09861c: sa_motion_publish_stop(ctx,0x8c09861cu); r[0]=r[4]; /* mov r4,r0 */
 goto L8c09861e;
L8c09861e: sa_motion_publish_stop(ctx,0x8c09861eu); *sr=(*sr&~1u)|(r[0]==0x8u); /* cmp/eq #0x8,r0 */
 goto L8c098620;
L8c098620: sa_motion_publish_stop(ctx,0x8c098620u); if(!(*sr&1u))goto L8c098624; /* bf 0x8c098624 */
 goto L8c098622;
L8c098622: sa_motion_publish_stop(ctx,0x8c098622u); r[4]=7u; /* mov #0x7,r4 */
 goto L8c098624;
L8c098624: sa_motion_publish_stop(ctx,0x8c098624u); r[3]=rd(m,0x8c0986c8u,4); /* mov.l 0x8c0986c8,r3 */
 goto L8c098626;
L8c098626: sa_motion_publish_stop(ctx,0x8c098626u); r[0]=r[4]; /* mov r4,r0 */
 goto L8c098628;
L8c098628: sa_motion_publish_stop(ctx,0x8c098628u); r[0]<<=2; /* shll2 r0 */
 goto L8c09862a;
L8c09862a: sa_motion_publish_stop(ctx,0x8c09862au); r[0]+=r[3]; /* add r3,r0 */
 goto L8c09862c;
L8c09862c: sa_motion_publish_stop(ctx,0x8c09862cu); r[2]=rd(m,r[0],4); /* mov.l @r0,r2 */
 goto L8c09862e;
L8c09862e: sa_motion_publish_stop(ctx,0x8c09862eu); *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098630;
L8c098630: sa_motion_publish_stop(ctx,0x8c098630u); if(!(*sr&1u))goto L8c09863a; /* bf 0x8c09863a */
 goto L8c098632;
L8c098632: sa_motion_publish_stop(ctx,0x8c098632u); wr(m,r[0],r[14],4); /* mov.l r14,@r0 */
 goto L8c098634;
L8c098634: sa_motion_publish_stop(ctx,0x8c098634u); wr(m,r[14],r[6],4); /* mov.l r6,@r14 */
 goto L8c098636;
L8c098636: sa_motion_publish_stop(ctx,0x8c098638u); wr(m,r[14]+0x4u,r[6],4);goto L8c098644; /* bra 0x8c098644 */
 goto L8c09863a;
L8c09863a: sa_motion_publish_stop(ctx,0x8c09863au); r[4]=rd(m,r[0],4); /* mov.l @r0,r4 */
 goto L8c09863c;
L8c09863c: sa_motion_publish_stop(ctx,0x8c09863cu); wr(m,r[14]+0x4u,r[6],4); /* mov.l r6,@(0x4,r14) */
 goto L8c09863e;
L8c09863e: sa_motion_publish_stop(ctx,0x8c09863eu); wr(m,r[14],r[4],4); /* mov.l r4,@r14 */
 goto L8c098640;
L8c098640: sa_motion_publish_stop(ctx,0x8c098640u); wr(m,r[4]+0x4u,r[14],4); /* mov.l r14,@(0x4,r4) */
 goto L8c098642;
L8c098642: sa_motion_publish_stop(ctx,0x8c098642u); wr(m,r[0],r[14],4); /* mov.l r14,@r0 */
 goto L8c098644;
L8c098644: sa_motion_publish_stop(ctx,0x8c098644u); r[0]=r[14]; /* mov r14,r0 */
 goto L8c098646;
L8c098646: sa_motion_publish_stop(ctx,0x8c098646u); wr(m,r[14]+0x1cu,r[6],4); /* mov.l r6,@(0x1c,r14) */
 goto L8c098648;
L8c098648: sa_motion_publish_stop(ctx,0x8c098648u); wr(m,r[14]+0x20u,r[6],4); /* mov.l r6,@(0x20,r14) */
 goto L8c09864a;
L8c09864a: sa_motion_publish_stop(ctx,0x8c09864au); wr(m,r[14]+0x24u,r[6],4); /* mov.l r6,@(0x24,r14) */
 goto L8c09864c;
L8c09864c: sa_motion_publish_stop(ctx,0x8c09864cu); wr(m,r[14]+0x28u,r[6],4); /* mov.l r6,@(0x28,r14) */
 goto L8c09864e;
L8c09864e: sa_motion_publish_stop(ctx,0x8c09864eu); wr(m,r[14]+0x2cu,r[6],4); /* mov.l r6,@(0x2c,r14) */
 goto L8c098650;
L8c098650: sa_motion_publish_stop(ctx,0x8c098650u); wr(m,r[14]+0x14u,r[6],4); /* mov.l r6,@(0x14,r14) */
 goto L8c098652;
L8c098652: sa_motion_publish_stop(ctx,0x8c098652u); wr(m,r[14]+0x18u,r[6],4); /* mov.l r6,@(0x18,r14) */
 goto L8c098654;
L8c098654: sa_motion_publish_stop(ctx,0x8c098654u); wr(m,r[14]+0xcu,r[6],4); /* mov.l r6,@(0xc,r14) */
 goto L8c098656;
L8c098656: sa_motion_publish_stop(ctx,0x8c098656u); wr(m,r[14]+0x8u,r[6],4); /* mov.l r6,@(0x8,r14) */
 goto L8c098658;
L8c098658: sa_motion_publish_stop(ctx,0x8c09865au); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
