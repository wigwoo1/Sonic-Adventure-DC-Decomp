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
/* USA Rev A sound-work cancellation by identity0945a0. Scans38 records; name provisional. */
SA_STAGE_EXPORT int sa_sound_voice_cancel_identity(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c0945a0;
L8c0945a0: r[1]=8u; /* mov #0x8,r1 */
 goto L8c0945a2;
L8c0945a2: r[7]=rd(m,0x8c094700u,4); /* mov.l 0x8c094700,r7 */
 goto L8c0945a4;
L8c0945a4: r[1]<<=8; /* shll8 r1 */
 goto L8c0945a6;
L8c0945a6: r[1]+=4294967224u; /* add #-0x48,r1 */
 goto L8c0945a8;
L8c0945a8: r[5]=r[7]; /* mov r7,r5 */
 goto L8c0945aa;
L8c0945aa: r[1]+=r[7]; /* add r7,r1 */
 goto L8c0945ac;
L8c0945ac: r[6]=r[7]; /* mov r7,r6 */
 goto L8c0945ae;
L8c0945ae: *sr=(*sr&~1u)|(r[5]>=r[1]); /* cmp/hs r1,r5 */
 goto L8c0945b0;
L8c0945b0: r[0]=0u; /* mov #0x0,r0 */
 goto L8c0945b2;
L8c0945b2: take=(*sr&1u);r[7]=16u;if(take)goto L8c0945cc;goto L8c0945b6; /* bt/s 0x8c0945cc */
 goto L8c0945b6;
L8c0945b6: r[2]=rd(m,r[5]+0x10u,4); /* mov.l @(0x10,r5),r2 */
 goto L8c0945b8;
L8c0945b8: *sr=(*sr&~1u)|(r[4]==r[2]); /* cmp/eq r4,r2 */
 goto L8c0945ba;
L8c0945ba: if(!(*sr&1u))goto L8c0945c4; /* bf 0x8c0945c4 */
 goto L8c0945bc;
L8c0945bc: wr(m,r[6]+0x4u,r[0],4); /* mov.l r0,@(0x4,r6) */
 goto L8c0945be;
L8c0945be: r[2]=rd(m,r[6]+0xcu,4); /* mov.l @(0xc,r6),r2 */
 goto L8c0945c0;
L8c0945c0: r[2]|=r[7]; /* or r7,r2 */
 goto L8c0945c2;
L8c0945c2: wr(m,r[6]+0xcu,r[2],4); /* mov.l r2,@(0xc,r6) */
 goto L8c0945c4;
L8c0945c4: r[5]+=52u; /* add #0x34,r5 */
 goto L8c0945c6;
L8c0945c6: *sr=(*sr&~1u)|(r[5]>=r[1]); /* cmp/hs r1,r5 */
 goto L8c0945c8;
L8c0945c8: take=!(*sr&1u);r[6]+=52u;if(take)goto L8c0945b6;goto L8c0945cc; /* bf/s 0x8c0945b6 */
 goto L8c0945cc;
L8c0945cc: return 0; /* rts */
}
