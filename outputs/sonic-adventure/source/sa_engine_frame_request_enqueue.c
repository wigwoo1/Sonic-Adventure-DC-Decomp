/* Sonic Adventure USA Rev A frame request enqueue, 0x8c666ba8. */
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
/* Static C recovery of retail Tikal helper 8c666ba8..8c666c14.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_frame_request_enqueue(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c666ba8;
L8c666ba8: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c666baa;
L8c666baa: r[2]=32u; /* mov #0x20,r2 */
 goto L8c666bac;
L8c666bac: r[3]=rd(m,0x8c666c18u,4); /* mov.l 0x8c666c18,r3 */
 goto L8c666bae;
L8c666bae: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c666bb0;
L8c666bb0: r[13]=r[4]; /* mov r4,r13 */
 goto L8c666bb2;
L8c666bb2: r[0]=sx(m,0x8c666c14u,2); /* mov.w 0x8c666c14,r0 */
 goto L8c666bb4;
L8c666bb4: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c666bb6;
L8c666bb6: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c666bb8;
L8c666bb8: r[4]=rd(m,r[3],4); /* mov.l @r3,r4 */
 goto L8c666bba;
L8c666bba: r[1]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r1 */
 goto L8c666bbc;
L8c666bbc: *sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[2]); /* cmp/ge r2,r1 */
 goto L8c666bbe;
L8c666bbe: take=!(*sr&1u);r[14]=0u;if(take)goto L8c666bc8;goto L8c666bc2; /* bf/s 0x8c666bc8 */
 goto L8c666bc2;
L8c666bc2: r[14]=rd(m,0x8c666c1cu,4); /* mov.l 0x8c666c1c,r14 */
 goto L8c666bc4;
L8c666bc4: goto L8c666bfc; /* bra 0x8c666bfc */
 goto L8c666bc8;
L8c666bc8: r[12]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r12 */
 goto L8c666bca;
L8c666bca: r[5]=r[13]; /* mov r13,r5 */
 goto L8c666bcc;
L8c666bcc: r[5]+=4u; /* add #0x4,r5 */
 goto L8c666bce;
L8c666bce: r[12]+=1u; /* add #0x1,r12 */
 goto L8c666bd0;
L8c666bd0: wr(m,r[4]+r[0],r[12],4); /* mov.l r12,@(r0,r4) */
 goto L8c666bd2;
L8c666bd2: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c666bd4;
L8c666bd4: r[12]+=4294967295u; /* add #-0x1,r12 */
 goto L8c666bd6;
L8c666bd6: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c666bd8;
L8c666bd8: r[12]<<=2; /* shll2 r12 */
 goto L8c666bda;
L8c666bda: r[2]=rd(m,r[13],4); /* mov.l @r13,r2 */
 goto L8c666bdc;
L8c666bdc: r[12]<<=2; /* shll2 r12 */
 goto L8c666bde;
L8c666bde: r[12]+=r[3]; /* add r3,r12 */
 goto L8c666be0;
L8c666be0: r[3]=rd(m,0x8c666c20u,4); /* mov.l 0x8c666c20,r3 */
 goto L8c666be2;
L8c666be2: r[4]=r[12]; /* mov r12,r4 */
 goto L8c666be4;
L8c666be4: wr(m,r[12],r[2],1); /* mov.b r2,@r12 */
 goto L8c666be6;
L8c666be6: r[6]=14u; /* mov #0xe,r6 */
 goto L8c666be8;
L8c666be8: target=r[3];*pr=0x8c666becu;r[4]+=2u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666bec;
L8c666bec: r[1]=rd(m,0x8c666c18u,4); /* mov.l 0x8c666c18,r1 */
 goto L8c666bee;
L8c666bee: r[2]=0u; /* mov #0x0,r2 */
 goto L8c666bf0;
L8c666bf0: r[0]=sx(m,0x8c666c16u,2); /* mov.w 0x8c666c16,r0 */
 goto L8c666bf2;
L8c666bf2: wr(m,r[13]+0x14u,r[2],4); /* mov.l r2,@(0x14,r13) */
 goto L8c666bf4;
L8c666bf4: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c666bf6;
L8c666bf6: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c666bf8;
L8c666bf8: r[2]+=4294967295u; /* add #-0x1,r2 */
 goto L8c666bfa;
L8c666bfa: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c666bfc;
L8c666bfc: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c666bfe;
L8c666bfe: if((*sr&1u))goto L8c666c08; /* bt 0x8c666c08 */
 goto L8c666c00;
L8c666c00: r[2]=rd(m,0x8c666c28u,4); /* mov.l 0x8c666c28,r2 */
 goto L8c666c02;
L8c666c02: r[4]=rd(m,0x8c666c24u,4); /* mov.l 0x8c666c24,r4 */
 goto L8c666c04;
L8c666c04: target=r[2];*pr=0x8c666c08u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c666c08;
L8c666c08: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c666c0a;
L8c666c0a: r[0]=r[14]; /* mov r14,r0 */
 goto L8c666c0c;
L8c666c0c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c666c0e;
L8c666c0e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c666c10;
L8c666c10: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
