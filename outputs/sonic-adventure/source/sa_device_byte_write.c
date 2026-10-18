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
/* Retail device byte read/modify/write; read66910c/write66ac3c external. */
SA_STAGE_EXPORT int sa_device_byte_write(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c669144;
L8c669144: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c669146;
L8c669146: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c669148;
L8c669148: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66914a;
L8c66914a: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c66914c;
L8c66914c: r[3]=rd(m,0x8c669188u,4); /* mov.l 0x8c669188,r3 */
 goto L8c66914e;
L8c66914e: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]); /* cmp/ge r3,r4 */
 goto L8c669150;
L8c669150: take=(*sr&1u);wr(m,r[15],r[5],1);if(take)goto L8c66917a;goto L8c669154; /* bt/s 0x8c66917a */
 goto L8c669154;
L8c669154: r[13]=r[15]; /* mov r15,r13 */
 goto L8c669156;
L8c669156: r[1]=3u; /* mov #0x3,r1 */
 goto L8c669158;
L8c669158: r[14]=r[4]; /* mov r4,r14 */
 goto L8c66915a;
L8c66915a: r[13]+=8u; /* add #0x8,r13 */
 goto L8c66915c;
L8c66915c: r[3]=rd(m,0x8c66918cu,4); /* mov.l 0x8c66918c,r3 */
 goto L8c66915e;
L8c66915e: r[2]=4294967292u; /* mov #-0x4,r2 */
 goto L8c669160;
L8c669160: r[1]&=r[14]; /* and r14,r1 */
 goto L8c669162;
L8c669162: r[5]=r[13]; /* mov r13,r5 */
 goto L8c669164;
L8c669164: r[14]&=r[2]; /* and r2,r14 */
 goto L8c669166;
L8c669166: wr(m,r[15]+0x4u,r[1],4); /* mov.l r1,@(0x4,r15) */
 goto L8c669168;
L8c669168: target=r[3];*pr=0x8c66916cu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66916c;
L8c66916c: r[3]=sx(m,r[15],1); /* mov.b @r15,r3 */
 goto L8c66916e;
L8c66916e: r[0]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r0 */
 goto L8c669170;
L8c669170: wr(m,r[13]+r[0],r[3],1); /* mov.b r3,@(r0,r13) */
 goto L8c669172;
L8c669172: r[3]=rd(m,0x8c669190u,4); /* mov.l 0x8c669190,r3 */
 goto L8c669174;
L8c669174: r[5]=rd(m,r[13],4); /* mov.l @r13,r5 */
 goto L8c669176;
L8c669176: target=r[3];*pr=0x8c66917au;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66917a;
L8c66917a: r[0]=0u; /* mov #0x0,r0 */
 goto L8c66917c;
L8c66917c: r[15]+=12u; /* add #0xc,r15 */
 goto L8c66917e;
L8c66917e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c669180;
L8c669180: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c669182;
L8c669182: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
