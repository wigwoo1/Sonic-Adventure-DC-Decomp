/* Sonic Adventure USA Rev A recovered frame setup/copy routine. */
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
/* Static C recovery of retail Tikal helper 8c666e50..8c666ed0.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_frame_object_pool_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c666e50;
L8c666e50: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c666e52;
L8c666e52: r[7]=0u; /* mov #0x0,r7 */
 goto L8c666e54;
L8c666e54: r[6]=rd(m,0x8c666ed4u,4); /* mov.l 0x8c666ed4,r6 */
 goto L8c666e56;
L8c666e56: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c666e58;
L8c666e58: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c666e5a;
L8c666e5a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c666e5c;
L8c666e5c: r[13]=rd(m,r[6],4); /* mov.l @r6,r13 */
 goto L8c666e5e;
L8c666e5e: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c666e60;
L8c666e60: take=!(*sr&1u);r[14]=r[7];if(take)goto L8c666e6a;goto L8c666e64; /* bf/s 0x8c666e6a */
 goto L8c666e64;
L8c666e64: r[14]=rd(m,0x8c666ed8u,4); /* mov.l 0x8c666ed8,r14 */
 goto L8c666e66;
L8c666e66: goto L8c666eb8; /* bra 0x8c666eb8 */
 goto L8c666e6a;
L8c666e6a: r[0]=sx(m,0x8c666ed0u,2); /* mov.w 0x8c666ed0,r0 */
 goto L8c666e6c;
L8c666e6c: *sr=(*sr&~1u)|((int32_t)r[5]>0); /* cmp/pl r5 */
 goto L8c666e6e;
L8c666e6e: r[2]=sx(m,0x8c666ed0u,2); /* mov.w 0x8c666ed0,r2 */
 goto L8c666e70;
L8c666e70: wr(m,r[13]+r[0],r[4],4); /* mov.l r4,@(r0,r13) */
 goto L8c666e72;
L8c666e72: r[0]+=4u; /* add #0x4,r0 */
 goto L8c666e74;
L8c666e74: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c666e76;
L8c666e76: r[13]=r[7]; /* mov r7,r13 */
 goto L8c666e78;
L8c666e78: r[12]=rd(m,0x8c666ee0u,4); /* mov.l 0x8c666ee0,r12 */
 goto L8c666e7a;
L8c666e7a: wr(m,r[3]+r[0],r[5],4); /* mov.l r5,@(r0,r3) */
 goto L8c666e7c;
L8c666e7c: r[0]+=4u; /* add #0x4,r0 */
 goto L8c666e7e;
L8c666e7e: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c666e80;
L8c666e80: wr(m,r[3]+r[0],r[7],4); /* mov.l r7,@(r0,r3) */
 goto L8c666e82;
L8c666e82: r[0]+=4u; /* add #0x4,r0 */
 goto L8c666e84;
L8c666e84: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c666e86;
L8c666e86: wr(m,r[3]+r[0],r[7],4); /* mov.l r7,@(r0,r3) */
 goto L8c666e88;
L8c666e88: r[0]+=4u; /* add #0x4,r0 */
 goto L8c666e8a;
L8c666e8a: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c666e8c;
L8c666e8c: wr(m,r[3]+r[0],r[7],4); /* mov.l r7,@(r0,r3) */
 goto L8c666e8e;
L8c666e8e: r[0]+=4u; /* add #0x4,r0 */
 goto L8c666e90;
L8c666e90: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c666e92;
L8c666e92: wr(m,r[3]+r[0],r[7],4); /* mov.l r7,@(r0,r3) */
 goto L8c666e94;
L8c666e94: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c666e96;
L8c666e96: r[2]+=r[3]; /* add r3,r2 */
 goto L8c666e98;
L8c666e98: r[3]=rd(m,0x8c666edcu,4); /* mov.l 0x8c666edc,r3 */
 goto L8c666e9a;
L8c666e9a: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c666e9c;
L8c666e9c: take=!(*sr&1u);r[4]=r[7];if(take)goto L8c666eb8;goto L8c666ea0; /* bf/s 0x8c666eb8 */
 goto L8c666ea0;
L8c666ea0: r[0]=sx(m,0x8c666ed0u,2); /* mov.w 0x8c666ed0,r0 */
 goto L8c666ea2;
L8c666ea2: r[2]=r[12]; /* mov r12,r2 */
 goto L8c666ea4;
L8c666ea4: r[1]=rd(m,r[6],4); /* mov.l @r6,r1 */
 goto L8c666ea6;
L8c666ea6: r[3]=rd(m,0x8c666ee4u,4); /* mov.l 0x8c666ee4,r3 */
 goto L8c666ea8;
L8c666ea8: r[1]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r1 */
 goto L8c666eaa;
L8c666eaa: r[1]+=r[4]; /* add r4,r1 */
 goto L8c666eac;
L8c666eac: target=r[3];*pr=0x8c666eb0u;r[0]=32u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666eb0;
L8c666eb0: r[13]+=1u; /* add #0x1,r13 */
 goto L8c666eb2;
L8c666eb2: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[5]); /* cmp/ge r5,r13 */
 goto L8c666eb4;
L8c666eb4: take=!(*sr&1u);r[4]+=32u;if(take)goto L8c666ea0;goto L8c666eb8; /* bf/s 0x8c666ea0 */
 goto L8c666eb8;
L8c666eb8: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c666eba;
L8c666eba: if((*sr&1u))goto L8c666ec4; /* bt 0x8c666ec4 */
 goto L8c666ebc;
L8c666ebc: r[2]=rd(m,0x8c666eecu,4); /* mov.l 0x8c666eec,r2 */
 goto L8c666ebe;
L8c666ebe: r[4]=rd(m,0x8c666ee8u,4); /* mov.l 0x8c666ee8,r4 */
 goto L8c666ec0;
L8c666ec0: target=r[2];*pr=0x8c666ec4u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c666ec4;
L8c666ec4: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c666ec6;
L8c666ec6: r[0]=r[14]; /* mov r14,r0 */
 goto L8c666ec8;
L8c666ec8: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c666eca;
L8c666eca: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c666ecc;
L8c666ecc: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
