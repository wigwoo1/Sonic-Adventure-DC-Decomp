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
/* Static C recovery of retail Tikal helper 8c668b58..8c668bee.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_work_object_create(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c668b58;
L8c668b58: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c668b5a;
L8c668b5a: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c668b5c;
L8c668b5c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c668b5e;
L8c668b5e: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c668b60;
L8c668b60: r[0]=*sr; /* stc SR,r0 */
 goto L8c668b62;
L8c668b62: r[3]=sx(m,0x8c668c2eu,2); /* mov.w 0x8c668c2e,r3 */
 goto L8c668b64;
L8c668b64: r[0]>>=2; /* shlr2 r0 */
 goto L8c668b66;
L8c668b66: r[0]>>=2; /* shlr2 r0 */
 goto L8c668b68;
L8c668b68: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c668b6a;
L8c668b6a: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c668b6c;
L8c668b6c: r[0]=*sr; /* stc SR,r0 */
 goto L8c668b6e;
L8c668b6e: r[0]&=r[3]; /* and r3,r0 */
 goto L8c668b70;
L8c668b70: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c668b72;
L8c668b72: *sr=r[0]; /* ldc r0,SR */
 goto L8c668b74;
L8c668b74: r[13]=0u; /* mov #0x0,r13 */
 goto L8c668b76;
L8c668b76: r[14]=rd(m,0x8c668c44u,4); /* mov.l 0x8c668c44,r14 */
 goto L8c668b78;
L8c668b78: r[6]=8u; /* mov #0x8,r6 */
 goto L8c668b7a;
L8c668b7a: r[5]=r[13]; /* mov r13,r5 */
 goto L8c668b7c;
L8c668b7c: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c668b7e;
L8c668b7e: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c668b80;
L8c668b80: if((*sr&1u))goto L8c668b8c; /* bt 0x8c668b8c */
 goto L8c668b82;
L8c668b82: r[2]=sx(m,0x8c668c30u,2); /* mov.w 0x8c668c30,r2 */
 goto L8c668b84;
L8c668b84: r[5]+=1u; /* add #0x1,r5 */
 goto L8c668b86;
L8c668b86: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]); /* cmp/ge r6,r5 */
 goto L8c668b88;
L8c668b88: take=!(*sr&1u);r[14]+=r[2];if(take)goto L8c668b7c;goto L8c668b8c; /* bf/s 0x8c668b7c */
 goto L8c668b8c;
L8c668b8c: r[0]=r[5]; /* mov r5,r0 */
 goto L8c668b8e;
L8c668b8e: *sr=(*sr&~1u)|(r[0]==0x8u); /* cmp/eq #0x8,r0 */
 goto L8c668b90;
L8c668b90: if(!(*sr&1u))goto L8c668b96; /* bf 0x8c668b96 */
 goto L8c668b92;
L8c668b92: r[14]=r[13];goto L8c668bd0; /* bra 0x8c668bd0 */
 goto L8c668b96;
L8c668b96: r[0]=sx(m,0x8c668c32u,2); /* mov.w 0x8c668c32,r0 */
 goto L8c668b98;
L8c668b98: r[5]=0u; /* mov #0x0,r5 */
 goto L8c668b9a;
L8c668b9a: r[3]=rd(m,0x8c668c48u,4); /* mov.l 0x8c668c48,r3 */
 goto L8c668b9c;
L8c668b9c: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c668b9e;
L8c668b9e: r[0]+=4u; /* add #0x4,r0 */
 goto L8c668ba0;
L8c668ba0: wr(m,r[14]+r[0],r[13],4); /* mov.l r13,@(r0,r14) */
 goto L8c668ba2;
L8c668ba2: r[0]+=4294967288u; /* add #-0x8,r0 */
 goto L8c668ba4;
L8c668ba4: r[4]=sx(m,0x8c668c36u,2); /* mov.w 0x8c668c36,r4 */
 goto L8c668ba6;
L8c668ba6: r[6]=sx(m,0x8c668c34u,2); /* mov.w 0x8c668c34,r6 */
 goto L8c668ba8;
L8c668ba8: wr(m,r[14]+r[0],r[13],4); /* mov.l r13,@(r0,r14) */
 goto L8c668baa;
L8c668baa: target=r[3];*pr=0x8c668baeu;r[4]+=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668bae;
L8c668bae: r[6]=sx(m,0x8c668c38u,2); /* mov.w 0x8c668c38,r6 */
 goto L8c668bb0;
L8c668bb0: r[5]=0u; /* mov #0x0,r5 */
 goto L8c668bb2;
L8c668bb2: r[2]=rd(m,0x8c668c48u,4); /* mov.l 0x8c668c48,r2 */
 goto L8c668bb4;
L8c668bb4: r[4]=r[6]; /* mov r6,r4 */
 goto L8c668bb6;
L8c668bb6: r[4]+=4u; /* add #0x4,r4 */
 goto L8c668bb8;
L8c668bb8: target=r[2];*pr=0x8c668bbcu;r[4]+=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c668bbc;
L8c668bbc: r[3]=rd(m,0x8c668c58u,4); /* mov.l 0x8c668c58,r3 */
 goto L8c668bbe;
L8c668bbe: r[4]=sx(m,0x8c668c3au,2); /* mov.w 0x8c668c3a,r4 */
 goto L8c668bc0;
L8c668bc0: target=r[3];*pr=0x8c668bc4u;r[4]+=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668bc4;
L8c668bc4: r[4]=rd(m,0x8c668c54u,4); /* mov.l 0x8c668c54,r4 */
 goto L8c668bc6;
L8c668bc6: r[2]=1u; /* mov #0x1,r2 */
 goto L8c668bc8;
L8c668bc8: wr(m,r[14],r[2],4); /* mov.l r2,@r14 */
 goto L8c668bca;
L8c668bca: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c668bcc;
L8c668bcc: r[3]+=1u; /* add #0x1,r3 */
 goto L8c668bce;
L8c668bce: wr(m,r[4],r[3],4); /* mov.l r3,@r4 */
 goto L8c668bd0;
L8c668bd0: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c668bd2;
L8c668bd2: r[2]=*sr; /* stc SR,r2 */
 goto L8c668bd4;
L8c668bd4: r[1]=sx(m,0x8c668c2eu,2); /* mov.w 0x8c668c2e,r1 */
 goto L8c668bd6;
L8c668bd6: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c668bd8;
L8c668bd8: r[0]<<=2; /* shll2 r0 */
 goto L8c668bda;
L8c668bda: r[2]&=r[1]; /* and r1,r2 */
 goto L8c668bdc;
L8c668bdc: r[0]<<=2; /* shll2 r0 */
 goto L8c668bde;
L8c668bde: r[0]|=r[2]; /* or r2,r0 */
 goto L8c668be0;
L8c668be0: *sr=r[0]; /* ldc r0,SR */
 goto L8c668be2;
L8c668be2: r[0]=r[14]; /* mov r14,r0 */
 goto L8c668be4;
L8c668be4: r[15]+=4u; /* add #0x4,r15 */
 goto L8c668be6;
L8c668be6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c668be8;
L8c668be8: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c668bea;
L8c668bea: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
