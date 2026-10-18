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
/* Static C recovery of retail Tikal helper 8c644ff6..8c645076.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_device_read(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c644ff6;
L8c644ff6: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c644ff8;
L8c644ff8: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c644ffa;
L8c644ffa: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c644ffc;
L8c644ffc: r[15]+=4294967276u; /* add #-0x14,r15 */
 goto L8c644ffe;
L8c644ffe: r[14]=r[15]; /* mov r15,r14 */
 goto L8c645000;
L8c645000: r[14]+=4u; /* add #0x4,r14 */
 goto L8c645002;
L8c645002: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c645004;
L8c645004: r[13]=r[6]; /* mov r6,r13 */
 goto L8c645006;
L8c645006: r[2]=0u; /* mov #0x0,r2 */
 goto L8c645008;
L8c645008: r[6]=rd(m,0x8c6451dcu,4); /* mov.l 0x8c6451dc,r6 */
 goto L8c64500a;
L8c64500a: r[5]=r[14]; /* mov r14,r5 */
 goto L8c64500c;
L8c64500c: r[0]=r[7]; /* mov r7,r0 */
 goto L8c64500e;
L8c64500e: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c645010;
L8c645010: r[5]+=8u; /* add #0x8,r5 */
 goto L8c645012;
L8c645012: wr(m,r[14],r[4],4); /* mov.l r4,@r14 */
 goto L8c645014;
L8c645014: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c645016;
L8c645016: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c645018;
L8c645018: wr(m,r[14]+0xcu,r[2],4); /* mov.l r2,@(0xc,r14) */
 goto L8c64501a;
L8c64501a: take=!(*sr&1u);r[6]&=r[13];if(take)goto L8c645022;goto L8c64501e; /* bf/s 0x8c645022 */
 goto L8c64501e;
L8c64501e: r[4]=17u;goto L8c64502c; /* bra 0x8c64502c */
 goto L8c645022;
L8c645022: *sr=(*sr&~1u)|!(r[7]&r[7]); /* tst r7,r7 */
 goto L8c645024;
L8c645024: if(!(*sr&1u))goto L8c645030; /* bf 0x8c645030 */
 goto L8c645026;
L8c645026: r[2]=rd(m,0x8c6451e0u,4); /* mov.l 0x8c6451e0,r2 */
 goto L8c645028;
L8c645028: r[4]=16u; /* mov #0x10,r4 */
 goto L8c64502a;
L8c64502a: r[6]|=r[2]; /* or r2,r6 */
 goto L8c64502c;
L8c64502c: wr(m,r[5],r[6],4);goto L8c645034; /* bra 0x8c645034 */
 goto L8c645030;
L8c645030: r[0]=4294967278u;goto L8c64506c; /* bra 0x8c64506c */
 goto L8c645034;
L8c645034: r[2]=rd(m,0x8c6451e4u,4); /* mov.l 0x8c6451e4,r2 */
 goto L8c645036;
L8c645036: target=r[2];*pr=0x8c64503au;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c64503a;
L8c64503a: r[14]=r[0]; /* mov r0,r14 */
 goto L8c64503c;
L8c64503c: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c64503e;
L8c64503e: if((*sr&1u))goto L8c64506a; /* bt 0x8c64506a */
 goto L8c645040;
L8c645040: r[3]=4294967268u; /* mov #-0x1c,r3 */
 goto L8c645042;
L8c645042: r[0]=r[13]; /* mov r13,r0 */
 goto L8c645044;
L8c645044: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c645046;
L8c645046: r[0]&=0xeu; /* and #0xe,r0 */
 goto L8c645048;
L8c645048: *sr=(*sr&~1u)|(r[0]==0xau); /* cmp/eq #0xa,r0 */
 goto L8c64504a;
L8c64504a: if((*sr&1u))goto L8c64506a; /* bt 0x8c64506a */
 goto L8c64504c;
L8c64504c: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c64504e;
L8c64504e: r[3]=31u; /* mov #0x1f,r3 */
 goto L8c645050;
L8c645050: r[5]<<=8; /* shll8 r5 */
 goto L8c645052;
L8c645052: r[5]<<=2; /* shll2 r5 */
 goto L8c645054;
L8c645054: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1; /* shll r5 */
 goto L8c645056;
L8c645056: *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c645058;
L8c645058: if(!(*sr&1u))goto L8c645064; /* bf 0x8c645064 */
 goto L8c64505a;
L8c64505a: r[3]=rd(m,0x8c6451e8u,4); /* mov.l 0x8c6451e8,r3 */
 goto L8c64505c;
L8c64505c: target=r[3];*pr=0x8c645060u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c645060;
L8c645060: goto L8c64506a; /* bra 0x8c64506a */
 goto L8c645064;
L8c645064: r[3]=rd(m,0x8c6451ecu,4); /* mov.l 0x8c6451ec,r3 */
 goto L8c645066;
L8c645066: target=r[3];*pr=0x8c64506au;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c64506a;
L8c64506a: r[0]=r[14]; /* mov r14,r0 */
 goto L8c64506c;
L8c64506c: r[15]+=20u; /* add #0x14,r15 */
 goto L8c64506e;
L8c64506e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c645070;
L8c645070: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c645072;
L8c645072: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
