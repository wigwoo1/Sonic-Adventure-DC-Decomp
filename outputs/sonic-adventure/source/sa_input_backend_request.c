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
/* Static C recovery of retail Tikal helper 8c6608a4..8c660a7e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
static int sa_input_backend_request_core(uint32_t entry,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 switch(entry){case 0:goto L8c6608a4;case 0x8c660906u:goto L8c660906;case 0x8c66091au:goto L8c66091a;case 0x8c660932u:goto L8c660932;case 0x8c660a68u:goto L8c660a68;default:return -4;}
L8c6608a4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6608a6;
L8c6608a6: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6608a8;
L8c6608a8: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6608aa;
L8c6608aa: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c6608ac;
L8c6608ac: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c6608ae;
L8c6608ae: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c6608b0;
L8c6608b0: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c6608b2;
L8c6608b2: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6608b4;
L8c6608b4: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto L8c6608b6;
L8c6608b6: r[13]=r[4]; /* mov r4,r13 */
 goto L8c6608b8;
L8c6608b8: r[0]=rd(m,0x8c660958u,4); /* mov.l 0x8c660958,r0 */
 goto L8c6608ba;
L8c6608ba: r[13]<<=2; /* shll2 r13 */
 goto L8c6608bc;
L8c6608bc: r[10]=rd(m,0x8c66095cu,4); /* mov.l 0x8c66095c,r10 */
 goto L8c6608be;
L8c6608be: r[13]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r13 */
 goto L8c6608c0;
L8c6608c0: r[14]=r[5]; /* mov r5,r14 */
 goto L8c6608c2;
L8c6608c2: target=r[10];*pr=0x8c6608c6u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r10 */
 goto L8c6608c6;
L8c6608c6: r[3]=rd(m,0x8c660960u,4); /* mov.l 0x8c660960,r3 */
 goto L8c6608c8;
L8c6608c8: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c6608ca;
L8c6608ca: target=r[3];*pr=0x8c6608ceu;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6608ce;
L8c6608ce: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c6608d0;
L8c6608d0: r[4]=r[0]; /* mov r0,r4 */
 goto L8c6608d2;
L8c6608d2: r[11]=rd(m,0x8c660964u,4); /* mov.l 0x8c660964,r11 */
 goto L8c6608d4;
L8c6608d4: r[3]=sx(m,r[2],1); /* mov.b @r2,r3 */
 goto L8c6608d6;
L8c6608d6: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c6608d8;
L8c6608d8: if(!(*sr&1u))goto L8c6608de; /* bf 0x8c6608de */
 goto L8c6608da;
L8c6608da: goto L8c660a64; /* bra 0x8c660a64 */
 goto L8c6608de;
L8c6608de: r[0]=rd(m,r[4],4); /* mov.l @r4,r0 */
 goto L8c6608e0;
L8c6608e0: *sr=(*sr&~1u)|!(r[0]&0x2u); /* tst #0x2,r0 */
 goto L8c6608e2;
L8c6608e2: if(!(*sr&1u))goto L8c6608e8; /* bf 0x8c6608e8 */
 goto L8c6608e4;
L8c6608e4: goto L8c660a64; /* bra 0x8c660a64 */
 goto L8c6608e8;
L8c6608e8: r[2]=rd(m,0x8c660968u,4); /* mov.l 0x8c660968,r2 */
 goto L8c6608ea;
L8c6608ea: target=r[2];*pr=0x8c6608eeu;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6608ee;
L8c6608ee: r[12]=rd(m,0x8c66096cu,4); /* mov.l 0x8c66096c,r12 */
 goto L8c6608f0;
L8c6608f0: target=r[10];*pr=0x8c6608f4u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r10 */
 goto L8c6608f4;
L8c6608f4: r[5]=2u; /* mov #0x2,r5 */
 goto L8c6608f6;
L8c6608f6: r[6]=0u; /* mov #0x0,r6 */
 goto L8c6608f8;
L8c6608f8: target=r[12];*pr=0x8c6608fcu;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c6608fc;
L8c6608fc: r[9]=r[0]; /* mov r0,r9 */
 goto L8c6608fe;
L8c6608fe: *sr=(*sr&~1u)|((int32_t)r[9]>=0); /* cmp/pz r9 */
 goto L8c660900;
L8c660900: if((*sr&1u))goto L8c66090a; /* bt 0x8c66090a */
 goto L8c660902;
L8c660902: target=r[11];*pr=0x8c660906u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c660906;
L8c660906: goto L8c6608f0; /* bra 0x8c6608f0 */
 goto L8c66090a;
L8c66090a: r[12]=rd(m,0x8c660970u,4); /* mov.l 0x8c660970,r12 */
 goto L8c66090c;
L8c66090c: r[5]=r[9]; /* mov r9,r5 */
 goto L8c66090e;
L8c66090e: target=r[12];*pr=0x8c660912u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c660912;
L8c660912: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c660914;
L8c660914: if(!(*sr&1u))goto L8c66091e; /* bf 0x8c66091e */
 goto L8c660916;
L8c660916: target=r[11];*pr=0x8c66091au;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c66091a;
L8c66091a: goto L8c66090c; /* bra 0x8c66090c */
 goto L8c66091e;
L8c66091e: target=r[10];*pr=0x8c660922u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r10 */
 goto L8c660922;
L8c660922: r[3]=rd(m,0x8c660974u,4); /* mov.l 0x8c660974,r3 */
 goto L8c660924;
L8c660924: r[8]=r[0]; /* mov r0,r8 */
 goto L8c660926;
L8c660926: target=r[3];*pr=0x8c66092au;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66092a;
L8c66092a: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c66092c;
L8c66092c: if((*sr&1u))goto L8c660938; /* bt 0x8c660938 */
 goto L8c66092e;
L8c66092e: target=r[11];*pr=0x8c660932u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c660932;
L8c660932: r[0]=sx(m,0x8c66094eu,2); /* mov.w 0x8c66094e,r0 */
 goto L8c660934;
L8c660934: goto L8c660a6a; /* bra 0x8c660a6a */
 goto L8c660938;
L8c660938: r[0]=sx(m,r[8]+0x4u,1); /* mov.b @(0x4,r8),r0 */
 goto L8c66093a;
L8c66093a: r[12]=rd(m,r[8]+0x8u,4); /* mov.l @(0x8,r8),r12 */
 goto L8c66093c;
L8c66093c: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c66093e;
L8c66093e: *sr=(*sr&~1u)|(r[0]==0x8u); /* cmp/eq #0x8,r0 */
 goto L8c660940;
L8c660940: take=(*sr&1u);r[12]+=8u;if(take)goto L8c660978;goto L8c660944; /* bt/s 0x8c660978 */
 goto L8c660944;
L8c660944: r[1]=sx(m,0x8c660950u,2); /* mov.w 0x8c660950,r1 */
 goto L8c660946;
L8c660946: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c660948;
L8c660948: if((*sr&1u))goto L8c6608ee; /* bt 0x8c6608ee */
 goto L8c66094a;
L8c66094a: goto L8c660a68; /* bra 0x8c660a68 */
 goto L8c660978;
L8c660978: r[10]=rd(m,0x8c660ad4u,4); /* mov.l 0x8c660ad4,r10 */
 goto L8c66097a;
L8c66097a: r[5]=0u; /* mov #0x0,r5 */
 goto L8c66097c;
L8c66097c: r[2]=rd(m,r[10],4); /* mov.l @r10,r2 */
 goto L8c66097e;
L8c66097e: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c660980;
L8c660980: target=r[3];*pr=0x8c660984u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c660984;
L8c660984: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c660986;
L8c660986: r[5]=2u; /* mov #0x2,r5 */
 goto L8c660988;
L8c660988: wr(m,r[14],r[0],4); /* mov.l r0,@r14 */
 goto L8c66098a;
L8c66098a: r[2]=rd(m,r[10],4); /* mov.l @r10,r2 */
 goto L8c66098c;
L8c66098c: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c66098e;
L8c66098e: target=r[3];*pr=0x8c660992u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c660992;
L8c660992: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c660994;
L8c660994: r[5]=4u; /* mov #0x4,r5 */
 goto L8c660996;
L8c660996: wr(m,r[14]+0x4u,r[0],4); /* mov.l r0,@(0x4,r14) */
 goto L8c660998;
L8c660998: r[2]=rd(m,r[10],4); /* mov.l @r10,r2 */
 goto L8c66099a;
L8c66099a: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c66099c;
L8c66099c: target=r[3];*pr=0x8c6609a0u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6609a0;
L8c6609a0: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c6609a2;
L8c6609a2: r[5]=6u; /* mov #0x6,r5 */
 goto L8c6609a4;
L8c6609a4: wr(m,r[14]+0x8u,r[0],4); /* mov.l r0,@(0x8,r14) */
 goto L8c6609a6;
L8c6609a6: r[2]=rd(m,r[10],4); /* mov.l @r10,r2 */
 goto L8c6609a8;
L8c6609a8: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c6609aa;
L8c6609aa: target=r[3];*pr=0x8c6609aeu;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6609ae;
L8c6609ae: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c6609b0;
L8c6609b0: r[5]=8u; /* mov #0x8,r5 */
 goto L8c6609b2;
L8c6609b2: wr(m,r[14]+0xcu,r[0],4); /* mov.l r0,@(0xc,r14) */
 goto L8c6609b4;
L8c6609b4: r[2]=rd(m,r[10],4); /* mov.l @r10,r2 */
 goto L8c6609b6;
L8c6609b6: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c6609b8;
L8c6609b8: target=r[3];*pr=0x8c6609bcu;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6609bc;
L8c6609bc: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c6609be;
L8c6609be: r[5]=10u; /* mov #0xa,r5 */
 goto L8c6609c0;
L8c6609c0: wr(m,r[14]+0x10u,r[0],4); /* mov.l r0,@(0x10,r14) */
 goto L8c6609c2;
L8c6609c2: r[2]=rd(m,r[10],4); /* mov.l @r10,r2 */
 goto L8c6609c4;
L8c6609c4: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c6609c6;
L8c6609c6: target=r[3];*pr=0x8c6609cau;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6609ca;
L8c6609ca: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c6609cc;
L8c6609cc: r[5]=12u; /* mov #0xc,r5 */
 goto L8c6609ce;
L8c6609ce: wr(m,r[14]+0x14u,r[0],4); /* mov.l r0,@(0x14,r14) */
 goto L8c6609d0;
L8c6609d0: r[2]=rd(m,r[10],4); /* mov.l @r10,r2 */
 goto L8c6609d2;
L8c6609d2: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c6609d4;
L8c6609d4: target=r[3];*pr=0x8c6609d8u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6609d8;
L8c6609d8: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c6609da;
L8c6609da: r[5]=16u; /* mov #0x10,r5 */
 goto L8c6609dc;
L8c6609dc: wr(m,r[14]+0x18u,r[0],4); /* mov.l r0,@(0x18,r14) */
 goto L8c6609de;
L8c6609de: r[2]=rd(m,r[10],4); /* mov.l @r10,r2 */
 goto L8c6609e0;
L8c6609e0: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c6609e2;
L8c6609e2: target=r[3];*pr=0x8c6609e6u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6609e6;
L8c6609e6: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c6609e8;
L8c6609e8: r[5]=18u; /* mov #0x12,r5 */
 goto L8c6609ea;
L8c6609ea: wr(m,r[14]+0x1cu,r[0],4); /* mov.l r0,@(0x1c,r14) */
 goto L8c6609ec;
L8c6609ec: r[2]=rd(m,r[10],4); /* mov.l @r10,r2 */
 goto L8c6609ee;
L8c6609ee: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c6609f0;
L8c6609f0: target=r[3];*pr=0x8c6609f4u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6609f4;
L8c6609f4: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c6609f6;
L8c6609f6: r[5]=20u; /* mov #0x14,r5 */
 goto L8c6609f8;
L8c6609f8: wr(m,r[14]+0x20u,r[0],4); /* mov.l r0,@(0x20,r14) */
 goto L8c6609fa;
L8c6609fa: r[2]=rd(m,r[10],4); /* mov.l @r10,r2 */
 goto L8c6609fc;
L8c6609fc: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c6609fe;
L8c6609fe: target=r[3];*pr=0x8c660a02u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c660a02;
L8c660a02: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c660a04;
L8c660a04: r[2]=sx(m,0x8c660ac8u,2); /* mov.w 0x8c660ac8,r2 */
 goto L8c660a06;
L8c660a06: wr(m,r[14]+0x34u,r[0],4); /* mov.l r0,@(0x34,r14) */
 goto L8c660a08;
L8c660a08: r[3]=sx(m,0x8c660acau,2); /* mov.w 0x8c660aca,r3 */
 goto L8c660a0a;
L8c660a0a: wr(m,r[14]+0x38u,r[2],4); /* mov.l r2,@(0x38,r14) */
 goto L8c660a0c;
L8c660a0c: r[0]=sx(m,r[12]+0xeu,1); /* mov.b @(0xe,r12),r0 */
 goto L8c660a0e;
L8c660a0e: r[2]=rd(m,0x8c660ad8u,4); /* mov.l 0x8c660ad8,r2 */
 goto L8c660a10;
L8c660a10: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c660a12;
L8c660a12: wr(m,r[14]+0x24u,r[0],4); /* mov.l r0,@(0x24,r14) */
 goto L8c660a14;
L8c660a14: r[0]=sx(m,r[12]+0xfu,1); /* mov.b @(0xf,r12),r0 */
 goto L8c660a16;
L8c660a16: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c660a18;
L8c660a18: wr(m,r[14]+0x28u,r[0],4); /* mov.l r0,@(0x28,r14) */
 goto L8c660a1a;
L8c660a1a: wr(m,r[14]+0x2cu,r[3],4); /* mov.l r3,@(0x2c,r14) */
 goto L8c660a1c;
L8c660a1c: target=r[2];*pr=0x8c660a20u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c660a20;
L8c660a20: r[3]=rd(m,0x8c660adcu,4); /* mov.l 0x8c660adc,r3 */
 goto L8c660a22;
L8c660a22: r[4]=r[15]; /* mov r15,r4 */
 goto L8c660a24;
L8c660a24: r[5]=r[0]; /* mov r0,r5 */
 goto L8c660a26;
L8c660a26: target=r[3];*pr=0x8c660a2au;r[4]+=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c660a2a;
L8c660a2a: *sr=(*sr&~1u)|((int32_t)r[0]>=0); /* cmp/pz r0 */
 goto L8c660a2c;
L8c660a2c: if(!(*sr&1u))goto L8c660a64; /* bf 0x8c660a64 */
 goto L8c660a2e;
L8c660a2e: r[0]=sx(m,r[15]+0xau,2); /* mov.w @(0xa,r15),r0 */
 goto L8c660a30;
L8c660a30: r[1]=64u; /* mov #0x40,r1 */
 goto L8c660a32;
L8c660a32: r[1]+=r[14]; /* add r14,r1 */
 goto L8c660a34;
L8c660a34: r[3]=rd(m,0x8c660ae0u,4); /* mov.l 0x8c660ae0,r3 */
 goto L8c660a36;
L8c660a36: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c660a38;
L8c660a38: r[4]=sx(m,0x8c660accu,2); /* mov.w 0x8c660acc,r4 */
 goto L8c660a3a;
L8c660a3a: wr(m,r[14]+0x3cu,r[0],4); /* mov.l r0,@(0x3c,r14) */
 goto L8c660a3c;
L8c660a3c: r[0]=sx(m,r[15]+0x8u,2); /* mov.w @(0x8,r15),r0 */
 goto L8c660a3e;
L8c660a3e: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c660a40;
L8c660a40: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c660a42;
L8c660a42: r[0]=sx(m,r[15]+0xau,2); /* mov.w @(0xa,r15),r0 */
 goto L8c660a44;
L8c660a44: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c660a46;
L8c660a46: target=r[3];*pr=0x8c660a4au;r[1]=r[4];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c660a4a;
L8c660a4a: r[1]=68u; /* mov #0x44,r1 */
 goto L8c660a4c;
L8c660a4c: r[3]=rd(m,0x8c660ae0u,4); /* mov.l 0x8c660ae0,r3 */
 goto L8c660a4e;
L8c660a4e: r[1]+=r[14]; /* add r14,r1 */
 goto L8c660a50;
L8c660a50: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c660a52;
L8c660a52: r[0]=sx(m,r[15]+0x8u,2); /* mov.w @(0x8,r15),r0 */
 goto L8c660a54;
L8c660a54: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c660a56;
L8c660a56: target=r[3];*pr=0x8c660a5au;r[1]=r[4];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c660a5a;
L8c660a5a: r[1]=72u; /* mov #0x48,r1 */
 goto L8c660a5c;
L8c660a5c: r[1]+=r[14]; /* add r14,r1 */
 goto L8c660a5e;
L8c660a5e: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c660a60;
L8c660a60: r[0]=0u;goto L8c660a6a; /* bra 0x8c660a6a */
 goto L8c660a64;
L8c660a64: target=r[11];*pr=0x8c660a68u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c660a68;
L8c660a68: r[0]=sx(m,0x8c660aceu,2); /* mov.w 0x8c660ace,r0 */
 goto L8c660a6a;
L8c660a6a: r[15]+=16u; /* add #0x10,r15 */
 goto L8c660a6c;
L8c660a6c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c660a6e;
L8c660a6e: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c660a70;
L8c660a70: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c660a72;
L8c660a72: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c660a74;
L8c660a74: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c660a76;
L8c660a76: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c660a78;
L8c660a78: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c660a7a;
L8c660a7a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_input_backend_request(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){return sa_input_backend_request_core(0,m,f,sr,r,pr,fpul,macl,ctx,call);}
SA_STAGE_EXPORT int sa_input_backend_request_resume(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){return sa_input_backend_request_core(*pr,m,f,sr,r,pr,fpul,macl,ctx,call);}
