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
/* Static C recovery of retail Tikal helper 8c666ef0..8c666f72.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_frame_queue_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c666ef0;
L8c666ef0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c666ef2;
L8c666ef2: r[7]=0u; /* mov #0x0,r7 */
 goto L8c666ef4;
L8c666ef4: r[6]=rd(m,0x8c666f74u,4); /* mov.l 0x8c666f74,r6 */
 goto L8c666ef6;
L8c666ef6: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c666ef8;
L8c666ef8: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c666efa;
L8c666efa: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c666efc;
L8c666efc: r[14]=rd(m,r[6],4); /* mov.l @r6,r14 */
 goto L8c666efe;
L8c666efe: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c666f00;
L8c666f00: take=!(*sr&1u);r[13]=r[7];if(take)goto L8c666f0a;goto L8c666f04; /* bf/s 0x8c666f0a */
 goto L8c666f04;
L8c666f04: r[13]=rd(m,0x8c666f78u,4); /* mov.l 0x8c666f78,r13 */
 goto L8c666f06;
L8c666f06: goto L8c666f5a; /* bra 0x8c666f5a */
 goto L8c666f0a;
L8c666f0a: r[0]=sx(m,0x8c666f72u,2); /* mov.w 0x8c666f72,r0 */
 goto L8c666f0c;
L8c666f0c: *sr=(*sr&~1u)|((int32_t)r[5]>0); /* cmp/pl r5 */
 goto L8c666f0e;
L8c666f0e: r[2]=sx(m,0x8c666f72u,2); /* mov.w 0x8c666f72,r2 */
 goto L8c666f10;
L8c666f10: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c666f12;
L8c666f12: r[0]+=4u; /* add #0x4,r0 */
 goto L8c666f14;
L8c666f14: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c666f16;
L8c666f16: r[14]=r[7]; /* mov r7,r14 */
 goto L8c666f18;
L8c666f18: r[1]=rd(m,0x8c666f80u,4); /* mov.l 0x8c666f80,r1 */
 goto L8c666f1a;
L8c666f1a: wr(m,r[3]+r[0],r[5],4); /* mov.l r5,@(r0,r3) */
 goto L8c666f1c;
L8c666f1c: r[0]+=4u; /* add #0x4,r0 */
 goto L8c666f1e;
L8c666f1e: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c666f20;
L8c666f20: r[12]=rd(m,0x8c666f88u,4); /* mov.l 0x8c666f88,r12 */
 goto L8c666f22;
L8c666f22: wr(m,r[3]+r[0],r[7],4); /* mov.l r7,@(r0,r3) */
 goto L8c666f24;
L8c666f24: r[0]+=4u; /* add #0x4,r0 */
 goto L8c666f26;
L8c666f26: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c666f28;
L8c666f28: wr(m,r[3]+r[0],r[7],4); /* mov.l r7,@(r0,r3) */
 goto L8c666f2a;
L8c666f2a: r[0]+=4u; /* add #0x4,r0 */
 goto L8c666f2c;
L8c666f2c: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c666f2e;
L8c666f2e: wr(m,r[3]+r[0],r[7],4); /* mov.l r7,@(r0,r3) */
 goto L8c666f30;
L8c666f30: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c666f32;
L8c666f32: r[2]+=r[3]; /* add r3,r2 */
 goto L8c666f34;
L8c666f34: r[3]=rd(m,0x8c666f7cu,4); /* mov.l 0x8c666f7c,r3 */
 goto L8c666f36;
L8c666f36: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c666f38;
L8c666f38: r[3]=rd(m,0x8c666f84u,4); /* mov.l 0x8c666f84,r3 */
 goto L8c666f3a;
L8c666f3a: wr(m,r[1],r[4],4); /* mov.l r4,@r1 */
 goto L8c666f3c;
L8c666f3c: wr(m,r[3],r[5],4); /* mov.l r5,@r3 */
 goto L8c666f3e;
L8c666f3e: take=!(*sr&1u);r[4]=r[7];if(take)goto L8c666f5a;goto L8c666f42; /* bf/s 0x8c666f5a */
 goto L8c666f42;
L8c666f42: r[0]=sx(m,0x8c666f72u,2); /* mov.w 0x8c666f72,r0 */
 goto L8c666f44;
L8c666f44: r[2]=r[12]; /* mov r12,r2 */
 goto L8c666f46;
L8c666f46: r[1]=rd(m,r[6],4); /* mov.l @r6,r1 */
 goto L8c666f48;
L8c666f48: r[3]=rd(m,0x8c666f8cu,4); /* mov.l 0x8c666f8c,r3 */
 goto L8c666f4a;
L8c666f4a: r[1]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r1 */
 goto L8c666f4c;
L8c666f4c: r[1]+=r[4]; /* add r4,r1 */
 goto L8c666f4e;
L8c666f4e: target=r[3];*pr=0x8c666f52u;r[0]=8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666f52;
L8c666f52: r[14]+=1u; /* add #0x1,r14 */
 goto L8c666f54;
L8c666f54: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[5]); /* cmp/ge r5,r14 */
 goto L8c666f56;
L8c666f56: take=!(*sr&1u);r[4]+=8u;if(take)goto L8c666f42;goto L8c666f5a; /* bf/s 0x8c666f42 */
 goto L8c666f5a;
L8c666f5a: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c666f5c;
L8c666f5c: if((*sr&1u))goto L8c666f66; /* bt 0x8c666f66 */
 goto L8c666f5e;
L8c666f5e: r[2]=rd(m,0x8c666f94u,4); /* mov.l 0x8c666f94,r2 */
 goto L8c666f60;
L8c666f60: r[4]=rd(m,0x8c666f90u,4); /* mov.l 0x8c666f90,r4 */
 goto L8c666f62;
L8c666f62: target=r[2];*pr=0x8c666f66u;r[5]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c666f66;
L8c666f66: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c666f68;
L8c666f68: r[0]=r[13]; /* mov r13,r0 */
 goto L8c666f6a;
L8c666f6a: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c666f6c;
L8c666f6c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c666f6e;
L8c666f6e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
