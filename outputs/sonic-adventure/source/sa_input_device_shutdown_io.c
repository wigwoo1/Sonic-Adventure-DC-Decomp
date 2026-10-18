#include "input_io.h"
#include "fpu_cleanup_transfer.h"
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(const struct sa_input_io *io,uint8_t *m,uint32_t a,unsigned n){uint32_t physical=a&0x1fffffffu;if(physical<0x0c000000u||physical>=0x0d000000u)return io->read(io->user,a,n);uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(const struct sa_input_io *io,uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(io,m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(const struct sa_input_io *io,uint8_t *m,uint32_t a,uint32_t v,unsigned n){uint32_t physical=a&0x1fffffffu;if(physical<0x0c000000u||physical>=0x0d000000u){io->write(io->user,a,v,n);return;}for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|((a==b)||(!(a&0x7fffffffu)&&!(b&0x7fffffffu)));return 0;
}
/* Static C recovery of retail Tikal helper 8c65f2a4..8c65f348.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_shutdown_io(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,const struct sa_input_io *io,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f2a4;
L8c65f2a4: r[15]-=4;wr(io,m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65f2a6;
L8c65f2a6: r[15]-=4;wr(io,m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65f2a8;
L8c65f2a8: r[15]-=4;wr(io,m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65f2aa;
L8c65f2aa: r[15]-=4;wr(io,m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65f2ac;
L8c65f2ac: r[15]-=4;wr(io,m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65f2ae;
L8c65f2ae: r[15]-=4;wr(io,m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c65f2b0;
L8c65f2b0: r[15]-=4;wr(io,m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65f2b2;
L8c65f2b2: r[13]=rd(io,m,0x8c65f3ccu,4); /* mov.l 0x8c65f3cc,r13 */
 goto L8c65f2b4;
L8c65f2b4: target=r[13];*pr=0x8c65f2b8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c65f2b8;
L8c65f2b8: r[11]=rd(io,m,0x8c65f3d4u,4); /* mov.l 0x8c65f3d4,r11 */
 goto L8c65f2ba;
L8c65f2ba: r[9]=r[0]; /* mov r0,r9 */
 goto L8c65f2bc;
L8c65f2bc: r[14]=rd(io,m,0x8c65f3d8u,4); /* mov.l 0x8c65f3d8,r14 */
 goto L8c65f2be;
L8c65f2be: r[10]=rd(io,m,0x8c65f3d0u,4); /* mov.l 0x8c65f3d0,r10 */
 goto L8c65f2c0;
L8c65f2c0: r[12]=1u;goto L8c65f2d2; /* bra 0x8c65f2d2 */
 goto L8c65f2c4;
L8c65f2c4: target=r[13];*pr=0x8c65f2c8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c65f2c8;
L8c65f2c8: r[5]=r[0]; /* mov r0,r5 */
 goto L8c65f2ca;
L8c65f2ca: target=r[11];*pr=0x8c65f2ceu;r[4]=r[9];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c65f2ce;
L8c65f2ce: *sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[10]); /* cmp/gt r10,r0 */
 goto L8c65f2d0;
L8c65f2d0: if((*sr&1u))goto L8c65f2de; /* bt 0x8c65f2de */
 goto L8c65f2d2;
L8c65f2d2: r[0]=sx(io,m,0x8c65f3c0u,2); /* mov.w 0x8c65f3c0,r0 */
 goto L8c65f2d4;
L8c65f2d4: r[2]=rd(io,m,r[14],4); /* mov.l @r14,r2 */
 goto L8c65f2d6;
L8c65f2d6: r[3]=rd(io,m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto L8c65f2d8;
L8c65f2d8: r[1]=rd(io,m,r[3]+0x18u,4); /* mov.l @(0x18,r3),r1 */
 goto L8c65f2da;
L8c65f2da: *sr=(*sr&~1u)|!(r[12]&r[1]); /* tst r12,r1 */
 goto L8c65f2dc;
L8c65f2dc: if(!(*sr&1u))goto L8c65f2c4; /* bf 0x8c65f2c4 */
 goto L8c65f2de;
L8c65f2de: r[0]=sx(io,m,0x8c65f3c0u,2); /* mov.w 0x8c65f3c0,r0 */
 goto L8c65f2e0;
L8c65f2e0: r[4]=0u; /* mov #0x0,r4 */
 goto L8c65f2e2;
L8c65f2e2: r[2]=rd(io,m,r[14],4); /* mov.l @r14,r2 */
 goto L8c65f2e4;
L8c65f2e4: r[3]=rd(io,m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto L8c65f2e6;
L8c65f2e6: wr(io,m,r[3]+0x18u,r[4],4); /* mov.l r4,@(0x18,r3) */
 goto L8c65f2e8;
L8c65f2e8: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f2ea;
L8c65f2ea: r[2]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65f2ec;
L8c65f2ec: wr(io,m,r[2]+0x14u,r[4],4); /* mov.l r4,@(0x14,r2) */
 goto L8c65f2ee;
L8c65f2ee: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f2f0;
L8c65f2f0: r[2]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65f2f2;
L8c65f2f2: wr(io,m,r[2]+0x10u,r[4],4); /* mov.l r4,@(0x10,r2) */
 goto L8c65f2f4;
L8c65f2f4: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f2f6;
L8c65f2f6: r[1]=rd(io,m,0x8c65f3dcu,4); /* mov.l 0x8c65f3dc,r1 */
 goto L8c65f2f8;
L8c65f2f8: r[2]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65f2fa;
L8c65f2fa: r[0]=sx(io,m,0x8c65f3c2u,2); /* mov.w 0x8c65f3c2,r0 */
 goto L8c65f2fc;
L8c65f2fc: wr(io,m,r[2]+r[0],r[1],4); /* mov.l r1,@(r0,r2) */
 goto L8c65f2fe;
L8c65f2fe: r[0]=sx(io,m,0x8c65f3c0u,2); /* mov.w 0x8c65f3c0,r0 */
 goto L8c65f300;
L8c65f300: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f302;
L8c65f302: r[1]=rd(io,m,0x8c65f3e0u,4); /* mov.l 0x8c65f3e0,r1 */
 goto L8c65f304;
L8c65f304: r[2]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65f306;
L8c65f306: r[0]=sx(io,m,0x8c65f3c4u,2); /* mov.w 0x8c65f3c4,r0 */
 goto L8c65f308;
L8c65f308: wr(io,m,r[2]+r[0],r[1],4); /* mov.l r1,@(r0,r2) */
 goto L8c65f30a;
L8c65f30a: r[0]=sx(io,m,0x8c65f3c0u,2); /* mov.w 0x8c65f3c0,r0 */
 goto L8c65f30c;
L8c65f30c: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f30e;
L8c65f30e: r[1]=rd(io,m,0x8c65f3e4u,4); /* mov.l 0x8c65f3e4,r1 */
 goto L8c65f310;
L8c65f310: r[2]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65f312;
L8c65f312: target=r[13];*pr=0x8c65f316u;wr(io,m,r[2]+0x4u,r[1],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c65f316;
L8c65f316: r[10]=sx(io,m,0x8c65f3c6u,2); /* mov.w 0x8c65f3c6,r10 */
 goto L8c65f318;
L8c65f318: r[9]=r[0];goto L8c65f32a; /* bra 0x8c65f32a */
 goto L8c65f31c;
L8c65f31c: target=r[13];*pr=0x8c65f320u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c65f320;
L8c65f320: r[5]=r[0]; /* mov r0,r5 */
 goto L8c65f322;
L8c65f322: target=r[11];*pr=0x8c65f326u;r[4]=r[9];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c65f326;
L8c65f326: *sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[10]); /* cmp/gt r10,r0 */
 goto L8c65f328;
L8c65f328: if((*sr&1u))goto L8c65f336; /* bt 0x8c65f336 */
 goto L8c65f32a;
L8c65f32a: r[0]=sx(io,m,0x8c65f3c0u,2); /* mov.w 0x8c65f3c0,r0 */
 goto L8c65f32c;
L8c65f32c: r[2]=rd(io,m,r[14],4); /* mov.l @r14,r2 */
 goto L8c65f32e;
L8c65f32e: r[3]=rd(io,m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto L8c65f330;
L8c65f330: r[1]=rd(io,m,r[3]+0x18u,4); /* mov.l @(0x18,r3),r1 */
 goto L8c65f332;
L8c65f332: *sr=(*sr&~1u)|!(r[12]&r[1]); /* tst r12,r1 */
 goto L8c65f334;
L8c65f334: if(!(*sr&1u))goto L8c65f31c; /* bf 0x8c65f31c */
 goto L8c65f336;
L8c65f336: *pr=rd(io,m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65f338;
L8c65f338: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65f33a;
L8c65f33a: r[9]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c65f33c;
L8c65f33c: r[10]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65f33e;
L8c65f33e: r[11]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65f340;
L8c65f340: r[12]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65f342;
L8c65f342: r[13]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65f344;
L8c65f344: r[14]=rd(io,m,r[15],4);r[15]+=4;return 0; /* rts */
}
