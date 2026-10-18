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
/* Retail device word write66ac3c; transfer10d770 external. */
SA_STAGE_EXPORT int sa_device_word_write(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66ac3c;
L8c66ac3c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66ac3e;
L8c66ac3e: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c66ac40;
L8c66ac40: r[3]=rd(m,0x8c66ac70u,4); /* mov.l 0x8c66ac70,r3 */
 goto L8c66ac42;
L8c66ac42: r[2]=r[4]; /* mov r4,r2 */
 goto L8c66ac44;
L8c66ac44: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c66ac46;
L8c66ac46: *sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]); /* cmp/ge r3,r2 */
 goto L8c66ac48;
L8c66ac48: take=(*sr&1u);wr(m,r[15],r[4],4);if(take)goto L8c66ac66;goto L8c66ac4c; /* bt/s 0x8c66ac66 */
 goto L8c66ac4c;
L8c66ac4c: r[2]=0u; /* mov #0x0,r2 */
 goto L8c66ac4e;
L8c66ac4e: r[1]=rd(m,0x8c66ac74u,4); /* mov.l 0x8c66ac74,r1 */
 goto L8c66ac50;
L8c66ac50: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c66ac52;
L8c66ac52: r[7]=1u; /* mov #0x1,r7 */
 goto L8c66ac54;
L8c66ac54: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c66ac56;
L8c66ac56: r[6]=4u; /* mov #0x4,r6 */
 goto L8c66ac58;
L8c66ac58: r[5]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r5 */
 goto L8c66ac5a;
L8c66ac5a: r[4]=r[15]; /* mov r15,r4 */
 goto L8c66ac5c;
L8c66ac5c: r[5]+=r[1]; /* add r1,r5 */
 goto L8c66ac5e;
L8c66ac5e: r[1]=rd(m,0x8c66ac78u,4); /* mov.l 0x8c66ac78,r1 */
 goto L8c66ac60;
L8c66ac60: target=r[1];*pr=0x8c66ac64u;r[4]+=12u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c66ac64;
L8c66ac64: r[15]+=8u; /* add #0x8,r15 */
 goto L8c66ac66;
L8c66ac66: r[15]+=8u; /* add #0x8,r15 */
 goto L8c66ac68;
L8c66ac68: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66ac6a;
L8c66ac6a: r[0]=0u;return 0; /* rts */
}
