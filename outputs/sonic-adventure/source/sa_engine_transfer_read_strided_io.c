#include "input_io.h"
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
/* Static C recovery of retail Tikal helper 8c10d460..8c10d59a.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_transfer_read_strided_io(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,const struct sa_input_io *io,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d460;
L8c10d460: r[15]-=4;wr(io,m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c10d462;
L8c10d462: r[15]-=4;wr(io,m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c10d464;
L8c10d464: r[15]-=4;wr(io,m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c10d466;
L8c10d466: r[15]-=4;wr(io,m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c10d468;
L8c10d468: r[15]-=4;wr(io,m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c10d46a;
L8c10d46a: r[15]-=4;wr(io,m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c10d46c;
L8c10d46c: r[15]-=4;wr(io,m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10d46e;
L8c10d46e: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c10d470;
L8c10d470: r[9]=r[5]; /* mov r5,r9 */
 goto L8c10d472;
L8c10d472: r[10]=r[4]; /* mov r4,r10 */
 goto L8c10d474;
L8c10d474: r[4]=4294967295u; /* mov #-0x1,r4 */
 goto L8c10d476;
L8c10d476: wr(io,m,r[15],r[6],4); /* mov.l r6,@r15 */
 goto L8c10d478;
L8c10d478: r[0]=rd(io,m,r[15]+0x24u,4); /* mov.l @(0x24,r15),r0 */
 goto L8c10d47a;
L8c10d47a: r[14]=r[7]; /* mov r7,r14 */
 goto L8c10d47c;
L8c10d47c: r[5]=1u; /* mov #0x1,r5 */
 goto L8c10d47e;
L8c10d47e: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c10d480;
L8c10d480: take=(*sr&1u);r[11]=0u;if(take)goto L8c10d490;goto L8c10d484; /* bt/s 0x8c10d490 */
 goto L8c10d484;
L8c10d484: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10d486;
L8c10d486: if((*sr&1u))goto L8c10d494; /* bt 0x8c10d494 */
 goto L8c10d488;
L8c10d488: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c10d48a;
L8c10d48a: if((*sr&1u))goto L8c10d498; /* bt 0x8c10d498 */
 goto L8c10d48c;
L8c10d48c: goto L8c10d49a; /* bra 0x8c10d49a */
 goto L8c10d490;
L8c10d490: r[12]=r[11];goto L8c10d49a; /* bra 0x8c10d49a */
 goto L8c10d494;
L8c10d494: r[12]=r[5];goto L8c10d49a; /* bra 0x8c10d49a */
 goto L8c10d498;
L8c10d498: r[12]=r[4]; /* mov r4,r12 */
 goto L8c10d49a;
L8c10d49a: r[0]=rd(io,m,r[15]+0x28u,4); /* mov.l @(0x28,r15),r0 */
 goto L8c10d49c;
L8c10d49c: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c10d49e;
L8c10d49e: if((*sr&1u))goto L8c10d4ac; /* bt 0x8c10d4ac */
 goto L8c10d4a0;
L8c10d4a0: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10d4a2;
L8c10d4a2: if((*sr&1u))goto L8c10d4b0; /* bt 0x8c10d4b0 */
 goto L8c10d4a4;
L8c10d4a4: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c10d4a6;
L8c10d4a6: if((*sr&1u))goto L8c10d4b4; /* bt 0x8c10d4b4 */
 goto L8c10d4a8;
L8c10d4a8: goto L8c10d4b6; /* bra 0x8c10d4b6 */
 goto L8c10d4ac;
L8c10d4ac: r[13]=r[11];goto L8c10d4b6; /* bra 0x8c10d4b6 */
 goto L8c10d4b0;
L8c10d4b0: r[13]=r[5];goto L8c10d4b6; /* bra 0x8c10d4b6 */
 goto L8c10d4b4;
L8c10d4b4: r[13]=r[4]; /* mov r4,r13 */
 goto L8c10d4b6;
L8c10d4b6: r[3]=rd(io,m,0x8c10d538u,4); /* mov.l 0x8c10d538,r3 */
 goto L8c10d4b8;
L8c10d4b8: r[1]=sx(io,m,0x8c10d532u,2); /* mov.w 0x8c10d532,r1 */
 goto L8c10d4ba;
L8c10d4ba: r[0]=rd(io,m,r[3],4); /* mov.l @r3,r0 */
 goto L8c10d4bc;
L8c10d4bc: r[0]=rd(io,m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto L8c10d4be;
L8c10d4be: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10d4c0;
L8c10d4c0: if(!(*sr&1u))goto L8c10d4d6; /* bf 0x8c10d4d6 */
 goto L8c10d4c2;
L8c10d4c2: r[0]=*sr; /* stc SR,r0 */
 goto L8c10d4c4;
L8c10d4c4: r[3]=sx(io,m,0x8c10d534u,2); /* mov.w 0x8c10d534,r3 */
 goto L8c10d4c6;
L8c10d4c6: r[0]>>=2; /* shlr2 r0 */
 goto L8c10d4c8;
L8c10d4c8: r[0]>>=2; /* shlr2 r0 */
 goto L8c10d4ca;
L8c10d4ca: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c10d4cc;
L8c10d4cc: wr(io,m,r[15]+0x4u,r[0],4); /* mov.l r0,@(0x4,r15) */
 goto L8c10d4ce;
L8c10d4ce: r[0]=*sr; /* stc SR,r0 */
 goto L8c10d4d0;
L8c10d4d0: r[0]&=r[3]; /* and r3,r0 */
 goto L8c10d4d2;
L8c10d4d2: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c10d4d4;
L8c10d4d4: *sr=r[0]; /* ldc r0,SR */
 goto L8c10d4d6;
L8c10d4d6: r[2]=rd(io,m,0x8c10d53cu,4); /* mov.l 0x8c10d53c,r2 */
 goto L8c10d4d8;
L8c10d4d8: target=r[2];*pr=0x8c10d4dcu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c10d4dc;
L8c10d4dc: r[1]=rd(io,m,0x8c10d540u,4); /* mov.l 0x8c10d540,r1 */
 goto L8c10d4de;
L8c10d4de: target=r[1];*pr=0x8c10d4e2u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c10d4e2;
L8c10d4e2: r[0]=rd(io,m,r[15],4); /* mov.l @r15,r0 */
 goto L8c10d4e4;
L8c10d4e4: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10d4e6;
L8c10d4e6: if((*sr&1u))goto L8c10d4f4; /* bt 0x8c10d4f4 */
 goto L8c10d4e8;
L8c10d4e8: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c10d4ea;
L8c10d4ea: if((*sr&1u))goto L8c10d510; /* bt 0x8c10d510 */
 goto L8c10d4ec;
L8c10d4ec: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c10d4ee;
L8c10d4ee: if((*sr&1u))goto L8c10d544; /* bt 0x8c10d544 */
 goto L8c10d4f0;
L8c10d4f0: goto L8c10d562; /* bra 0x8c10d562 */
 goto L8c10d4f4;
L8c10d4f4: *sr=(*sr&~1u)|((int32_t)r[14]>0); /* cmp/pl r14 */
 goto L8c10d4f6;
L8c10d4f6: r[5]=r[10]; /* mov r10,r5 */
 goto L8c10d4f8;
L8c10d4f8: r[6]=r[9]; /* mov r9,r6 */
 goto L8c10d4fa;
L8c10d4fa: take=!(*sr&1u);r[4]=r[11];if(take)goto L8c10d562;goto L8c10d4fe; /* bf/s 0x8c10d562 */
 goto L8c10d4fe;
L8c10d4fe: r[2]=sx(io,m,r[5],1); /* mov.b @r5,r2 */
 goto L8c10d500;
L8c10d500: r[4]+=1u; /* add #0x1,r4 */
 goto L8c10d502;
L8c10d502: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[14]); /* cmp/ge r14,r4 */
 goto L8c10d504;
L8c10d504: r[5]+=r[12]; /* add r12,r5 */
 goto L8c10d506;
L8c10d506: wr(io,m,r[6],r[2],1); /* mov.b r2,@r6 */
 goto L8c10d508;
L8c10d508: take=!(*sr&1u);r[6]+=r[13];if(take)goto L8c10d4fe;goto L8c10d50c; /* bf/s 0x8c10d4fe */
 goto L8c10d50c;
L8c10d50c: goto L8c10d562; /* bra 0x8c10d562 */
 goto L8c10d510;
L8c10d510: r[7]=r[12]; /* mov r12,r7 */
 goto L8c10d512;
L8c10d512: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1; /* shll r7 */
 goto L8c10d514;
L8c10d514: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1; /* shll r13 */
 goto L8c10d516;
L8c10d516: *sr=(*sr&~1u)|((int32_t)r[14]>0); /* cmp/pl r14 */
 goto L8c10d518;
L8c10d518: r[6]=r[10]; /* mov r10,r6 */
 goto L8c10d51a;
L8c10d51a: r[4]=r[9]; /* mov r9,r4 */
 goto L8c10d51c;
L8c10d51c: take=!(*sr&1u);r[5]=r[11];if(take)goto L8c10d562;goto L8c10d520; /* bf/s 0x8c10d562 */
 goto L8c10d520;
L8c10d520: r[2]=sx(io,m,r[6],2); /* mov.w @r6,r2 */
 goto L8c10d522;
L8c10d522: r[5]+=1u; /* add #0x1,r5 */
 goto L8c10d524;
L8c10d524: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[14]); /* cmp/ge r14,r5 */
 goto L8c10d526;
L8c10d526: r[6]+=r[7]; /* add r7,r6 */
 goto L8c10d528;
L8c10d528: wr(io,m,r[4],r[2],2); /* mov.w r2,@r4 */
 goto L8c10d52a;
L8c10d52a: take=!(*sr&1u);r[4]+=r[13];if(take)goto L8c10d520;goto L8c10d52e; /* bf/s 0x8c10d520 */
 goto L8c10d52e;
L8c10d52e: goto L8c10d562; /* bra 0x8c10d562 */
 goto L8c10d544;
L8c10d544: r[7]=r[12]; /* mov r12,r7 */
 goto L8c10d546;
L8c10d546: r[7]<<=2; /* shll2 r7 */
 goto L8c10d548;
L8c10d548: *sr=(*sr&~1u)|((int32_t)r[14]>0); /* cmp/pl r14 */
 goto L8c10d54a;
L8c10d54a: r[4]=r[9]; /* mov r9,r4 */
 goto L8c10d54c;
L8c10d54c: r[5]=r[10]; /* mov r10,r5 */
 goto L8c10d54e;
L8c10d54e: r[6]=r[11]; /* mov r11,r6 */
 goto L8c10d550;
L8c10d550: take=!(*sr&1u);r[13]<<=2;if(take)goto L8c10d562;goto L8c10d554; /* bf/s 0x8c10d562 */
 goto L8c10d554;
L8c10d554: r[2]=rd(io,m,r[5],4); /* mov.l @r5,r2 */
 goto L8c10d556;
L8c10d556: r[6]+=1u; /* add #0x1,r6 */
 goto L8c10d558;
L8c10d558: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[14]); /* cmp/ge r14,r6 */
 goto L8c10d55a;
L8c10d55a: r[5]+=r[7]; /* add r7,r5 */
 goto L8c10d55c;
L8c10d55c: wr(io,m,r[4],r[2],4); /* mov.l r2,@r4 */
 goto L8c10d55e;
L8c10d55e: take=!(*sr&1u);r[4]+=r[13];if(take)goto L8c10d554;goto L8c10d562; /* bf/s 0x8c10d554 */
 goto L8c10d562;
L8c10d562: r[3]=rd(io,m,0x8c10d5a0u,4); /* mov.l 0x8c10d5a0,r3 */
 goto L8c10d564;
L8c10d564: target=r[3];*pr=0x8c10d568u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10d568;
L8c10d568: r[3]=rd(io,m,0x8c10d5a4u,4); /* mov.l 0x8c10d5a4,r3 */
 goto L8c10d56a;
L8c10d56a: r[1]=sx(io,m,0x8c10d59au,2); /* mov.w 0x8c10d59a,r1 */
 goto L8c10d56c;
L8c10d56c: r[0]=rd(io,m,r[3],4); /* mov.l @r3,r0 */
 goto L8c10d56e;
L8c10d56e: r[0]=rd(io,m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto L8c10d570;
L8c10d570: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10d572;
L8c10d572: if(!(*sr&1u))goto L8c10d586; /* bf 0x8c10d586 */
 goto L8c10d574;
L8c10d574: r[0]=rd(io,m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r0 */
 goto L8c10d576;
L8c10d576: r[2]=*sr; /* stc SR,r2 */
 goto L8c10d578;
L8c10d578: r[3]=sx(io,m,0x8c10d59cu,2); /* mov.w 0x8c10d59c,r3 */
 goto L8c10d57a;
L8c10d57a: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c10d57c;
L8c10d57c: r[0]<<=2; /* shll2 r0 */
 goto L8c10d57e;
L8c10d57e: r[2]&=r[3]; /* and r3,r2 */
 goto L8c10d580;
L8c10d580: r[0]<<=2; /* shll2 r0 */
 goto L8c10d582;
L8c10d582: r[0]|=r[2]; /* or r2,r0 */
 goto L8c10d584;
L8c10d584: *sr=r[0]; /* ldc r0,SR */
 goto L8c10d586;
L8c10d586: r[0]=0u; /* mov #0x0,r0 */
 goto L8c10d588;
L8c10d588: r[15]+=8u; /* add #0x8,r15 */
 goto L8c10d58a;
L8c10d58a: *pr=rd(io,m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10d58c;
L8c10d58c: r[9]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c10d58e;
L8c10d58e: r[10]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c10d590;
L8c10d590: r[11]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c10d592;
L8c10d592: r[12]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c10d594;
L8c10d594: r[13]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c10d596;
L8c10d596: r[14]=rd(io,m,r[15],4);r[15]+=4;return 0; /* rts */
}
