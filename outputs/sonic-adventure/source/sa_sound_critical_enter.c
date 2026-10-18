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
int sa_sh4_sqrt(uint32_t,uint32_t *,uint32_t *);
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
/* Static C recovery of retail Tikal helper 8c65ad68..8c65ad8e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_critical_enter(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65ad68;
L8c65ad68: r[4]=rd(m,0x8c65adb8u,4); /* mov.l 0x8c65adb8,r4 */
 goto L8c65ad6a;
L8c65ad6a: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c65ad6c;
L8c65ad6c: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65ad6e;
L8c65ad6e: if(!(*sr&1u))goto L8c65ad86; /* bf 0x8c65ad86 */
 goto L8c65ad70;
L8c65ad70: r[0]=*sr; /* stc SR,r0 */
 goto L8c65ad72;
L8c65ad72: r[3]=rd(m,0x8c65adbcu,4); /* mov.l 0x8c65adbc,r3 */
 goto L8c65ad74;
L8c65ad74: r[2]=sx(m,0x8c65adb4u,2); /* mov.w 0x8c65adb4,r2 */
 goto L8c65ad76;
L8c65ad76: r[0]>>=2; /* shlr2 r0 */
 goto L8c65ad78;
L8c65ad78: r[0]>>=2; /* shlr2 r0 */
 goto L8c65ad7a;
L8c65ad7a: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c65ad7c;
L8c65ad7c: wr(m,r[3],r[0],4); /* mov.l r0,@r3 */
 goto L8c65ad7e;
L8c65ad7e: r[0]=*sr; /* stc SR,r0 */
 goto L8c65ad80;
L8c65ad80: r[0]&=r[2]; /* and r2,r0 */
 goto L8c65ad82;
L8c65ad82: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c65ad84;
L8c65ad84: *sr=r[0]; /* ldc r0,SR */
 goto L8c65ad86;
L8c65ad86: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c65ad88;
L8c65ad88: r[3]+=1u; /* add #0x1,r3 */
 goto L8c65ad8a;
L8c65ad8a: wr(m,r[4],r[3],4);return 0; /* rts */
}
