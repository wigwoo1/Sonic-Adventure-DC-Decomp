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
/* Retail default stream input callback667384..6673a2.
 * Explicit SH4 ABI. Position and capacity counters use unsigned subtraction.
 * Verified scope: sound_stream_default_input_verification.json. */
SA_STAGE_EXPORT int sa_sound_stream_default_input(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c667384;
L8c667384: r[0]=sx(m,0x8c667466u,2); /* mov.w 0x8c667466,r0 */
 goto L8c667386;
L8c667386: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c667388;
L8c667388: wr(m,r[5],r[3],4); /* mov.l r3,@r5 */
 goto L8c66738a;
L8c66738a: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c66738c;
L8c66738c: r[0]=64u; /* mov #0x40,r0 */
 goto L8c66738e;
L8c66738e: r[2]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r2 */
 goto L8c667390;
L8c667390: r[0]+=88u; /* add #0x58,r0 */
 goto L8c667392;
L8c667392: r[2]-=r[3]; /* sub r3,r2 */
 goto L8c667394;
L8c667394: wr(m,r[6],r[2],4); /* mov.l r2,@r6 */
 goto L8c667396;
L8c667396: r[1]=rd(m,r[4]+0x18u,4); /* mov.l @(0x18,r4),r1 */
 goto L8c667398;
L8c667398: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c66739a;
L8c66739a: r[1]-=r[3]; /* sub r3,r1 */
 goto L8c66739c;
L8c66739c: wr(m,r[7],r[1],4); /* mov.l r1,@r7 */
 goto L8c66739e;
L8c66739e: r[0]=rd(m,r[4]+0x3cu,4);return 0; /* rts */
}
