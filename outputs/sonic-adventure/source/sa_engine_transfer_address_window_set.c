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
/* Static C recovery of USA Rev A transfer initialization. Hardware accesses require explicit IO specialization. */
SA_STAGE_EXPORT int sa_engine_transfer_address_window_set(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ee64;
L8c10ee64: r[6]=rd(m,0x8c10ee8cu,4); /* mov.l 0x8c10ee8c,r6 */
 goto L8c10ee66;
L8c10ee66: r[3]=rd(m,0x8c10ee90u,4); /* mov.l 0x8c10ee90,r3 */
 goto L8c10ee68;
L8c10ee68: r[4]&=r[6]; /* and r6,r4 */
 goto L8c10ee6a;
L8c10ee6a: r[2]=rd(m,0x8c10ee94u,4); /* mov.l 0x8c10ee94,r2 */
 goto L8c10ee6c;
L8c10ee6c: r[4]>>=16; /* shlr16 r4 */
 goto L8c10ee6e;
L8c10ee6e: r[5]&=r[6]; /* and r6,r5 */
 goto L8c10ee70;
L8c10ee70: r[4]>>=2; /* shlr2 r4 */
 goto L8c10ee72;
L8c10ee72: r[5]>>=16; /* shlr16 r5 */
 goto L8c10ee74;
L8c10ee74: r[4]>>=2; /* shlr2 r4 */
 goto L8c10ee76;
L8c10ee76: r[4]<<=8; /* shll8 r4 */
 goto L8c10ee78;
L8c10ee78: r[5]>>=2; /* shlr2 r5 */
 goto L8c10ee7a;
L8c10ee7a: r[6]=r[4]; /* mov r4,r6 */
 goto L8c10ee7c;
L8c10ee7c: r[5]>>=2; /* shlr2 r5 */
 goto L8c10ee7e;
L8c10ee7e: r[6]|=r[5]; /* or r5,r6 */
 goto L8c10ee80;
L8c10ee80: r[6]|=r[3]; /* or r3,r6 */
 goto L8c10ee82;
L8c10ee82: wr(m,r[2],r[6],4); /* mov.l r6,@r2 */
 goto L8c10ee84;
L8c10ee84: r[1]=rd(m,0x8c10ee98u,4); /* mov.l 0x8c10ee98,r1 */
 goto L8c10ee86;
L8c10ee86: wr(m,r[1],r[6],4); /* mov.l r6,@r1 */
 goto L8c10ee88;
L8c10ee88: r[0]=0u;return 0; /* rts */
}
