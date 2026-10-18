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
/* Retail linear-source contiguous-capacity check 8c65e302..8c65e35e. */
SA_STAGE_EXPORT int sa_sound_linear_source_contiguous_check(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65e302;
L8c65e302: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65e304;
L8c65e304: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65e306;
L8c65e306: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65e308;
L8c65e308: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65e30a;
L8c65e30a: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c65e30c;
L8c65e30c: r[12]=r[6]; /* mov r6,r12 */
 goto L8c65e30e;
L8c65e30e: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c65e310;
L8c65e310: wr(m,r[15]+0x4u,r[7],4); /* mov.l r7,@(0x4,r15) */
 goto L8c65e312;
L8c65e312: *pr=0x8c65e316u;r[14]=r[4];status=call(ctx,0x8c65e08cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65e08c */
 goto L8c65e316;
L8c65e316: wr(m,r[15]+0x8u,r[0],4); /* mov.l r0,@(0x8,r15) */
 goto L8c65e318;
L8c65e318: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65e31a;
L8c65e31a: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e31c;
L8c65e31c: take=(*sr&1u);r[4]=0u;if(take)goto L8c65e32c;goto L8c65e320; /* bt/s 0x8c65e32c */
 goto L8c65e320;
L8c65e320: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c65e322;
L8c65e322: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c65e324;
L8c65e324: if(!(*sr&1u))goto L8c65e334; /* bf 0x8c65e334 */
 goto L8c65e326;
L8c65e326: r[4]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r4 */
 goto L8c65e328;
L8c65e328: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[12]); /* cmp/ge r12,r4 */
 goto L8c65e32a;
L8c65e32a: if((*sr&1u))goto L8c65e330; /* bt 0x8c65e330 */
 goto L8c65e32c;
L8c65e32c: r[13]=r[4];goto L8c65e344; /* bra 0x8c65e344 */
 goto L8c65e330;
L8c65e330: r[13]=r[12];goto L8c65e344; /* bra 0x8c65e344 */
 goto L8c65e334;
L8c65e334: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65e336;
L8c65e336: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e338;
L8c65e338: take=(*sr&1u);r[13]=r[4];if(take)goto L8c65e344;goto L8c65e33c; /* bt/s 0x8c65e344 */
 goto L8c65e33c;
L8c65e33c: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65e33e;
L8c65e33e: r[5]=4294967293u; /* mov #-0x3,r5 */
 goto L8c65e340;
L8c65e340: target=r[3];*pr=0x8c65e344u;r[4]=rd(m,r[14]+0x20u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65e344;
L8c65e344: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c65e346;
L8c65e346: wr(m,r[2],r[13],4); /* mov.l r13,@r2 */
 goto L8c65e348;
L8c65e348: r[5]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r5 */
 goto L8c65e34a;
L8c65e34a: *pr=0x8c65e34eu;r[4]=r[14];status=call(ctx,0x8c65e0a4u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65e0a4 */
 goto L8c65e34e;
L8c65e34e: *sr=(*sr&~1u)|(r[12]==r[13]); /* cmp/eq r12,r13 */
 goto L8c65e350;
L8c65e350: r[0]=*sr&1u; /* movt r0 */
 goto L8c65e352;
L8c65e352: r[15]+=12u; /* add #0xc,r15 */
 goto L8c65e354;
L8c65e354: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e356;
L8c65e356: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65e358;
L8c65e358: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65e35a;
L8c65e35a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
