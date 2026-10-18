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
/* Static C recovery of USA Rev A transfer completion. Use IO specialization for interrupt acknowledgement. */
SA_STAGE_EXPORT int sa_engine_transfer_complete(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ebe8;
L8c10ebe8: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c10ebea;
L8c10ebea: r[14]=r[4]; /* mov r4,r14 */
 goto L8c10ebec;
L8c10ebec: r[4]=rd(m,0x8c10ec38u,4); /* mov.l 0x8c10ec38,r4 */
 goto L8c10ebee;
L8c10ebee: r[6]=0u; /* mov #0x0,r6 */
 goto L8c10ebf0;
L8c10ebf0: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c10ebf2;
L8c10ebf2: r[5]=rd(m,r[4],4); /* mov.l @r4,r5 */
 goto L8c10ebf4;
L8c10ebf4: r[3]<<=2; /* shll2 r3 */
 goto L8c10ebf6;
L8c10ebf6: r[2]=sx(m,0x8c10ec36u,2); /* mov.w 0x8c10ec36,r2 */
 goto L8c10ebf8;
L8c10ebf8: r[3]<<=2; /* shll2 r3 */
 goto L8c10ebfa;
L8c10ebfa: r[5]=rd(m,r[5],4); /* mov.l @r5,r5 */
 goto L8c10ebfc;
L8c10ebfc: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c10ebfe;
L8c10ebfe: r[5]+=r[3]; /* add r3,r5 */
 goto L8c10ec00;
L8c10ec00: wr(m,r[5]+0x14u,r[6],4); /* mov.l r6,@(0x14,r5) */
 goto L8c10ec02;
L8c10ec02: r[5]=1u; /* mov #0x1,r5 */
 goto L8c10ec04;
L8c10ec04: wr(m,r[14]+0x2cu,r[6],4); /* mov.l r6,@(0x2c,r14) */
 goto L8c10ec06;
L8c10ec06: wr(m,r[14]+0x30u,r[5],4); /* mov.l r5,@(0x30,r14) */
 goto L8c10ec08;
L8c10ec08: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c10ec0a;
L8c10ec0a: r[0]=rd(m,r[4],4); /* mov.l @r4,r0 */
 goto L8c10ec0c;
L8c10ec0c: r[3]<<=2; /* shll2 r3 */
 goto L8c10ec0e;
L8c10ec0e: r[2]+=r[0]; /* add r0,r2 */
 goto L8c10ec10;
L8c10ec10: r[3]+=r[2]; /* add r2,r3 */
 goto L8c10ec12;
L8c10ec12: r[2]=rd(m,0x8c10ec3cu,4); /* mov.l 0x8c10ec3c,r2 */
 goto L8c10ec14;
L8c10ec14: r[1]=rd(m,r[3],4); /* mov.l @r3,r1 */
 goto L8c10ec16;
L8c10ec16: r[1]+=1u; /* add #0x1,r1 */
 goto L8c10ec18;
L8c10ec18: wr(m,r[3],r[1],4); /* mov.l r1,@r3 */
 goto L8c10ec1a;
L8c10ec1a: r[1]=rd(m,r[14],4); /* mov.l @r14,r1 */
 goto L8c10ec1c;
L8c10ec1c: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c10ec1e;
L8c10ec1e: r[1]+=15u; /* add #0xf,r1 */
 goto L8c10ec20;
L8c10ec20: {uint32_t shift=r[1];if(!(shift&0x80000000u))r[5]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[5]=n?(uint32_t)((int32_t)r[5]>>n):((r[5]&0x80000000u)?0xffffffffu:0u);}} /* shad r1,r5 */
 goto L8c10ec22;
L8c10ec22: wr(m,r[3],r[5],4); /* mov.l r5,@r3 */
 goto L8c10ec24;
L8c10ec24: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c10ec26;
L8c10ec26: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c10ec28;
L8c10ec28: if((*sr&1u))goto L8c10ec32; /* bt 0x8c10ec32 */
 goto L8c10ec2a;
L8c10ec2a: r[4]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r4 */
 goto L8c10ec2c;
L8c10ec2c: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c10ec2e;
L8c10ec2e: target=r[3];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
L8c10ec32: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
