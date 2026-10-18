#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_cache_invalidate(void *,uint32_t);
int sa_sh4_cache_purge(void *,uint32_t);
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
/* Static C recovery of retail Tikal helper 8c66846a..8c668498.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_channel_count(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66846a;
L8c66846a: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66846c;
L8c66846c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66846e;
L8c66846e: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c668470;
L8c668470: r[2]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r2 */
 goto L8c668472;
L8c668472: r[3]=r[4]; /* mov r4,r3 */
 goto L8c668474;
L8c668474: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c668476;
L8c668476: wr(m,r[3]+0x8u,r[2],4); /* mov.l r2,@(0x8,r3) */
 goto L8c668478;
L8c668478: r[3]=rd(m,0x8c6686c8u,4); /* mov.l 0x8c6686c8,r3 */
 goto L8c66847a;
L8c66847a: r[4]=rd(m,r[15],4); /* mov.l @r15,r4 */
 goto L8c66847c;
L8c66847c: target=r[3];*pr=0x8c668480u;r[4]=rd(m,r[4]+0x28u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668480;
L8c668480: r[14]=r[0]; /* mov r0,r14 */
 goto L8c668482;
L8c668482: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c668484;
L8c668484: if((*sr&1u))goto L8c66848e; /* bt 0x8c66848e */
 goto L8c668486;
L8c668486: r[3]=rd(m,0x8c6686c0u,4); /* mov.l 0x8c6686c0,r3 */
 goto L8c668488;
L8c668488: r[4]=rd(m,0x8c6686ccu,4); /* mov.l 0x8c6686cc,r4 */
 goto L8c66848a;
L8c66848a: target=r[3];*pr=0x8c66848eu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66848e;
L8c66848e: r[0]=r[14]; /* mov r14,r0 */
 goto L8c668490;
L8c668490: r[15]+=4u; /* add #0x4,r15 */
 goto L8c668492;
L8c668492: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c668494;
L8c668494: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
