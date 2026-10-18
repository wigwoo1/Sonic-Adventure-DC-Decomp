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
/* Retail device word read 66910c; strided transfer10d460 external. */
SA_STAGE_EXPORT int sa_device_word_read(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66910c;
L8c66910c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66910e;
L8c66910e: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c669110;
L8c669110: r[3]=rd(m,0x8c669138u,4); /* mov.l 0x8c669138,r3 */
 goto L8c669112;
L8c669112: r[2]=r[4]; /* mov r4,r2 */
 goto L8c669114;
L8c669114: *sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]); /* cmp/ge r3,r2 */
 goto L8c669116;
L8c669116: take=(*sr&1u);wr(m,r[15],r[4],4);if(take)goto L8c669130;goto L8c66911a; /* bt/s 0x8c669130 */
 goto L8c66911a;
L8c66911a: r[2]=0u; /* mov #0x0,r2 */
 goto L8c66911c;
L8c66911c: r[1]=rd(m,0x8c66913cu,4); /* mov.l 0x8c66913c,r1 */
 goto L8c66911e;
L8c66911e: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c669120;
L8c669120: r[7]=1u; /* mov #0x1,r7 */
 goto L8c669122;
L8c669122: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c669124;
L8c669124: r[4]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r4 */
 goto L8c669126;
L8c669126: r[4]+=r[1]; /* add r1,r4 */
 goto L8c669128;
L8c669128: r[1]=rd(m,0x8c669140u,4); /* mov.l 0x8c669140,r1 */
 goto L8c66912a;
L8c66912a: target=r[1];*pr=0x8c66912eu;r[6]=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c66912e;
L8c66912e: r[15]+=8u; /* add #0x8,r15 */
 goto L8c669130;
L8c669130: r[15]+=4u; /* add #0x4,r15 */
 goto L8c669132;
L8c669132: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c669134;
L8c669134: r[0]=0u;return 0; /* rts */
}
