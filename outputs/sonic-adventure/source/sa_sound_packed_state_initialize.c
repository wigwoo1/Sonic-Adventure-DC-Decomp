#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_cache_invalidate(void *,uint32_t);
int sa_sh4_cache_purge(void *,uint32_t);
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
/* Retail packed audio-state initializer, 8c66a440..8c66a524.
 * Native metadata66a378 and fill10d31c resolve through caller dispatch.
 * Initializes format2 and retains unknown encoding selectors. */
SA_STAGE_EXPORT int sa_sound_packed_state_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66a440;
L8c66a440: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66a442;
L8c66a442: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66a444;
L8c66a444: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66a446;
L8c66a446: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c66a448;
L8c66a448: r[3]=sx(m,0x8c66a496u,2); /* mov.w 0x8c66a496,r3 */
 goto L8c66a44a;
L8c66a44a: r[14]=r[4]; /* mov r4,r14 */
 goto L8c66a44c;
L8c66a44c: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c66a44e;
L8c66a44e: r[2]=r[14]; /* mov r14,r2 */
 goto L8c66a450;
L8c66a450: r[3]+=r[14]; /* add r14,r3 */
 goto L8c66a452;
L8c66a452: wr(m,r[15]+0x8u,r[6],4); /* mov.l r6,@(0x8,r15) */
 goto L8c66a454;
L8c66a454: r[2]+=16u; /* add #0x10,r2 */
 goto L8c66a456;
L8c66a456: r[7]=r[14]; /* mov r14,r7 */
 goto L8c66a458;
L8c66a458: r[0]=1u; /* mov #0x1,r0 */
 goto L8c66a45a;
L8c66a45a: r[7]+=12u; /* add #0xc,r7 */
 goto L8c66a45c;
L8c66a45c: wr(m,r[14]+0x2u,r[0],2); /* mov.w r0,@(0x2,r14) */
 goto L8c66a45e;
L8c66a45e: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c66a460;
L8c66a460: r[3]=r[14]; /* mov r14,r3 */
 goto L8c66a462;
L8c66a462: r[3]+=24u; /* add #0x18,r3 */
 goto L8c66a464;
L8c66a464: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c66a466;
L8c66a466: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c66a468;
L8c66a468: r[2]=r[14]; /* mov r14,r2 */
 goto L8c66a46a;
L8c66a46a: r[3]=r[14]; /* mov r14,r3 */
 goto L8c66a46c;
L8c66a46c: r[2]+=20u; /* add #0x14,r2 */
 goto L8c66a46e;
L8c66a46e: r[3]+=14u; /* add #0xe,r3 */
 goto L8c66a470;
L8c66a470: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c66a472;
L8c66a472: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c66a474;
L8c66a474: r[2]=r[14]; /* mov r14,r2 */
 goto L8c66a476;
L8c66a476: r[2]+=15u; /* add #0xf,r2 */
 goto L8c66a478;
L8c66a478: r[3]=r[14]; /* mov r14,r3 */
 goto L8c66a47a;
L8c66a47a: r[3]+=13u; /* add #0xd,r3 */
 goto L8c66a47c;
L8c66a47c: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c66a47e;
L8c66a47e: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c66a480;
L8c66a480: r[6]=r[15]; /* mov r15,r6 */
 goto L8c66a482;
L8c66a482: r[5]=rd(m,r[15]+0x24u,4); /* mov.l @(0x24,r15),r5 */
 goto L8c66a484;
L8c66a484: r[6]+=28u; /* add #0x1c,r6 */
 goto L8c66a486;
L8c66a486: *pr=0x8c66a48au;r[4]=rd(m,r[15]+0x20u,4);status=call(ctx,0x8c66a378u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c66a378 */
 goto L8c66a48a;
L8c66a48a: r[4]=r[0]; /* mov r0,r4 */
 goto L8c66a48c;
L8c66a48c: *sr=(*sr&~1u)|((int32_t)r[4]>=0); /* cmp/pz r4 */
 goto L8c66a48e;
L8c66a48e: take=(*sr&1u);r[15]+=28u;if(take)goto L8c66a498;goto L8c66a492; /* bt/s 0x8c66a498 */
 goto L8c66a492;
L8c66a492: r[0]=0u;goto L8c66a51a; /* bra 0x8c66a51a */
 goto L8c66a498;
L8c66a498: r[13]=0u; /* mov #0x0,r13 */
 goto L8c66a49a;
L8c66a49a: r[0]=r[13]; /* mov r13,r0 */
 goto L8c66a49c;
L8c66a49c: wr(m,r[14]+0x1cu,r[0],2); /* mov.w r0,@(0x1c,r14) */
 goto L8c66a49e;
L8c66a49e: r[0]=74u; /* mov #0x4a,r0 */
 goto L8c66a4a0;
L8c66a4a0: wr(m,r[0]+r[14],r[13],2); /* mov.w r13,@(r0,r14) */
 goto L8c66a4a2;
L8c66a4a2: r[0]=72u; /* mov #0x48,r0 */
 goto L8c66a4a4;
L8c66a4a4: wr(m,r[0]+r[14],r[13],2); /* mov.w r13,@(r0,r14) */
 goto L8c66a4a6;
L8c66a4a6: r[0]=38u; /* mov #0x26,r0 */
 goto L8c66a4a8;
L8c66a4a8: wr(m,r[0]+r[14],r[13],2); /* mov.w r13,@(r0,r14) */
 goto L8c66a4aa;
L8c66a4aa: r[0]=36u; /* mov #0x24,r0 */
 goto L8c66a4ac;
L8c66a4ac: wr(m,r[0]+r[14],r[13],2); /* mov.w r13,@(r0,r14) */
 goto L8c66a4ae;
L8c66a4ae: r[1]=88u; /* mov #0x58,r1 */
 goto L8c66a4b0;
L8c66a4b0: r[1]+=r[14]; /* add r14,r1 */
 goto L8c66a4b2;
L8c66a4b2: wr(m,r[14]+0x34u,r[13],4); /* mov.l r13,@(0x34,r14) */
 goto L8c66a4b4;
L8c66a4b4: wr(m,r[14]+0x30u,r[13],4); /* mov.l r13,@(0x30,r14) */
 goto L8c66a4b6;
L8c66a4b6: wr(m,r[14]+0x2cu,r[13],4); /* mov.l r13,@(0x2c,r14) */
 goto L8c66a4b8;
L8c66a4b8: wr(m,r[14]+0x28u,r[13],4); /* mov.l r13,@(0x28,r14) */
 goto L8c66a4ba;
L8c66a4ba: wr(m,r[14]+0x20u,r[13],4); /* mov.l r13,@(0x20,r14) */
 goto L8c66a4bc;
L8c66a4bc: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66a4be;
L8c66a4be: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c66a4c0;
L8c66a4c0: r[1]=92u; /* mov #0x5c,r1 */
 goto L8c66a4c2;
L8c66a4c2: r[0]=sx(m,r[14]+0xfu,1); /* mov.b @(0xf,r14),r0 */
 goto L8c66a4c4;
L8c66a4c4: r[1]+=r[14]; /* add r14,r1 */
 goto L8c66a4c6;
L8c66a4c6: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c66a4c8;
L8c66a4c8: r[0]=96u; /* mov #0x60,r0 */
 goto L8c66a4ca;
L8c66a4ca: r[3]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r3 */
 goto L8c66a4cc;
L8c66a4cc: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto L8c66a4ce;
L8c66a4ce: r[0]=100u; /* mov #0x64,r0 */
 goto L8c66a4d0;
L8c66a4d0: r[2]=rd(m,r[14]+0x3cu,4); /* mov.l @(0x3c,r14),r2 */
 goto L8c66a4d2;
L8c66a4d2: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c66a4d4;
L8c66a4d4: r[0]=64u; /* mov #0x40,r0 */
 goto L8c66a4d6;
L8c66a4d6: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c66a4d8;
L8c66a4d8: r[0]=104u; /* mov #0x68,r0 */
 goto L8c66a4da;
L8c66a4da: r[4]=r[14]; /* mov r14,r4 */
 goto L8c66a4dc;
L8c66a4dc: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto L8c66a4de;
L8c66a4de: r[0]=68u; /* mov #0x44,r0 */
 goto L8c66a4e0;
L8c66a4e0: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c66a4e2;
L8c66a4e2: r[0]=108u; /* mov #0x6c,r0 */
 goto L8c66a4e4;
L8c66a4e4: r[3]=r[14]; /* mov r14,r3 */
 goto L8c66a4e6;
L8c66a4e6: r[3]+=124u; /* add #0x7c,r3 */
 goto L8c66a4e8;
L8c66a4e8: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c66a4ea;
L8c66a4ea: r[0]=72u; /* mov #0x48,r0 */
 goto L8c66a4ec;
L8c66a4ec: r[2]=sx(m,r[0]+r[14],2); /* mov.w @(r0,r14),r2 */
 goto L8c66a4ee;
L8c66a4ee: r[0]=74u; /* mov #0x4a,r0 */
 goto L8c66a4f0;
L8c66a4f0: r[6]=4u; /* mov #0x4,r6 */
 goto L8c66a4f2;
L8c66a4f2: r[5]=r[13]; /* mov r13,r5 */
 goto L8c66a4f4;
L8c66a4f4: wr(m,r[3],r[2],2); /* mov.w r2,@r3 */
 goto L8c66a4f6;
L8c66a4f6: r[2]=r[14]; /* mov r14,r2 */
 goto L8c66a4f8;
L8c66a4f8: r[3]=sx(m,r[0]+r[14],2); /* mov.w @(r0,r14),r3 */
 goto L8c66a4fa;
L8c66a4fa: r[0]=126u; /* mov #0x7e,r0 */
 goto L8c66a4fc;
L8c66a4fc: r[2]+=76u; /* add #0x4c,r2 */
 goto L8c66a4fe;
L8c66a4fe: wr(m,r[0]+r[14],r[3],2); /* mov.w r3,@(r0,r14) */
 goto L8c66a500;
L8c66a500: r[0]+=2u; /* add #0x2,r0 */
 goto L8c66a502;
L8c66a502: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c66a504;
L8c66a504: r[0]+=28u; /* add #0x1c,r0 */
 goto L8c66a506;
L8c66a506: wr(m,r[14]+r[0],r[13],4); /* mov.l r13,@(r0,r14) */
 goto L8c66a508;
L8c66a508: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c66a50a;
L8c66a50a: wr(m,r[14]+r[0],r[13],4); /* mov.l r13,@(r0,r14) */
 goto L8c66a50c;
L8c66a50c: r[0]+=16u; /* add #0x10,r0 */
 goto L8c66a50e;
L8c66a50e: r[3]=2u; /* mov #0x2,r3 */
 goto L8c66a510;
L8c66a510: wr(m,r[0]+r[14],r[3],2); /* mov.w r3,@(r0,r14) */
 goto L8c66a512;
L8c66a512: r[3]=rd(m,0x8c66a5c0u,4); /* mov.l 0x8c66a5c0,r3 */
 goto L8c66a514;
L8c66a514: target=r[3];*pr=0x8c66a518u;r[4]+=76u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66a518;
L8c66a518: r[0]=sx(m,r[15],2); /* mov.w @r15,r0 */
 goto L8c66a51a;
L8c66a51a: r[15]+=12u; /* add #0xc,r15 */
 goto L8c66a51c;
L8c66a51c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66a51e;
L8c66a51e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66a520;
L8c66a520: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
