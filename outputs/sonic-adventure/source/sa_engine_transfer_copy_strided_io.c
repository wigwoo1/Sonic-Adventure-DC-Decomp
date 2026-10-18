#include "input_io.h"
/* Sonic Adventure USA Rev A main executable recovered transfer routine. */
#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
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
/* Static C recovery of retail Tikal helper 8c10d770..8c10d908.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_transfer_copy_strided_io(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,const struct sa_input_io *io,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d770;
L8c10d770: r[15]-=4;wr(io,m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c10d772;
L8c10d772: r[15]-=4;wr(io,m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c10d774;
L8c10d774: r[15]-=4;wr(io,m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c10d776;
L8c10d776: r[15]-=4;wr(io,m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c10d778;
L8c10d778: r[15]-=4;wr(io,m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c10d77a;
L8c10d77a: r[15]-=4;wr(io,m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c10d77c;
L8c10d77c: r[15]-=4;wr(io,m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c10d77e;
L8c10d77e: r[15]-=4;wr(io,m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10d780;
L8c10d780: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto L8c10d782;
L8c10d782: wr(io,m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c10d784;
L8c10d784: r[5]=1u; /* mov #0x1,r5 */
 goto L8c10d786;
L8c10d786: r[8]=r[4]; /* mov r4,r8 */
 goto L8c10d788;
L8c10d788: wr(io,m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c10d78a;
L8c10d78a: r[0]=rd(io,m,r[15]+0x30u,4); /* mov.l @(0x30,r15),r0 */
 goto L8c10d78c;
L8c10d78c: r[13]=r[7]; /* mov r7,r13 */
 goto L8c10d78e;
L8c10d78e: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c10d790;
L8c10d790: take=(*sr&1u);r[4]=4294967295u;if(take)goto L8c10d7a0;goto L8c10d794; /* bt/s 0x8c10d7a0 */
 goto L8c10d794;
L8c10d794: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10d796;
L8c10d796: if((*sr&1u))goto L8c10d7a4; /* bt 0x8c10d7a4 */
 goto L8c10d798;
L8c10d798: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c10d79a;
L8c10d79a: if((*sr&1u))goto L8c10d7a8; /* bt 0x8c10d7a8 */
 goto L8c10d79c;
L8c10d79c: goto L8c10d7aa; /* bra 0x8c10d7aa */
 goto L8c10d7a0;
L8c10d7a0: r[10]=0u;goto L8c10d7aa; /* bra 0x8c10d7aa */
 goto L8c10d7a4;
L8c10d7a4: r[10]=r[5];goto L8c10d7aa; /* bra 0x8c10d7aa */
 goto L8c10d7a8;
L8c10d7a8: r[10]=r[4]; /* mov r4,r10 */
 goto L8c10d7aa;
L8c10d7aa: r[0]=rd(io,m,r[15]+0x34u,4); /* mov.l @(0x34,r15),r0 */
 goto L8c10d7ac;
L8c10d7ac: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c10d7ae;
L8c10d7ae: if((*sr&1u))goto L8c10d7bc; /* bt 0x8c10d7bc */
 goto L8c10d7b0;
L8c10d7b0: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10d7b2;
L8c10d7b2: if((*sr&1u))goto L8c10d7c0; /* bt 0x8c10d7c0 */
 goto L8c10d7b4;
L8c10d7b4: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c10d7b6;
L8c10d7b6: if((*sr&1u))goto L8c10d7c4; /* bt 0x8c10d7c4 */
 goto L8c10d7b8;
L8c10d7b8: goto L8c10d7c6; /* bra 0x8c10d7c6 */
 goto L8c10d7bc;
L8c10d7bc: r[9]=0u;goto L8c10d7c6; /* bra 0x8c10d7c6 */
 goto L8c10d7c0;
L8c10d7c0: r[9]=r[5];goto L8c10d7c6; /* bra 0x8c10d7c6 */
 goto L8c10d7c4;
L8c10d7c4: r[9]=r[4]; /* mov r4,r9 */
 goto L8c10d7c6;
L8c10d7c6: r[3]=rd(io,m,0x8c10d844u,4); /* mov.l 0x8c10d844,r3 */
 goto L8c10d7c8;
L8c10d7c8: r[1]=sx(io,m,0x8c10d840u,2); /* mov.w 0x8c10d840,r1 */
 goto L8c10d7ca;
L8c10d7ca: r[0]=rd(io,m,r[3],4); /* mov.l @r3,r0 */
 goto L8c10d7cc;
L8c10d7cc: r[0]=rd(io,m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto L8c10d7ce;
L8c10d7ce: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10d7d0;
L8c10d7d0: if(!(*sr&1u))goto L8c10d7e6; /* bf 0x8c10d7e6 */
 goto L8c10d7d2;
L8c10d7d2: r[0]=*sr; /* stc SR,r0 */
 goto L8c10d7d4;
L8c10d7d4: r[3]=sx(io,m,0x8c10d842u,2); /* mov.w 0x8c10d842,r3 */
 goto L8c10d7d6;
L8c10d7d6: r[0]>>=2; /* shlr2 r0 */
 goto L8c10d7d8;
L8c10d7d8: r[0]>>=2; /* shlr2 r0 */
 goto L8c10d7da;
L8c10d7da: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c10d7dc;
L8c10d7dc: wr(io,m,r[15]+0xcu,r[0],4); /* mov.l r0,@(0xc,r15) */
 goto L8c10d7de;
L8c10d7de: r[0]=*sr; /* stc SR,r0 */
 goto L8c10d7e0;
L8c10d7e0: r[0]&=r[3]; /* and r3,r0 */
 goto L8c10d7e2;
L8c10d7e2: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c10d7e4;
L8c10d7e4: *sr=r[0]; /* ldc r0,SR */
 goto L8c10d7e6;
L8c10d7e6: r[2]=rd(io,m,0x8c10d848u,4); /* mov.l 0x8c10d848,r2 */
 goto L8c10d7e8;
L8c10d7e8: target=r[2];*pr=0x8c10d7ecu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c10d7ec;
L8c10d7ec: r[12]=rd(io,m,0x8c10d84cu,4); /* mov.l 0x8c10d84c,r12 */
 goto L8c10d7ee;
L8c10d7ee: target=r[12];*pr=0x8c10d7f2u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c10d7f2;
L8c10d7f2: r[0]=rd(io,m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r0 */
 goto L8c10d7f4;
L8c10d7f4: r[11]=rd(io,m,0x8c10d850u,4); /* mov.l 0x8c10d850,r11 */
 goto L8c10d7f6;
L8c10d7f6: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10d7f8;
L8c10d7f8: take=(*sr&1u);r[14]=7u;if(take)goto L8c10d808;goto L8c10d7fc; /* bt/s 0x8c10d808 */
 goto L8c10d7fc;
L8c10d7fc: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c10d7fe;
L8c10d7fe: if((*sr&1u))goto L8c10d854; /* bt 0x8c10d854 */
 goto L8c10d800;
L8c10d800: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c10d802;
L8c10d802: if((*sr&1u))goto L8c10d890; /* bt 0x8c10d890 */
 goto L8c10d804;
L8c10d804: goto L8c10d8ce; /* bra 0x8c10d8ce */
 goto L8c10d808;
L8c10d808: r[3]=rd(io,m,r[15],4); /* mov.l @r15,r3 */
 goto L8c10d80a;
L8c10d80a: *sr=(*sr&~1u)|((int32_t)r[13]>0); /* cmp/pl r13 */
 goto L8c10d80c;
L8c10d80c: wr(io,m,r[15]+0x4u,r[3],4); /* mov.l r3,@(0x4,r15) */
 goto L8c10d80e;
L8c10d80e: wr(io,m,r[15],r[8],4); /* mov.l r8,@r15 */
 goto L8c10d810;
L8c10d810: take=!(*sr&1u);r[8]=0u;if(take)goto L8c10d8ce;goto L8c10d814; /* bf/s 0x8c10d8ce */
 goto L8c10d814;
L8c10d814: r[2]=r[8]; /* mov r8,r2 */
 goto L8c10d816;
L8c10d816: *sr=(*sr&~1u)|!(r[14]&r[2]); /* tst r14,r2 */
 goto L8c10d818;
L8c10d818: if(!(*sr&1u))goto L8c10d822; /* bf 0x8c10d822 */
 goto L8c10d81a;
L8c10d81a: target=r[12];*pr=0x8c10d81eu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c10d81e;
L8c10d81e: target=r[11];*pr=0x8c10d822u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c10d822;
L8c10d822: r[3]=rd(io,m,r[15],4); /* mov.l @r15,r3 */
 goto L8c10d824;
L8c10d824: r[8]+=1u; /* add #0x1,r8 */
 goto L8c10d826;
L8c10d826: r[1]=rd(io,m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r1 */
 goto L8c10d828;
L8c10d828: *sr=(*sr&~1u)|((int32_t)r[8]>=(int32_t)r[13]); /* cmp/ge r13,r8 */
 goto L8c10d82a;
L8c10d82a: r[2]=sx(io,m,r[3],1); /* mov.b @r3,r2 */
 goto L8c10d82c;
L8c10d82c: wr(io,m,r[1],r[2],1); /* mov.b r2,@r1 */
 goto L8c10d82e;
L8c10d82e: r[3]=rd(io,m,r[15],4); /* mov.l @r15,r3 */
 goto L8c10d830;
L8c10d830: r[3]+=r[10]; /* add r10,r3 */
 goto L8c10d832;
L8c10d832: wr(io,m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c10d834;
L8c10d834: r[2]=rd(io,m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c10d836;
L8c10d836: r[2]+=r[9]; /* add r9,r2 */
 goto L8c10d838;
L8c10d838: take=!(*sr&1u);wr(io,m,r[15]+0x4u,r[2],4);if(take)goto L8c10d814;goto L8c10d83c; /* bf/s 0x8c10d814 */
 goto L8c10d83c;
L8c10d83c: goto L8c10d8ce; /* bra 0x8c10d8ce */
 goto L8c10d854;
L8c10d854: r[2]=rd(io,m,r[15],4); /* mov.l @r15,r2 */
 goto L8c10d856;
L8c10d856: *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1; /* shll r10 */
 goto L8c10d858;
L8c10d858: *sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1; /* shll r9 */
 goto L8c10d85a;
L8c10d85a: *sr=(*sr&~1u)|((int32_t)r[13]>0); /* cmp/pl r13 */
 goto L8c10d85c;
L8c10d85c: wr(io,m,r[15]+0x4u,r[2],4); /* mov.l r2,@(0x4,r15) */
 goto L8c10d85e;
L8c10d85e: wr(io,m,r[15],r[8],4); /* mov.l r8,@r15 */
 goto L8c10d860;
L8c10d860: take=!(*sr&1u);r[8]=0u;if(take)goto L8c10d8ce;goto L8c10d864; /* bf/s 0x8c10d8ce */
 goto L8c10d864;
L8c10d864: r[2]=r[8]; /* mov r8,r2 */
 goto L8c10d866;
L8c10d866: *sr=(*sr&~1u)|!(r[14]&r[2]); /* tst r14,r2 */
 goto L8c10d868;
L8c10d868: if(!(*sr&1u))goto L8c10d872; /* bf 0x8c10d872 */
 goto L8c10d86a;
L8c10d86a: target=r[12];*pr=0x8c10d86eu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c10d86e;
L8c10d86e: target=r[11];*pr=0x8c10d872u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c10d872;
L8c10d872: r[3]=rd(io,m,r[15],4); /* mov.l @r15,r3 */
 goto L8c10d874;
L8c10d874: r[8]+=1u; /* add #0x1,r8 */
 goto L8c10d876;
L8c10d876: r[1]=rd(io,m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r1 */
 goto L8c10d878;
L8c10d878: *sr=(*sr&~1u)|((int32_t)r[8]>=(int32_t)r[13]); /* cmp/ge r13,r8 */
 goto L8c10d87a;
L8c10d87a: r[2]=sx(io,m,r[3],2); /* mov.w @r3,r2 */
 goto L8c10d87c;
L8c10d87c: wr(io,m,r[1],r[2],2); /* mov.w r2,@r1 */
 goto L8c10d87e;
L8c10d87e: r[3]=rd(io,m,r[15],4); /* mov.l @r15,r3 */
 goto L8c10d880;
L8c10d880: r[3]+=r[10]; /* add r10,r3 */
 goto L8c10d882;
L8c10d882: wr(io,m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c10d884;
L8c10d884: r[2]=rd(io,m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c10d886;
L8c10d886: r[2]+=r[9]; /* add r9,r2 */
 goto L8c10d888;
L8c10d888: take=!(*sr&1u);wr(io,m,r[15]+0x4u,r[2],4);if(take)goto L8c10d864;goto L8c10d88c; /* bf/s 0x8c10d864 */
 goto L8c10d88c;
L8c10d88c: goto L8c10d8ce; /* bra 0x8c10d8ce */
 goto L8c10d890;
L8c10d890: r[10]<<=2; /* shll2 r10 */
 goto L8c10d892;
L8c10d892: r[2]=rd(io,m,r[15],4); /* mov.l @r15,r2 */
 goto L8c10d894;
L8c10d894: *sr=(*sr&~1u)|((int32_t)r[13]>0); /* cmp/pl r13 */
 goto L8c10d896;
L8c10d896: wr(io,m,r[15]+0x4u,r[2],4); /* mov.l r2,@(0x4,r15) */
 goto L8c10d898;
L8c10d898: wr(io,m,r[15],r[8],4); /* mov.l r8,@r15 */
 goto L8c10d89a;
L8c10d89a: r[8]=0u; /* mov #0x0,r8 */
 goto L8c10d89c;
L8c10d89c: wr(io,m,r[15]+0x8u,r[10],4); /* mov.l r10,@(0x8,r15) */
 goto L8c10d89e;
L8c10d89e: r[10]=r[9]; /* mov r9,r10 */
 goto L8c10d8a0;
L8c10d8a0: take=!(*sr&1u);r[10]<<=2;if(take)goto L8c10d8ce;goto L8c10d8a4; /* bf/s 0x8c10d8ce */
 goto L8c10d8a4;
L8c10d8a4: r[2]=r[8]; /* mov r8,r2 */
 goto L8c10d8a6;
L8c10d8a6: *sr=(*sr&~1u)|!(r[14]&r[2]); /* tst r14,r2 */
 goto L8c10d8a8;
L8c10d8a8: if(!(*sr&1u))goto L8c10d8b2; /* bf 0x8c10d8b2 */
 goto L8c10d8aa;
L8c10d8aa: target=r[12];*pr=0x8c10d8aeu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c10d8ae;
L8c10d8ae: target=r[11];*pr=0x8c10d8b2u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c10d8b2;
L8c10d8b2: r[3]=rd(io,m,r[15],4); /* mov.l @r15,r3 */
 goto L8c10d8b4;
L8c10d8b4: r[8]+=1u; /* add #0x1,r8 */
 goto L8c10d8b6;
L8c10d8b6: r[1]=rd(io,m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r1 */
 goto L8c10d8b8;
L8c10d8b8: *sr=(*sr&~1u)|((int32_t)r[8]>=(int32_t)r[13]); /* cmp/ge r13,r8 */
 goto L8c10d8ba;
L8c10d8ba: r[2]=rd(io,m,r[3],4); /* mov.l @r3,r2 */
 goto L8c10d8bc;
L8c10d8bc: wr(io,m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c10d8be;
L8c10d8be: r[1]=rd(io,m,r[15],4); /* mov.l @r15,r1 */
 goto L8c10d8c0;
L8c10d8c0: r[3]=rd(io,m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c10d8c2;
L8c10d8c2: r[1]+=r[3]; /* add r3,r1 */
 goto L8c10d8c4;
L8c10d8c4: wr(io,m,r[15],r[1],4); /* mov.l r1,@r15 */
 goto L8c10d8c6;
L8c10d8c6: r[2]=rd(io,m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c10d8c8;
L8c10d8c8: r[2]+=r[10]; /* add r10,r2 */
 goto L8c10d8ca;
L8c10d8ca: take=!(*sr&1u);wr(io,m,r[15]+0x4u,r[2],4);if(take)goto L8c10d8a4;goto L8c10d8ce; /* bf/s 0x8c10d8a4 */
 goto L8c10d8ce;
L8c10d8ce: r[3]=rd(io,m,0x8c10d90cu,4); /* mov.l 0x8c10d90c,r3 */
 goto L8c10d8d0;
L8c10d8d0: target=r[3];*pr=0x8c10d8d4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10d8d4;
L8c10d8d4: r[3]=rd(io,m,0x8c10d910u,4); /* mov.l 0x8c10d910,r3 */
 goto L8c10d8d6;
L8c10d8d6: r[1]=sx(io,m,0x8c10d908u,2); /* mov.w 0x8c10d908,r1 */
 goto L8c10d8d8;
L8c10d8d8: r[0]=rd(io,m,r[3],4); /* mov.l @r3,r0 */
 goto L8c10d8da;
L8c10d8da: r[0]=rd(io,m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto L8c10d8dc;
L8c10d8dc: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10d8de;
L8c10d8de: if(!(*sr&1u))goto L8c10d8f2; /* bf 0x8c10d8f2 */
 goto L8c10d8e0;
L8c10d8e0: r[0]=rd(io,m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r0 */
 goto L8c10d8e2;
L8c10d8e2: r[2]=*sr; /* stc SR,r2 */
 goto L8c10d8e4;
L8c10d8e4: r[3]=sx(io,m,0x8c10d90au,2); /* mov.w 0x8c10d90a,r3 */
 goto L8c10d8e6;
L8c10d8e6: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c10d8e8;
L8c10d8e8: r[0]<<=2; /* shll2 r0 */
 goto L8c10d8ea;
L8c10d8ea: r[2]&=r[3]; /* and r3,r2 */
 goto L8c10d8ec;
L8c10d8ec: r[0]<<=2; /* shll2 r0 */
 goto L8c10d8ee;
L8c10d8ee: r[0]|=r[2]; /* or r2,r0 */
 goto L8c10d8f0;
L8c10d8f0: *sr=r[0]; /* ldc r0,SR */
 goto L8c10d8f2;
L8c10d8f2: r[0]=0u; /* mov #0x0,r0 */
 goto L8c10d8f4;
L8c10d8f4: r[15]+=16u; /* add #0x10,r15 */
 goto L8c10d8f6;
L8c10d8f6: *pr=rd(io,m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10d8f8;
L8c10d8f8: r[8]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c10d8fa;
L8c10d8fa: r[9]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c10d8fc;
L8c10d8fc: r[10]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c10d8fe;
L8c10d8fe: r[11]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c10d900;
L8c10d900: r[12]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c10d902;
L8c10d902: r[13]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c10d904;
L8c10d904: r[14]=rd(io,m,r[15],4);r[15]+=4;return 0; /* rts */
}
