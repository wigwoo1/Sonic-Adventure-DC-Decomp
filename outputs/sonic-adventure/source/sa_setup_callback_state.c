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
/* Static C recovery of retail Tikal helper 8c051760..8c051784.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_setup_callback_state(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c051760;
L8c051760: r[0]=rd(m,0x8c05188cu,4); /* mov.l 0x8c05188c,r0 */
 goto L8c051762;
L8c051762: r[3]=r[4]; /* mov r4,r3 */
 goto L8c051764;
L8c051764: r[2]=rd(m,0x8c051884u,4); /* mov.l 0x8c051884,r2 */
 goto L8c051766;
L8c051766: r[3]=0u; /* mov #0x0,r3 */
 goto L8c051768;
L8c051768: r[6]=rd(m,0x8c051880u,4); /* mov.l 0x8c051880,r6 */
 goto L8c05176a;
L8c05176a: r[1]=rd(m,0x8c051888u,4); /* mov.l 0x8c051888,r1 */
 goto L8c05176c;
L8c05176c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c05176e;
L8c05176e: wr(m,r[6],r[4],4); /* mov.l r4,@r6 */
 goto L8c051770;
L8c051770: wr(m,r[2],r[4],4); /* mov.l r4,@r2 */
 goto L8c051772;
L8c051772: r[4]=rd(m,0x8c051890u,4); /* mov.l 0x8c051890,r4 */
 goto L8c051774;
L8c051774: wr(m,r[1],r[5],4); /* mov.l r5,@r1 */
 goto L8c051776;
L8c051776: wr(m,r[0],r[3],4); /* mov.l r3,@r0 */
 goto L8c051778;
L8c051778: r[3]=rd(m,0x8c051894u,4); /* mov.l 0x8c051894,r3 */
 goto L8c05177a;
L8c05177a: target=r[3];*pr=0x8c05177eu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c05177e;
L8c05177e: r[2]=rd(m,0x8c051898u,4); /* mov.l 0x8c051898,r2 */
 goto L8c051780;
L8c051780: target=r[2];*pr=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
}
