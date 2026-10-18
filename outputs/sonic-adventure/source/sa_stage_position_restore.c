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
/* Static C recovery of retail Tikal helper 8c04f8fa..8c04fb3a.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_stage_position_restore(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c04f8fa;
L8c04f8fa: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c04f8fc;
L8c04f8fc: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c04f8fe;
L8c04f8fe: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c04f900;
L8c04f900: r[3]=rd(m,0x8c04f958u,4); /* mov.l 0x8c04f958,r3 */
 goto L8c04f902;
L8c04f902: target=r[3];*pr=0x8c04f906u;r[14]=r[4];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c04f906;
L8c04f906: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c04f908;
L8c04f908: if((*sr&1u))goto L8c04f960; /* bt 0x8c04f960 */
 goto L8c04f90a;
L8c04f90a: r[3]=rd(m,0x8c04f95cu,4); /* mov.l 0x8c04f95c,r3 */
 goto L8c04f90c;
L8c04f90c: r[5]=r[14]; /* mov r14,r5 */
 goto L8c04f90e;
L8c04f90e: r[4]=r[14]; /* mov r14,r4 */
 goto L8c04f910;
L8c04f910: r[5]+=20u; /* add #0x14,r5 */
 goto L8c04f912;
L8c04f912: target=r[3];*pr=0x8c04f916u;r[4]+=32u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c04f916;
L8c04f916: goto L8c04fad4; /* bra 0x8c04fad4 */
 goto L8c04f960;
L8c04f960: r[0]=rd(m,0x8c04fa44u,4); /* mov.l 0x8c04fa44,r0 */
 goto L8c04f962;
L8c04f962: r[2]=rd(m,0x8c04fa40u,4); /* mov.l 0x8c04fa40,r2 */
 goto L8c04f964;
L8c04f964: r[1]=sx(m,r[0],2); /* mov.w @r0,r1 */
 goto L8c04f966;
L8c04f966: r[3]=sx(m,r[2],2); /* mov.w @r2,r3 */
 goto L8c04f968;
L8c04f968: *sr=(*sr&~1u)|(r[1]==r[3]); /* cmp/eq r1,r3 */
 goto L8c04f96a;
L8c04f96a: if(!(*sr&1u))goto L8c04f972; /* bf 0x8c04f972 */
 goto L8c04f96c;
L8c04f96c: r[1]=rd(m,0x8c04fa48u,4); /* mov.l 0x8c04fa48,r1 */
 goto L8c04f96e;
L8c04f96e: target=r[1];*pr=0x8c04f972u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c04f972;
L8c04f972: r[3]=rd(m,0x8c04fa4cu,4); /* mov.l 0x8c04fa4c,r3 */
 goto L8c04f974;
L8c04f974: r[4]=sx(m,r[3],2); /* mov.w @r3,r4 */
 goto L8c04f976;
L8c04f976: r[0]=r[4]&65535u; /* extu.w r4,r0 */
 goto L8c04f978;
L8c04f978: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c04f97a;
L8c04f97a: take=!(*sr&1u);r[4]=r[0];if(take)goto L8c04f986;goto L8c04f97e; /* bf/s 0x8c04f986 */
 goto L8c04f97e;
L8c04f97e: r[2]=rd(m,0x8c04fa50u,4); /* mov.l 0x8c04fa50,r2 */
 goto L8c04f980;
L8c04f980: r[5]=0u; /* mov #0x0,r5 */
 goto L8c04f982;
L8c04f982: target=r[2];*pr=0x8c04f986u;r[4]=3u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c04f986;
L8c04f986: r[5]=rd(m,0x8c04fa54u,4); /* mov.l 0x8c04fa54,r5 */
 goto L8c04f988;
L8c04f988: r[4]=rd(m,r[5],4); /* mov.l @r5,r4 */
 goto L8c04f98a;
L8c04f98a: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c04f98c;
L8c04f98c: take=(*sr&1u);r[6]=0u;if(take)goto L8c04f9a6;goto L8c04f990; /* bt/s 0x8c04f9a6 */
 goto L8c04f990;
L8c04f990: r[2]=rd(m,r[4]+0x10u,4); /* mov.l @(0x10,r4),r2 */
 goto L8c04f992;
L8c04f992: wr(m,r[14]+0x18u,r[2],4); /* mov.l r2,@(0x18,r14) */
 goto L8c04f994;
L8c04f994: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c04f996;
L8c04f996: r[2]=rd(m,r[3]+0x4u,4); /* mov.l @(0x4,r3),r2 */
 goto L8c04f998;
L8c04f998: r[1]=rd(m,r[3]+0x8u,4); /* mov.l @(0x8,r3),r1 */
 goto L8c04f99a;
L8c04f99a: wr(m,r[14]+0x20u,r[2],4); /* mov.l r2,@(0x20,r14) */
 goto L8c04f99c;
L8c04f99c: wr(m,r[14]+0x24u,r[1],4); /* mov.l r1,@(0x24,r14) */
 goto L8c04f99e;
L8c04f99e: r[2]=rd(m,r[3]+0xcu,4); /* mov.l @(0xc,r3),r2 */
 goto L8c04f9a0;
L8c04f9a0: wr(m,r[14]+0x28u,r[2],4); /* mov.l r2,@(0x28,r14) */
 goto L8c04f9a2;
L8c04f9a2: wr(m,r[5],r[6],4);goto L8c04fad4; /* bra 0x8c04fad4 */
 goto L8c04f9a6;
L8c04f9a6: r[5]=rd(m,0x8c04fa58u,4); /* mov.l 0x8c04fa58,r5 */
 goto L8c04f9a8;
L8c04f9a8: r[0]=sx(m,r[5],2); /* mov.w @r5,r0 */
 goto L8c04f9aa;
L8c04f9aa: *sr=(*sr&~1u)|(r[0]==0x1au); /* cmp/eq #0x1a,r0 */
 goto L8c04f9ac;
L8c04f9ac: take=(*sr&1u);r[4]=r[0];if(take)goto L8c04f9bc;goto L8c04f9b0; /* bt/s 0x8c04f9bc */
 goto L8c04f9b0;
L8c04f9b0: r[0]=r[4]; /* mov r4,r0 */
 goto L8c04f9b2;
L8c04f9b2: *sr=(*sr&~1u)|(r[0]==0x1bu); /* cmp/eq #0x1b,r0 */
 goto L8c04f9b4;
L8c04f9b4: if((*sr&1u))goto L8c04f9bc; /* bt 0x8c04f9bc */
 goto L8c04f9b6;
L8c04f9b6: r[0]=r[4]; /* mov r4,r0 */
 goto L8c04f9b8;
L8c04f9b8: *sr=(*sr&~1u)|(r[0]==0x1cu); /* cmp/eq #0x1c,r0 */
 goto L8c04f9ba;
L8c04f9ba: if(!(*sr&1u))goto L8c04f9c6; /* bf 0x8c04f9c6 */
 goto L8c04f9bc;
L8c04f9bc: r[3]=rd(m,0x8c04fa5cu,4); /* mov.l 0x8c04fa5c,r3 */
 goto L8c04f9be;
L8c04f9be: target=r[3];*pr=0x8c04f9c2u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c04f9c2;
L8c04f9c2: goto L8c04fad4; /* bra 0x8c04fad4 */
 goto L8c04f9c6;
L8c04f9c6: r[0]=sx(m,r[5],2); /* mov.w @r5,r0 */
 goto L8c04f9c8;
L8c04f9c8: *sr=(*sr&~1u)|(r[0]==0x1du); /* cmp/eq #0x1d,r0 */
 goto L8c04f9ca;
L8c04f9ca: take=(*sr&1u);r[4]=r[0];if(take)goto L8c04f9d4;goto L8c04f9ce; /* bt/s 0x8c04f9d4 */
 goto L8c04f9ce;
L8c04f9ce: r[0]=r[4]; /* mov r4,r0 */
 goto L8c04f9d0;
L8c04f9d0: *sr=(*sr&~1u)|(r[0]==0x20u); /* cmp/eq #0x20,r0 */
 goto L8c04f9d2;
L8c04f9d2: if(!(*sr&1u))goto L8c04f9de; /* bf 0x8c04f9de */
 goto L8c04f9d4;
L8c04f9d4: r[3]=rd(m,0x8c04fa60u,4); /* mov.l 0x8c04fa60,r3 */
 goto L8c04f9d6;
L8c04f9d6: target=r[3];*pr=0x8c04f9dau;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c04f9da;
L8c04f9da: goto L8c04fad4; /* bra 0x8c04fad4 */
 goto L8c04f9de;
L8c04f9de: r[0]=sx(m,r[5],2); /* mov.w @r5,r0 */
 goto L8c04f9e0;
L8c04f9e0: *sr=(*sr&~1u)|(r[0]==0x21u); /* cmp/eq #0x21,r0 */
 goto L8c04f9e2;
L8c04f9e2: if(!(*sr&1u))goto L8c04f9ee; /* bf 0x8c04f9ee */
 goto L8c04f9e4;
L8c04f9e4: r[2]=rd(m,0x8c04fa64u,4); /* mov.l 0x8c04fa64,r2 */
 goto L8c04f9e6;
L8c04f9e6: target=r[2];*pr=0x8c04f9eau;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c04f9ea;
L8c04f9ea: goto L8c04fad4; /* bra 0x8c04fad4 */
 goto L8c04f9ee;
L8c04f9ee: *sr=(*sr&~1u)|(r[0]==0x22u); /* cmp/eq #0x22,r0 */
 goto L8c04f9f0;
L8c04f9f0: if(!(*sr&1u))goto L8c04f9fc; /* bf 0x8c04f9fc */
 goto L8c04f9f2;
L8c04f9f2: r[2]=rd(m,0x8c04fa68u,4); /* mov.l 0x8c04fa68,r2 */
 goto L8c04f9f4;
L8c04f9f4: target=r[2];*pr=0x8c04f9f8u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c04f9f8;
L8c04f9f8: goto L8c04fad4; /* bra 0x8c04fad4 */
 goto L8c04f9fc;
L8c04f9fc: r[0]=sx(m,r[14]+0x9u,1); /* mov.b @(0x9,r14),r0 */
 goto L8c04f9fe;
L8c04f9fe: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c04fa00;
L8c04fa00: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c04fa02;
L8c04fa02: if((*sr&1u))goto L8c04fa1c; /* bt 0x8c04fa1c */
 goto L8c04fa04;
L8c04fa04: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c04fa06;
L8c04fa06: if((*sr&1u))goto L8c04fa22; /* bt 0x8c04fa22 */
 goto L8c04fa08;
L8c04fa08: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c04fa0a;
L8c04fa0a: if((*sr&1u))goto L8c04fa80; /* bt 0x8c04fa80 */
 goto L8c04fa0c;
L8c04fa0c: *sr=(*sr&~1u)|(r[0]==0x7u); /* cmp/eq #0x7,r0 */
 goto L8c04fa0e;
L8c04fa0e: if((*sr&1u))goto L8c04fa86; /* bt 0x8c04fa86 */
 goto L8c04fa10;
L8c04fa10: *sr=(*sr&~1u)|(r[0]==0x5u); /* cmp/eq #0x5,r0 */
 goto L8c04fa12;
L8c04fa12: if((*sr&1u))goto L8c04fa8c; /* bt 0x8c04fa8c */
 goto L8c04fa14;
L8c04fa14: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c04fa16;
L8c04fa16: if((*sr&1u))goto L8c04fa92; /* bt 0x8c04fa92 */
 goto L8c04fa18;
L8c04fa18: goto L8c04fa94; /* bra 0x8c04fa94 */
 goto L8c04fa1c;
L8c04fa1c: r[4]=rd(m,0x8c04fa6cu,4); /* mov.l 0x8c04fa6c,r4 */
 goto L8c04fa1e;
L8c04fa1e: goto L8c04fa94; /* bra 0x8c04fa94 */
 goto L8c04fa22;
L8c04fa22: r[4]=rd(m,0x8c04fa70u,4); /* mov.l 0x8c04fa70,r4 */
 goto L8c04fa24;
L8c04fa24: goto L8c04fa94; /* bra 0x8c04fa94 */
 goto L8c04fa80;
L8c04fa80: r[4]=rd(m,0x8c04fb80u,4); /* mov.l 0x8c04fb80,r4 */
 goto L8c04fa82;
L8c04fa82: goto L8c04fa94; /* bra 0x8c04fa94 */
 goto L8c04fa86;
L8c04fa86: r[4]=rd(m,0x8c04fb84u,4); /* mov.l 0x8c04fb84,r4 */
 goto L8c04fa88;
L8c04fa88: goto L8c04fa94; /* bra 0x8c04fa94 */
 goto L8c04fa8c;
L8c04fa8c: r[4]=rd(m,0x8c04fb88u,4); /* mov.l 0x8c04fb88,r4 */
 goto L8c04fa8e;
L8c04fa8e: goto L8c04fa94; /* bra 0x8c04fa94 */
 goto L8c04fa92;
L8c04fa92: r[4]=rd(m,0x8c04fb8cu,4); /* mov.l 0x8c04fb8c,r4 */
 goto L8c04fa94;
L8c04fa94: fr[4]=0; /* fldi0 fr4 */
 goto L8c04fa96;
L8c04fa96: r[2]=sx(m,r[5],2); /* mov.w @r5,r2 */
 goto L8c04fa98;
L8c04fa98: r[3]=sx(m,r[4],2); /* mov.w @r4,r3 */
 goto L8c04fa9a;
L8c04fa9a: *sr=(*sr&~1u)|(r[2]==r[3]); /* cmp/eq r2,r3 */
 goto L8c04fa9c;
L8c04fa9c: if(!(*sr&1u))goto L8c04faba; /* bf 0x8c04faba */
 goto L8c04fa9e;
L8c04fa9e: r[2]=rd(m,0x8c04fb90u,4); /* mov.l 0x8c04fb90,r2 */
 goto L8c04faa0;
L8c04faa0: r[0]=sx(m,r[4]+0x2u,2); /* mov.w @(0x2,r4),r0 */
 goto L8c04faa2;
L8c04faa2: r[3]=sx(m,r[2],2); /* mov.w @r2,r3 */
 goto L8c04faa4;
L8c04faa4: *sr=(*sr&~1u)|(r[3]==r[0]); /* cmp/eq r3,r0 */
 goto L8c04faa6;
L8c04faa6: if(!(*sr&1u))goto L8c04faba; /* bf 0x8c04faba */
 goto L8c04faa8;
L8c04faa8: r[2]=rd(m,r[4]+0x10u,4); /* mov.l @(0x10,r4),r2 */
 goto L8c04faaa;
L8c04faaa: wr(m,r[14]+0x18u,r[2],4); /* mov.l r2,@(0x18,r14) */
 goto L8c04faac;
L8c04faac: r[3]=rd(m,r[4]+0x4u,4); /* mov.l @(0x4,r4),r3 */
 goto L8c04faae;
L8c04faae: r[1]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r1 */
 goto L8c04fab0;
L8c04fab0: wr(m,r[14]+0x20u,r[3],4); /* mov.l r3,@(0x20,r14) */
 goto L8c04fab2;
L8c04fab2: wr(m,r[14]+0x24u,r[1],4); /* mov.l r1,@(0x24,r14) */
 goto L8c04fab4;
L8c04fab4: r[3]=rd(m,r[4]+0xcu,4); /* mov.l @(0xc,r4),r3 */
 goto L8c04fab6;
L8c04fab6: wr(m,r[14]+0x28u,r[3],4);goto L8c04fad4; /* bra 0x8c04fad4 */
 goto L8c04faba;
L8c04faba: r[0]=sx(m,r[4],2); /* mov.w @r4,r0 */
 goto L8c04fabc;
L8c04fabc: *sr=(*sr&~1u)|(r[0]==0x2bu); /* cmp/eq #0x2b,r0 */
 goto L8c04fabe;
L8c04fabe: if(!(*sr&1u))goto L8c04fad0; /* bf 0x8c04fad0 */
 goto L8c04fac0;
L8c04fac0: r[0]=32u; /* mov #0x20,r0 */
 goto L8c04fac2;
L8c04fac2: wr(m,r[14]+0x18u,r[6],4); /* mov.l r6,@(0x18,r14) */
 goto L8c04fac4;
L8c04fac4: wr(m,r[0]+r[14],fr[4],4); /* fmov fr4,@(r0,r14) */
 goto L8c04fac6;
L8c04fac6: r[0]=36u; /* mov #0x24,r0 */
 goto L8c04fac8;
L8c04fac8: wr(m,r[0]+r[14],fr[4],4); /* fmov fr4,@(r0,r14) */
 goto L8c04faca;
L8c04faca: r[0]=40u; /* mov #0x28,r0 */
 goto L8c04facc;
L8c04facc: wr(m,r[0]+r[14],fr[4],4);goto L8c04fad4; /* bra 0x8c04fad4 */
 goto L8c04fad0;
L8c04fad0: r[4]+=20u;goto L8c04fa96; /* bra 0x8c04fa96 */
 goto L8c04fad4;
L8c04fad4: r[3]=rd(m,0x8c04fb94u,4); /* mov.l 0x8c04fb94,r3 */
 goto L8c04fad6;
L8c04fad6: r[7]=0u; /* mov #0x0,r7 */
 goto L8c04fad8;
L8c04fad8: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto L8c04fada;
L8c04fada: r[6]=r[14]; /* mov r14,r6 */
 goto L8c04fadc;
L8c04fadc: r[6]+=32u; /* add #0x20,r6 */
 goto L8c04fade;
L8c04fade: r[5]=r[7]; /* mov r7,r5 */
 goto L8c04fae0;
L8c04fae0: target=r[3];*pr=0x8c04fae4u;r[4]=r[0];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c04fae4;
L8c04fae4: r[2]=rd(m,0x8c04fb98u,4); /* mov.l 0x8c04fb98,r2 */
 goto L8c04fae6;
L8c04fae6: r[4]=sx(m,r[2],2); /* mov.w @r2,r4 */
 goto L8c04fae8;
L8c04fae8: r[0]=r[4]&65535u; /* extu.w r4,r0 */
 goto L8c04faea;
L8c04faea: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c04faec;
L8c04faec: take=!(*sr&1u);r[4]=r[0];if(take)goto L8c04fb16;goto L8c04faf0; /* bf/s 0x8c04fb16 */
 goto L8c04faf0;
L8c04faf0: r[3]=rd(m,0x8c04fb9cu,4); /* mov.l 0x8c04fb9c,r3 */
 goto L8c04faf2;
L8c04faf2: r[5]=r[15]; /* mov r15,r5 */
 goto L8c04faf4;
L8c04faf4: r[5]+=4u; /* add #0x4,r5 */
 goto L8c04faf6;
L8c04faf6: target=r[3];*pr=0x8c04fafau;r[4]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c04fafa;
L8c04fafa: r[4]=sx(m,r[15],1); /* mov.b @r15,r4 */
 goto L8c04fafc;
L8c04fafc: r[3]=60u; /* mov #0x3c,r3 */
 goto L8c04fafe;
L8c04fafe: r[0]=sx(m,r[15]+0x4u,1); /* mov.b @(0x4,r15),r0 */
 goto L8c04fb00;
L8c04fb00: r[4]=r[4]&255u; /* extu.b r4,r4 */
 goto L8c04fb02;
L8c04fb02: *macl=(r[3]&65535u)*(r[4]&65535u); /* mulu.w r3,r4 */
 goto L8c04fb04;
L8c04fb04: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c04fb06;
L8c04fb06: r[4]=*macl; /* sts MACL,r4 */
 goto L8c04fb08;
L8c04fb08: r[4]+=r[0]; /* add r0,r4 */
 goto L8c04fb0a;
L8c04fb0a: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]); /* cmp/ge r3,r4 */
 goto L8c04fb0c;
L8c04fb0c: if((*sr&1u))goto L8c04fb16; /* bt 0x8c04fb16 */
 goto L8c04fb0e;
L8c04fb0e: r[1]=rd(m,0x8c04fba0u,4); /* mov.l 0x8c04fba0,r1 */
 goto L8c04fb10;
L8c04fb10: r[5]=0u; /* mov #0x0,r5 */
 goto L8c04fb12;
L8c04fb12: target=r[1];*pr=0x8c04fb16u;r[4]=1u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c04fb16;
L8c04fb16: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto L8c04fb18;
L8c04fb18: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c04fb1a;
L8c04fb1a: if(!(*sr&1u))goto L8c04fb32; /* bf 0x8c04fb32 */
 goto L8c04fb1c;
L8c04fb1c: r[3]=rd(m,0x8c04fba4u,4); /* mov.l 0x8c04fba4,r3 */
 goto L8c04fb1e;
L8c04fb1e: r[1]=14u; /* mov #0xe,r1 */
 goto L8c04fb20;
L8c04fb20: r[2]=sx(m,r[3],2); /* mov.w @r3,r2 */
 goto L8c04fb22;
L8c04fb22: *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[1]); /* cmp/gt r1,r2 */
 goto L8c04fb24;
L8c04fb24: if((*sr&1u))goto L8c04fb32; /* bt 0x8c04fb32 */
 goto L8c04fb26;
L8c04fb26: r[3]=rd(m,0x8c04fba8u,4); /* mov.l 0x8c04fba8,r3 */
 goto L8c04fb28;
L8c04fb28: r[5]=r[14]; /* mov r14,r5 */
 goto L8c04fb2a;
L8c04fb2a: r[4]=r[14]; /* mov r14,r4 */
 goto L8c04fb2c;
L8c04fb2c: r[5]+=20u; /* add #0x14,r5 */
 goto L8c04fb2e;
L8c04fb2e: target=r[3];*pr=0x8c04fb32u;r[4]+=32u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c04fb32;
L8c04fb32: r[15]+=8u; /* add #0x8,r15 */
 goto L8c04fb34;
L8c04fb34: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c04fb36;
L8c04fb36: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
