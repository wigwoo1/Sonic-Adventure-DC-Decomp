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
/* Retail contiguous-capacity check 8c65e712..8c65e798. R4 source,
 * R5 mode, R6 requested bytes, R7 output count. Signed minimum of request,
 * capacity and ring remainder plus mirror; invalid mode reports error -3. */
SA_STAGE_EXPORT int sa_sound_source_contiguous_check(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65e712;
L8c65e712: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65e714;
L8c65e714: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65e716;
L8c65e716: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65e718;
L8c65e718: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65e71a;
L8c65e71a: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c65e71c;
L8c65e71c: r[12]=r[6]; /* mov r6,r12 */
 goto L8c65e71e;
L8c65e71e: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c65e720;
L8c65e720: wr(m,r[15]+0x4u,r[7],4); /* mov.l r7,@(0x4,r15) */
 goto L8c65e722;
L8c65e722: *pr=0x8c65e726u;r[14]=r[4];status=call(ctx,0x8c65e368u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65e368 */
 goto L8c65e726;
L8c65e726: wr(m,r[15]+0x8u,r[0],4); /* mov.l r0,@(0x8,r15) */
 goto L8c65e728;
L8c65e728: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65e72a;
L8c65e72a: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e72c;
L8c65e72c: if(!(*sr&1u))goto L8c65e746; /* bf 0x8c65e746 */
 goto L8c65e72e;
L8c65e72e: r[5]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r5 */
 goto L8c65e730;
L8c65e730: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto L8c65e732;
L8c65e732: r[2]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r2 */
 goto L8c65e734;
L8c65e734: r[5]-=r[3]; /* sub r3,r5 */
 goto L8c65e736;
L8c65e736: r[6]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r6 */
 goto L8c65e738;
L8c65e738: r[5]+=r[2]; /* add r2,r5 */
 goto L8c65e73a;
L8c65e73a: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[5]); /* cmp/ge r5,r6 */
 goto L8c65e73c;
L8c65e73c: if((*sr&1u))goto L8c65e742; /* bt 0x8c65e742 */
 goto L8c65e73e;
L8c65e73e: r[4]=r[6];goto L8c65e762; /* bra 0x8c65e762 */
 goto L8c65e742;
L8c65e742: r[4]=r[5];goto L8c65e762; /* bra 0x8c65e762 */
 goto L8c65e746;
L8c65e746: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c65e748;
L8c65e748: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c65e74a;
L8c65e74a: if(!(*sr&1u))goto L8c65e76e; /* bf 0x8c65e76e */
 goto L8c65e74c;
L8c65e74c: r[6]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r6 */
 goto L8c65e74e;
L8c65e74e: r[3]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r3 */
 goto L8c65e750;
L8c65e750: r[2]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r2 */
 goto L8c65e752;
L8c65e752: r[6]-=r[3]; /* sub r3,r6 */
 goto L8c65e754;
L8c65e754: r[5]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r5 */
 goto L8c65e756;
L8c65e756: r[6]+=r[2]; /* add r2,r6 */
 goto L8c65e758;
L8c65e758: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]); /* cmp/ge r6,r5 */
 goto L8c65e75a;
L8c65e75a: if((*sr&1u))goto L8c65e760; /* bt 0x8c65e760 */
 goto L8c65e75c;
L8c65e75c: r[4]=r[5];goto L8c65e762; /* bra 0x8c65e762 */
 goto L8c65e760;
L8c65e760: r[4]=r[6]; /* mov r6,r4 */
 goto L8c65e762;
L8c65e762: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[12]); /* cmp/ge r12,r4 */
 goto L8c65e764;
L8c65e764: if((*sr&1u))goto L8c65e76a; /* bt 0x8c65e76a */
 goto L8c65e766;
L8c65e766: r[13]=r[4];goto L8c65e77e; /* bra 0x8c65e77e */
 goto L8c65e76a;
L8c65e76a: r[13]=r[12];goto L8c65e77e; /* bra 0x8c65e77e */
 goto L8c65e76e;
L8c65e76e: r[3]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r3 */
 goto L8c65e770;
L8c65e770: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e772;
L8c65e772: take=(*sr&1u);r[13]=0u;if(take)goto L8c65e77e;goto L8c65e776; /* bt/s 0x8c65e77e */
 goto L8c65e776;
L8c65e776: r[3]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r3 */
 goto L8c65e778;
L8c65e778: r[5]=4294967293u; /* mov #-0x3,r5 */
 goto L8c65e77a;
L8c65e77a: target=r[3];*pr=0x8c65e77eu;r[4]=rd(m,r[14]+0x2cu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65e77e;
L8c65e77e: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c65e780;
L8c65e780: wr(m,r[2],r[13],4); /* mov.l r13,@r2 */
 goto L8c65e782;
L8c65e782: r[5]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r5 */
 goto L8c65e784;
L8c65e784: *pr=0x8c65e788u;r[4]=r[14];status=call(ctx,0x8c65e380u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65e380 */
 goto L8c65e788;
L8c65e788: *sr=(*sr&~1u)|(r[12]==r[13]); /* cmp/eq r12,r13 */
 goto L8c65e78a;
L8c65e78a: r[0]=*sr&1u; /* movt r0 */
 goto L8c65e78c;
L8c65e78c: r[15]+=12u; /* add #0xc,r15 */
 goto L8c65e78e;
L8c65e78e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e790;
L8c65e790: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65e792;
L8c65e792: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65e794;
L8c65e794: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
