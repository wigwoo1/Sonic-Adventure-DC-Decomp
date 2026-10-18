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
/* Retail decode handle allocate with explicit SH4 ABI.
 * Verified scope: file_handle_pool_verification.json. */
SA_STAGE_EXPORT int sa_file_decode_handle_allocate(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c644b6e;
L8c644b6e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c644b70;
L8c644b70: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c644b72;
L8c644b72: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c644b74;
L8c644b74: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c644b76;
L8c644b76: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c644b78;
L8c644b78: r[5]=rd(m,0x8c644c9cu,4); /* mov.l 0x8c644c9c,r5 */
 goto L8c644b7a;
L8c644b7a: r[12]=52u; /* mov #0x34,r12 */
 goto L8c644b7c;
L8c644b7c: r[4]=8u; /* mov #0x8,r4 */
 goto L8c644b7e;
L8c644b7e: r[14]=0u; /* mov #0x0,r14 */
 goto L8c644b80;
L8c644b80: *macl=r[12]*r[14]; /* mul.l r12,r14 */
 goto L8c644b82;
L8c644b82: r[3]=*macl; /* sts MACL,r3 */
 goto L8c644b84;
L8c644b84: r[3]+=r[5]; /* add r5,r3 */
 goto L8c644b86;
L8c644b86: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto L8c644b88;
L8c644b88: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c644b8a;
L8c644b8a: if((*sr&1u))goto L8c644b92; /* bt 0x8c644b92 */
 goto L8c644b8c;
L8c644b8c: r[14]+=1u; /* add #0x1,r14 */
 goto L8c644b8e;
L8c644b8e: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[4]); /* cmp/ge r4,r14 */
 goto L8c644b90;
L8c644b90: if(!(*sr&1u))goto L8c644b80; /* bf 0x8c644b80 */
 goto L8c644b92;
L8c644b92: r[0]=r[14]; /* mov r14,r0 */
 goto L8c644b94;
L8c644b94: *sr=(*sr&~1u)|(r[0]==0x00000008u); /* cmp/eq #0x8,r0 */
 goto L8c644b96;
L8c644b96: if(!(*sr&1u))goto L8c644b9c; /* bf 0x8c644b9c */
 goto L8c644b98;
L8c644b98: r[0]=0u;goto L8c644bda; /* bra 0x8c644bda */
 goto L8c644b9c;
L8c644b9c: *macl=r[12]*r[14]; /* mul.l r12,r14 */
 goto L8c644b9e;
L8c644b9e: r[3]=rd(m,0x8c644ca0u,4); /* mov.l 0x8c644ca0,r3 */
 goto L8c644ba0;
L8c644ba0: r[6]=52u; /* mov #0x34,r6 */
 goto L8c644ba2;
L8c644ba2: r[13]=*macl; /* sts MACL,r13 */
 goto L8c644ba4;
L8c644ba4: r[13]+=r[5]; /* add r5,r13 */
 goto L8c644ba6;
L8c644ba6: r[5]=0u; /* mov #0x0,r5 */
 goto L8c644ba8;
L8c644ba8: target=r[3];*pr=0x8c644bacu;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c644bac;
L8c644bac: r[2]=1u; /* mov #0x1,r2 */
 goto L8c644bae;
L8c644bae: r[7]=r[13]; /* mov r13,r7 */
 goto L8c644bb0;
L8c644bb0: r[3]=0u; /* mov #0x0,r3 */
 goto L8c644bb2;
L8c644bb2: wr(m,r[13],r[2],4); /* mov.l r2,@r13 */
 goto L8c644bb4;
L8c644bb4: r[7]+=48u; /* add #0x30,r7 */
 goto L8c644bb6;
L8c644bb6: r[2]=r[3]; /* mov r3,r2 */
 goto L8c644bb8;
L8c644bb8: r[5]=rd(m,0x8c644ca4u,4); /* mov.l 0x8c644ca4,r5 */
 goto L8c644bba;
L8c644bba: r[4]=sx(m,0x8c644c9au,2); /* mov.w 0x8c644c9a,r4 */
 goto L8c644bbc;
L8c644bbc: wr(m,r[13]+0x4u,r[14],4); /* mov.l r14,@(0x4,r13) */
 goto L8c644bbe;
L8c644bbe: wr(m,r[13]+0x8u,r[3],4); /* mov.l r3,@(0x8,r13) */
 goto L8c644bc0;
L8c644bc0: wr(m,r[13]+0xcu,r[3],4); /* mov.l r3,@(0xc,r13) */
 goto L8c644bc2;
L8c644bc2: r[3]=rd(m,0x8c644ca8u,4); /* mov.l 0x8c644ca8,r3 */
 goto L8c644bc4;
L8c644bc4: wr(m,r[15],r[7],4); /* mov.l r7,@r15 */
 goto L8c644bc6;
L8c644bc6: r[7]+=2u; /* add #0x2,r7 */
 goto L8c644bc8;
L8c644bc8: target=r[3];*pr=0x8c644bccu;r[6]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c644bcc;
L8c644bcc: r[2]=rd(m,0x8c644ca0u,4); /* mov.l 0x8c644ca0,r2 */
 goto L8c644bce;
L8c644bce: r[4]=r[13]; /* mov r13,r4 */
 goto L8c644bd0;
L8c644bd0: r[6]=8u; /* mov #0x8,r6 */
 goto L8c644bd2;
L8c644bd2: r[5]=0u; /* mov #0x0,r5 */
 goto L8c644bd4;
L8c644bd4: target=r[2];*pr=0x8c644bd8u;r[4]+=40u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c644bd8;
L8c644bd8: r[0]=r[13]; /* mov r13,r0 */
 goto L8c644bda;
L8c644bda: r[15]+=4u; /* add #0x4,r15 */
 goto L8c644bdc;
L8c644bdc: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c644bde;
L8c644bde: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c644be0;
L8c644be0: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c644be2;
L8c644be2: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
