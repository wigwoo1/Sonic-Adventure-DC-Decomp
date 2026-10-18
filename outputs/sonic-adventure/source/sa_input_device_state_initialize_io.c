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
/* Static C recovery of retail Tikal helper 8c65f558..8c65f788.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_state_initialize_io(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,const struct sa_input_io *io,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f558;
L8c65f558: r[15]-=4;wr(io,m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65f55a;
L8c65f55a: r[15]-=4;wr(io,m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65f55c;
L8c65f55c: r[15]-=4;wr(io,m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65f55e;
L8c65f55e: r[15]-=4;wr(io,m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65f560;
L8c65f560: r[15]-=4;wr(io,m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65f562;
L8c65f562: r[15]-=4;wr(io,m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c65f564;
L8c65f564: r[15]-=4;wr(io,m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c65f566;
L8c65f566: r[15]-=4;wr(io,m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65f568;
L8c65f568: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c65f56a;
L8c65f56a: r[14]=rd(io,m,0x8c65f65cu,4); /* mov.l 0x8c65f65c,r14 */
 goto L8c65f56c;
L8c65f56c: r[13]=0u; /* mov #0x0,r13 */
 goto L8c65f56e;
L8c65f56e: r[0]=sx(io,m,0x8c65f63cu,2); /* mov.w 0x8c65f63c,r0 */
 goto L8c65f570;
L8c65f570: r[4]=r[13]; /* mov r13,r4 */
 goto L8c65f572;
L8c65f572: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f574;
L8c65f574: r[12]=96u; /* mov #0x60,r12 */
 goto L8c65f576;
L8c65f576: r[6]=sx(io,m,0x8c65f63eu,2); /* mov.w 0x8c65f63e,r6 */
 goto L8c65f578;
L8c65f578: r[7]=r[13]; /* mov r13,r7 */
 goto L8c65f57a;
L8c65f57a: wr(io,m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c65f57c;
L8c65f57c: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65f57e;
L8c65f57e: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f580;
L8c65f580: r[5]=sx(io,m,0x8c65f640u,2); /* mov.w 0x8c65f640,r5 */
 goto L8c65f582;
L8c65f582: wr(io,m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c65f584;
L8c65f584: r[3]=sx(io,m,0x8c65f642u,2); /* mov.w 0x8c65f642,r3 */
 goto L8c65f586;
L8c65f586: r[0]=rd(io,m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65f588;
L8c65f588: r[2]=r[0]; /* mov r0,r2 */
 goto L8c65f58a;
L8c65f58a: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65f58c;
L8c65f58c: r[0]=sx(io,m,0x8c65f644u,2); /* mov.w 0x8c65f644,r0 */
 goto L8c65f58e;
L8c65f58e: r[3]+=r[4]; /* add r4,r3 */
 goto L8c65f590;
L8c65f590: r[1]=rd(io,m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c65f592;
L8c65f592: r[1]+=r[7]; /* add r7,r1 */
 goto L8c65f594;
L8c65f594: wr(io,m,r[3],r[1],4); /* mov.l r1,@r3 */
 goto L8c65f596;
L8c65f596: r[3]=sx(io,m,0x8c65f646u,2); /* mov.w 0x8c65f646,r3 */
 goto L8c65f598;
L8c65f598: r[0]=rd(io,m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65f59a;
L8c65f59a: r[2]=r[0]; /* mov r0,r2 */
 goto L8c65f59c;
L8c65f59c: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65f59e;
L8c65f59e: r[0]=sx(io,m,0x8c65f644u,2); /* mov.w 0x8c65f644,r0 */
 goto L8c65f5a0;
L8c65f5a0: r[3]+=r[4]; /* add r4,r3 */
 goto L8c65f5a2;
L8c65f5a2: r[4]+=4u; /* add #0x4,r4 */
 goto L8c65f5a4;
L8c65f5a4: r[1]=rd(io,m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c65f5a6;
L8c65f5a6: *sr=(*sr&~1u)|(r[4]>=r[12]); /* cmp/hs r12,r4 */
 goto L8c65f5a8;
L8c65f5a8: r[1]+=r[6]; /* add r6,r1 */
 goto L8c65f5aa;
L8c65f5aa: wr(io,m,r[3],r[1],4); /* mov.l r1,@r3 */
 goto L8c65f5ac;
L8c65f5ac: r[6]+=r[5]; /* add r5,r6 */
 goto L8c65f5ae;
L8c65f5ae: take=!(*sr&1u);r[7]+=r[5];if(take)goto L8c65f584;goto L8c65f5b2; /* bf/s 0x8c65f584 */
 goto L8c65f5b2;
L8c65f5b2: r[2]=rd(io,m,r[14],4); /* mov.l @r14,r2 */
 goto L8c65f5b4;
L8c65f5b4: r[6]=8u; /* mov #0x8,r6 */
 goto L8c65f5b6;
L8c65f5b6: r[0]=sx(io,m,0x8c65f648u,2); /* mov.w 0x8c65f648,r0 */
 goto L8c65f5b8;
L8c65f5b8: r[5]=0u; /* mov #0x0,r5 */
 goto L8c65f5ba;
L8c65f5ba: r[3]=r[2]; /* mov r2,r3 */
 goto L8c65f5bc;
L8c65f5bc: r[1]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r1 */
 goto L8c65f5be;
L8c65f5be: r[0]=sx(io,m,0x8c65f64au,2); /* mov.w 0x8c65f64a,r0 */
 goto L8c65f5c0;
L8c65f5c0: wr(io,m,r[2]+r[0],r[1],4); /* mov.l r1,@(r0,r2) */
 goto L8c65f5c2;
L8c65f5c2: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f5c4;
L8c65f5c4: r[0]=sx(io,m,0x8c65f648u,2); /* mov.w 0x8c65f648,r0 */
 goto L8c65f5c6;
L8c65f5c6: r[2]=r[3]; /* mov r3,r2 */
 goto L8c65f5c8;
L8c65f5c8: r[1]=sx(io,m,0x8c65f64cu,2); /* mov.w 0x8c65f64c,r1 */
 goto L8c65f5ca;
L8c65f5ca: r[2]=rd(io,m,r[2]+r[0],4); /* mov.l @(r0,r2),r2 */
 goto L8c65f5cc;
L8c65f5cc: r[0]=sx(io,m,0x8c65f64eu,2); /* mov.w 0x8c65f64e,r0 */
 goto L8c65f5ce;
L8c65f5ce: r[2]+=r[1]; /* add r1,r2 */
 goto L8c65f5d0;
L8c65f5d0: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f5d2;
L8c65f5d2: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c65f5d4;
L8c65f5d4: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f5d6;
L8c65f5d6: r[2]=r[3]; /* mov r3,r2 */
 goto L8c65f5d8;
L8c65f5d8: r[2]=rd(io,m,r[2]+r[0],4); /* mov.l @(r0,r2),r2 */
 goto L8c65f5da;
L8c65f5da: r[0]=sx(io,m,0x8c65f650u,2); /* mov.w 0x8c65f650,r0 */
 goto L8c65f5dc;
L8c65f5dc: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f5de;
L8c65f5de: r[4]=rd(io,m,r[14],4); /* mov.l @r14,r4 */
 goto L8c65f5e0;
L8c65f5e0: r[3]=rd(io,m,0x8c65f660u,4); /* mov.l 0x8c65f660,r3 */
 goto L8c65f5e2;
L8c65f5e2: target=r[3];*pr=0x8c65f5e6u;r[4]+=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65f5e6;
L8c65f5e6: r[4]=rd(io,m,r[14],4); /* mov.l @r14,r4 */
 goto L8c65f5e8;
L8c65f5e8: r[5]=0u; /* mov #0x0,r5 */
 goto L8c65f5ea;
L8c65f5ea: r[3]=rd(io,m,0x8c65f660u,4); /* mov.l 0x8c65f660,r3 */
 goto L8c65f5ec;
L8c65f5ec: r[6]=sx(io,m,0x8c65f652u,2); /* mov.w 0x8c65f652,r6 */
 goto L8c65f5ee;
L8c65f5ee: target=r[3];*pr=0x8c65f5f2u;r[4]+=40u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65f5f2;
L8c65f5f2: r[11]=sx(io,m,0x8c65f654u,2); /* mov.w 0x8c65f654,r11 */
 goto L8c65f5f4;
L8c65f5f4: r[12]=r[13]; /* mov r13,r12 */
 goto L8c65f5f6;
L8c65f5f6: r[3]=sx(io,m,0x8c65f656u,2); /* mov.w 0x8c65f656,r3 */
 goto L8c65f5f8;
L8c65f5f8: r[4]=rd(io,m,r[14],4); /* mov.l @r14,r4 */
 goto L8c65f5fa;
L8c65f5fa: r[4]+=r[3]; /* add r3,r4 */
 goto L8c65f5fc;
L8c65f5fc: *pr=0x8c65f600u;r[4]+=r[12];status=call(ctx,0x8c6602d2u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6602d2 */
 goto L8c65f600;
L8c65f600: r[12]+=120u; /* add #0x78,r12 */
 goto L8c65f602;
L8c65f602: *sr=(*sr&~1u)|(r[12]>=r[11]); /* cmp/hs r11,r12 */
 goto L8c65f604;
L8c65f604: if(!(*sr&1u))goto L8c65f5f6; /* bf 0x8c65f5f6 */
 goto L8c65f606;
L8c65f606: r[4]=r[13]; /* mov r13,r4 */
 goto L8c65f608;
L8c65f608: r[5]=24u; /* mov #0x18,r5 */
 goto L8c65f60a;
L8c65f60a: r[3]=sx(io,m,0x8c65f658u,2); /* mov.w 0x8c65f658,r3 */
 goto L8c65f60c;
L8c65f60c: r[0]=rd(io,m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65f60e;
L8c65f60e: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65f610;
L8c65f610: r[3]+=r[4]; /* add r4,r3 */
 goto L8c65f612;
L8c65f612: r[4]+=1u; /* add #0x1,r4 */
 goto L8c65f614;
L8c65f614: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]); /* cmp/ge r5,r4 */
 goto L8c65f616;
L8c65f616: take=!(*sr&1u);wr(io,m,r[3],r[13],1);if(take)goto L8c65f60a;goto L8c65f61a; /* bf/s 0x8c65f60a */
 goto L8c65f61a;
L8c65f61a: r[2]=rd(io,m,0x8c65f664u,4); /* mov.l 0x8c65f664,r2 */
 goto L8c65f61c;
L8c65f61c: r[1]=r[15]; /* mov r15,r1 */
 goto L8c65f61e;
L8c65f61e: r[3]=rd(io,m,0x8c65f668u,4); /* mov.l 0x8c65f668,r3 */
 goto L8c65f620;
L8c65f620: r[4]=r[15]; /* mov r15,r4 */
 goto L8c65f622;
L8c65f622: r[4]+=4u; /* add #0x4,r4 */
 goto L8c65f624;
L8c65f624: r[1]+=4u; /* add #0x4,r1 */
 goto L8c65f626;
L8c65f626: target=r[3];*pr=0x8c65f62au;r[0]=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65f62a;
L8c65f62a: r[0]=rd(io,m,0x8c65f66cu,4); /* mov.l 0x8c65f66c,r0 */
 goto L8c65f62c;
L8c65f62c: r[1]=rd(io,m,r[0],4); /* mov.l @r0,r1 */
 goto L8c65f62e;
L8c65f62e: r[3]=1u; /* mov #0x1,r3 */
 goto L8c65f630;
L8c65f630: r[1]=r[1]&255u; /* extu.b r1,r1 */
 goto L8c65f632;
L8c65f632: *sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[3]); /* cmp/gt r3,r1 */
 goto L8c65f634;
L8c65f634: take=(*sr&1u);r[11]=r[13];if(take)goto L8c65f670;goto L8c65f638; /* bt/s 0x8c65f670 */
 goto L8c65f638;
L8c65f638: r[9]=7u;goto L8c65f672; /* bra 0x8c65f672 */
 goto L8c65f670;
L8c65f670: r[9]=15u; /* mov #0xf,r9 */
 goto L8c65f672;
L8c65f672: r[5]=r[4]; /* mov r4,r5 */
 goto L8c65f674;
L8c65f674: r[4]+=4u; /* add #0x4,r4 */
 goto L8c65f676;
L8c65f676: r[6]=5u; /* mov #0x5,r6 */
 goto L8c65f678;
L8c65f678: wr(io,m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c65f67a;
L8c65f67a: r[7]=r[13]; /* mov r13,r7 */
 goto L8c65f67c;
L8c65f67c: r[8]=82u; /* mov #0x52,r8 */
 goto L8c65f67e;
L8c65f67e: r[10]=69u; /* mov #0x45,r10 */
 goto L8c65f680;
L8c65f680: r[3]=sx(io,m,r[5],1); /* mov.b @r5,r3 */
 goto L8c65f682;
L8c65f682: r[2]=(uint32_t)(int32_t)(int8_t)r[9]; /* exts.b r9,r2 */
 goto L8c65f684;
L8c65f684: *sr=(*sr&~1u)|!(r[2]&r[3]); /* tst r2,r3 */
 goto L8c65f686;
L8c65f686: if((*sr&1u))goto L8c65f6aa; /* bt 0x8c65f6aa */
 goto L8c65f688;
L8c65f688: r[3]=sx(io,m,0x8c65f788u,2); /* mov.w 0x8c65f788,r3 */
 goto L8c65f68a;
L8c65f68a: r[4]=r[11]; /* mov r11,r4 */
 goto L8c65f68c;
L8c65f68c: r[0]=rd(io,m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65f68e;
L8c65f68e: r[4]+=1u; /* add #0x1,r4 */
 goto L8c65f690;
L8c65f690: r[12]=r[13]; /* mov r13,r12 */
 goto L8c65f692;
L8c65f692: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65f694;
L8c65f694: r[3]+=r[7]; /* add r7,r3 */
 goto L8c65f696;
L8c65f696: wr(io,m,r[3],r[8],1); /* mov.b r8,@r3 */
 goto L8c65f698;
L8c65f698: r[3]=sx(io,m,0x8c65f788u,2); /* mov.w 0x8c65f788,r3 */
 goto L8c65f69a;
L8c65f69a: r[12]+=1u; /* add #0x1,r12 */
 goto L8c65f69c;
L8c65f69c: r[0]=rd(io,m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65f69e;
L8c65f69e: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[6]); /* cmp/ge r6,r12 */
 goto L8c65f6a0;
L8c65f6a0: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65f6a2;
L8c65f6a2: r[3]+=r[4]; /* add r4,r3 */
 goto L8c65f6a4;
L8c65f6a4: wr(io,m,r[3],r[10],1); /* mov.b r10,@r3 */
 goto L8c65f6a6;
L8c65f6a6: take=!(*sr&1u);r[4]+=1u;if(take)goto L8c65f698;goto L8c65f6aa; /* bf/s 0x8c65f698 */
 goto L8c65f6aa;
L8c65f6aa: r[3]=rd(io,m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65f6ac;
L8c65f6ac: r[5]+=1u; /* add #0x1,r5 */
 goto L8c65f6ae;
L8c65f6ae: r[11]+=6u; /* add #0x6,r11 */
 goto L8c65f6b0;
L8c65f6b0: *sr=(*sr&~1u)|(r[5]>=r[3]); /* cmp/hs r3,r5 */
 goto L8c65f6b2;
L8c65f6b2: take=!(*sr&1u);r[7]+=6u;if(take)goto L8c65f680;goto L8c65f6b6; /* bf/s 0x8c65f680 */
 goto L8c65f6b6;
L8c65f6b6: r[0]=sx(io,m,0x8c65f78au,2); /* mov.w 0x8c65f78a,r0 */
 goto L8c65f6b8;
L8c65f6b8: r[3]=rd(io,m,0x8c65f798u,4); /* mov.l 0x8c65f798,r3 */
 goto L8c65f6ba;
L8c65f6ba: r[1]=rd(io,m,r[14],4); /* mov.l @r14,r1 */
 goto L8c65f6bc;
L8c65f6bc: wr(io,m,r[1]+r[0],r[3],4); /* mov.l r3,@(r0,r1) */
 goto L8c65f6be;
L8c65f6be: r[0]=sx(io,m,0x8c65f78cu,2); /* mov.w 0x8c65f78c,r0 */
 goto L8c65f6c0;
L8c65f6c0: r[2]=rd(io,m,r[14],4); /* mov.l @r14,r2 */
 goto L8c65f6c2;
L8c65f6c2: wr(io,m,r[2]+r[0],r[13],4); /* mov.l r13,@(r0,r2) */
 goto L8c65f6c4;
L8c65f6c4: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c65f6c6;
L8c65f6c6: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f6c8;
L8c65f6c8: r[2]=1u; /* mov #0x1,r2 */
 goto L8c65f6ca;
L8c65f6ca: wr(io,m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c65f6cc;
L8c65f6cc: r[0]+=8u; /* add #0x8,r0 */
 goto L8c65f6ce;
L8c65f6ce: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f6d0;
L8c65f6d0: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f6d2;
L8c65f6d2: r[2]=10u; /* mov #0xa,r2 */
 goto L8c65f6d4;
L8c65f6d4: r[0]=sx(io,m,0x8c65f78eu,2); /* mov.w 0x8c65f78e,r0 */
 goto L8c65f6d6;
L8c65f6d6: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f6d8;
L8c65f6d8: wr(io,m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c65f6da;
L8c65f6da: r[0]+=36u; /* add #0x24,r0 */
 goto L8c65f6dc;
L8c65f6dc: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f6de;
L8c65f6de: wr(io,m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c65f6e0;
L8c65f6e0: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c65f6e2;
L8c65f6e2: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f6e4;
L8c65f6e4: wr(io,m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c65f6e6;
L8c65f6e6: r[0]+=4294967268u; /* add #-0x1c,r0 */
 goto L8c65f6e8;
L8c65f6e8: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f6ea;
L8c65f6ea: wr(io,m,r[3]+r[0],r[6],4); /* mov.l r6,@(r0,r3) */
 goto L8c65f6ec;
L8c65f6ec: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65f6ee;
L8c65f6ee: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f6f0;
L8c65f6f0: wr(io,m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c65f6f2;
L8c65f6f2: r[0]+=8u; /* add #0x8,r0 */
 goto L8c65f6f4;
L8c65f6f4: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f6f6;
L8c65f6f6: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f6f8;
L8c65f6f8: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65f6fa;
L8c65f6fa: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f6fc;
L8c65f6fc: r[2]=rd(io,m,0x8c65f79cu,4); /* mov.l 0x8c65f79c,r2 */
 goto L8c65f6fe;
L8c65f6fe: wr(io,m,r[3]+r[0],r[6],4); /* mov.l r6,@(r0,r3) */
 goto L8c65f700;
L8c65f700: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65f702;
L8c65f702: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f704;
L8c65f704: wr(io,m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c65f706;
L8c65f706: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65f708;
L8c65f708: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f70a;
L8c65f70a: wr(io,m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c65f70c;
L8c65f70c: r[0]=sx(io,m,0x8c65f790u,2); /* mov.w 0x8c65f790,r0 */
 goto L8c65f70e;
L8c65f70e: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f710;
L8c65f710: wr(io,m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c65f712;
L8c65f712: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65f714;
L8c65f714: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f716;
L8c65f716: wr(io,m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c65f718;
L8c65f718: r[0]+=12u; /* add #0xc,r0 */
 goto L8c65f71a;
L8c65f71a: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f71c;
L8c65f71c: wr(io,m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c65f71e;
L8c65f71e: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f720;
L8c65f720: r[0]=sx(io,m,0x8c65f792u,2); /* mov.w 0x8c65f792,r0 */
 goto L8c65f722;
L8c65f722: wr(io,m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c65f724;
L8c65f724: r[2]=rd(io,m,0x8c65f7a0u,4); /* mov.l 0x8c65f7a0,r2 */
 goto L8c65f726;
L8c65f726: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f728;
L8c65f728: wr(io,m,r[3]+0x24u,r[2],4); /* mov.l r2,@(0x24,r3) */
 goto L8c65f72a;
L8c65f72a: r[2]=rd(io,m,0x8c65f7a4u,4); /* mov.l 0x8c65f7a4,r2 */
 goto L8c65f72c;
L8c65f72c: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f72e;
L8c65f72e: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f730;
L8c65f730: r[0]=sx(io,m,0x8c65f794u,2); /* mov.w 0x8c65f794,r0 */
 goto L8c65f732;
L8c65f732: r[2]=rd(io,m,0x8c65f7a8u,4); /* mov.l 0x8c65f7a8,r2 */
 goto L8c65f734;
L8c65f734: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f736;
L8c65f736: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f738;
L8c65f738: r[0]+=28u; /* add #0x1c,r0 */
 goto L8c65f73a;
L8c65f73a: r[2]=rd(io,m,0x8c65f7acu,4); /* mov.l 0x8c65f7ac,r2 */
 goto L8c65f73c;
L8c65f73c: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f73e;
L8c65f73e: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f740;
L8c65f740: r[0]+=60u; /* add #0x3c,r0 */
 goto L8c65f742;
L8c65f742: r[2]=rd(io,m,0x8c65f7b0u,4); /* mov.l 0x8c65f7b0,r2 */
 goto L8c65f744;
L8c65f744: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f746;
L8c65f746: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f748;
L8c65f748: r[0]=sx(io,m,0x8c65f796u,2); /* mov.w 0x8c65f796,r0 */
 goto L8c65f74a;
L8c65f74a: r[2]=rd(io,m,0x8c65f7b4u,4); /* mov.l 0x8c65f7b4,r2 */
 goto L8c65f74c;
L8c65f74c: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f74e;
L8c65f74e: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f750;
L8c65f750: r[0]+=24u; /* add #0x18,r0 */
 goto L8c65f752;
L8c65f752: r[2]=rd(io,m,0x8c65f7b8u,4); /* mov.l 0x8c65f7b8,r2 */
 goto L8c65f754;
L8c65f754: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f756;
L8c65f756: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f758;
L8c65f758: r[0]+=28u; /* add #0x1c,r0 */
 goto L8c65f75a;
L8c65f75a: r[2]=rd(io,m,0x8c65f7bcu,4); /* mov.l 0x8c65f7bc,r2 */
 goto L8c65f75c;
L8c65f75c: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f75e;
L8c65f75e: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f760;
L8c65f760: r[0]+=8u; /* add #0x8,r0 */
 goto L8c65f762;
L8c65f762: r[2]=rd(io,m,0x8c65f7c0u,4); /* mov.l 0x8c65f7c0,r2 */
 goto L8c65f764;
L8c65f764: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f766;
L8c65f766: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f768;
L8c65f768: r[3]=rd(io,m,0x8c65f7c4u,4); /* mov.l 0x8c65f7c4,r3 */
 goto L8c65f76a;
L8c65f76a: target=r[3];*pr=0x8c65f76eu;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65f76e;
L8c65f76e: r[2]=rd(io,m,0x8c65f7c8u,4); /* mov.l 0x8c65f7c8,r2 */
 goto L8c65f770;
L8c65f770: target=r[2];*pr=0x8c65f774u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65f774;
L8c65f774: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65f776;
L8c65f776: *pr=rd(io,m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65f778;
L8c65f778: r[8]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c65f77a;
L8c65f77a: r[9]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c65f77c;
L8c65f77c: r[10]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65f77e;
L8c65f77e: r[11]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65f780;
L8c65f780: r[12]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65f782;
L8c65f782: r[13]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65f784;
L8c65f784: r[14]=rd(io,m,r[15],4);r[15]+=4;return 0; /* rts */
}
