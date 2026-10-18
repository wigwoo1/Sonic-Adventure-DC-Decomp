#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
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
/* Recovered sound request producer/copy helper; external calls dispatch to recovered native routines. */
SA_STAGE_EXPORT int sa_sound_request_copy20(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10cd82;
L8c10cd82: r[0]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r0 */
 goto L8c10cd84;
L8c10cd84: wr(m,r[1]+0x10u,r[3],4); /* mov.l r3,@(0x10,r1) */
 goto L8c10cd86;
L8c10cd86: r[3]=rd(m,r[2]+0x8u,4); /* mov.l @(0x8,r2),r3 */
 goto L8c10cd88;
L8c10cd88: wr(m,r[1]+0xcu,r[0],4); /* mov.l r0,@(0xc,r1) */
 goto L8c10cd8a;
L8c10cd8a: r[0]=rd(m,r[2]+0x4u,4); /* mov.l @(0x4,r2),r0 */
 goto L8c10cd8c;
L8c10cd8c: wr(m,r[1]+0x8u,r[3],4); /* mov.l r3,@(0x8,r1) */
 goto L8c10cd8e;
L8c10cd8e: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c10cd90;
L8c10cd90: wr(m,r[1]+0x4u,r[0],4); /* mov.l r0,@(0x4,r1) */
 goto L8c10cd92;
L8c10cd92: wr(m,r[1],r[3],4); /* mov.l r3,@r1 */
 goto L8c10cd94;
L8c10cd94: r[3]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
