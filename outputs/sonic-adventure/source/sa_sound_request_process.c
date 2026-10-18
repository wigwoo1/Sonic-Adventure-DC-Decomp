/* Sonic Adventure USA Rev A sound-driver request/work processing. */
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
/* Recovered retail sound-driver routine. External calls use recovered native helpers through the call bridge. */
SA_STAGE_EXPORT int sa_sound_request_process(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a378;
L8c65a378: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a37a;
L8c65a37a: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65a37c;
L8c65a37c: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65a37e;
L8c65a37e: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65a380;
L8c65a380: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65a382;
L8c65a382: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c65a384;
L8c65a384: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c65a386;
L8c65a386: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65a388;
L8c65a388: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c65a38a;
L8c65a38a: r[3]=rd(m,0x8c65a444u,4); /* mov.l 0x8c65a444,r3 */
 goto L8c65a38c;
L8c65a38c: r[14]=rd(m,r[3],4); /* mov.l @r3,r14 */
 goto L8c65a38e;
L8c65a38e: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65a390;
L8c65a390: if(!(*sr&1u))goto L8c65a398; /* bf 0x8c65a398 */
 goto L8c65a392;
L8c65a392: r[14]=rd(m,0x8c65a448u,4); /* mov.l 0x8c65a448,r14 */
 goto L8c65a394;
L8c65a394: goto L8c65a418; /* bra 0x8c65a418 */
 goto L8c65a398;
L8c65a398: r[13]=sx(m,0x8c65a43au,2); /* mov.w 0x8c65a43a,r13 */
 goto L8c65a39a;
L8c65a39a: r[13]+=r[14]; /* add r14,r13 */
 goto L8c65a39c;
L8c65a39c: r[0]=rd(m,r[13]+0x8u,4); /* mov.l @(0x8,r13),r0 */
 goto L8c65a39e;
L8c65a39e: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65a3a0;
L8c65a3a0: if((*sr&1u))goto L8c65a3b2; /* bt 0x8c65a3b2 */
 goto L8c65a3a2;
L8c65a3a2: r[3]=rd(m,0x8c65a44cu,4); /* mov.l 0x8c65a44c,r3 */
 goto L8c65a3a4;
L8c65a3a4: r[5]=r[15]; /* mov r15,r5 */
 goto L8c65a3a6;
L8c65a3a6: r[0]=sx(m,0x8c65a43cu,2); /* mov.w 0x8c65a43c,r0 */
 goto L8c65a3a8;
L8c65a3a8: target=r[3];*pr=0x8c65a3acu;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65a3ac;
L8c65a3ac: r[2]=sx(m,r[15],1); /* mov.b @r15,r2 */
 goto L8c65a3ae;
L8c65a3ae: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c65a3b0;
L8c65a3b0: if((*sr&1u))goto L8c65a3b8; /* bt 0x8c65a3b8 */
 goto L8c65a3b2;
L8c65a3b2: r[14]=rd(m,0x8c65a450u,4); /* mov.l 0x8c65a450,r14 */
 goto L8c65a3b4;
L8c65a3b4: goto L8c65a418; /* bra 0x8c65a418 */
 goto L8c65a3b8;
L8c65a3b8: r[3]=rd(m,0x8c65a444u,4); /* mov.l 0x8c65a444,r3 */
 goto L8c65a3ba;
L8c65a3ba: r[12]=0u; /* mov #0x0,r12 */
 goto L8c65a3bc;
L8c65a3bc: r[0]=sx(m,0x8c65a43eu,2); /* mov.w 0x8c65a43e,r0 */
 goto L8c65a3be;
L8c65a3be: r[10]=r[12]; /* mov r12,r10 */
 goto L8c65a3c0;
L8c65a3c0: r[11]=rd(m,r[3],4); /* mov.l @r3,r11 */
 goto L8c65a3c2;
L8c65a3c2: r[11]=rd(m,r[11]+r[0],4); /* mov.l @(r0,r11),r11 */
 goto L8c65a3c4;
L8c65a3c4: *sr=(*sr&~1u)|((int32_t)r[11]>0); /* cmp/pl r11 */
 goto L8c65a3c6;
L8c65a3c6: take=!(*sr&1u);r[9]=r[12];if(take)goto L8c65a40e;goto L8c65a3ca; /* bf/s 0x8c65a40e */
 goto L8c65a3ca;
L8c65a3ca: r[8]=28u; /* mov #0x1c,r8 */
 goto L8c65a3cc;
L8c65a3cc: r[4]=rd(m,r[13]+0x1cu,4); /* mov.l @(0x1c,r13),r4 */
 goto L8c65a3ce;
L8c65a3ce: *macl=r[8]*r[4]; /* mul.l r8,r4 */
 goto L8c65a3d0;
L8c65a3d0: r[4]+=1u; /* add #0x1,r4 */
 goto L8c65a3d2;
L8c65a3d2: wr(m,r[13]+0x1cu,r[4],4); /* mov.l r4,@(0x1c,r13) */
 goto L8c65a3d4;
L8c65a3d4: r[2]=rd(m,r[13]+0x1cu,4); /* mov.l @(0x1c,r13),r2 */
 goto L8c65a3d6;
L8c65a3d6: r[4]=*macl; /* sts MACL,r4 */
 goto L8c65a3d8;
L8c65a3d8: r[3]=rd(m,r[13]+0xcu,4); /* mov.l @(0xc,r13),r3 */
 goto L8c65a3da;
L8c65a3da: *sr=(*sr&~1u)|(r[11]==r[2]); /* cmp/eq r11,r2 */
 goto L8c65a3dc;
L8c65a3dc: r[4]+=r[3]; /* add r3,r4 */
 goto L8c65a3de;
L8c65a3de: if(!(*sr&1u))goto L8c65a3e2; /* bf 0x8c65a3e2 */
 goto L8c65a3e0;
L8c65a3e0: wr(m,r[13]+0x1cu,r[12],4); /* mov.l r12,@(0x1c,r13) */
 goto L8c65a3e2;
L8c65a3e2: r[0]=rd(m,r[4]+0x14u,4); /* mov.l @(0x14,r4),r0 */
 goto L8c65a3e4;
L8c65a3e4: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65a3e6;
L8c65a3e6: if(!(*sr&1u))goto L8c65a408; /* bf 0x8c65a408 */
 goto L8c65a3e8;
L8c65a3e8: r[3]=rd(m,0x8c65a454u,4); /* mov.l 0x8c65a454,r3 */
 goto L8c65a3ea;
L8c65a3ea: target=r[3];*pr=0x8c65a3eeu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65a3ee;
L8c65a3ee: r[14]=r[0]; /* mov r0,r14 */
 goto L8c65a3f0;
L8c65a3f0: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65a3f2;
L8c65a3f2: if(!(*sr&1u))goto L8c65a418; /* bf 0x8c65a418 */
 goto L8c65a3f4;
L8c65a3f4: r[10]+=1u; /* add #0x1,r10 */
 goto L8c65a3f6;
L8c65a3f6: r[0]=r[10]; /* mov r10,r0 */
 goto L8c65a3f8;
L8c65a3f8: *sr=(*sr&~1u)|(r[0]==0x20u); /* cmp/eq #0x20,r0 */
 goto L8c65a3fa;
L8c65a3fa: if((*sr&1u))goto L8c65a40e; /* bt 0x8c65a40e */
 goto L8c65a3fc;
L8c65a3fc: r[2]=rd(m,0x8c65a444u,4); /* mov.l 0x8c65a444,r2 */
 goto L8c65a3fe;
L8c65a3fe: r[0]=sx(m,0x8c65a440u,2); /* mov.w 0x8c65a440,r0 */
 goto L8c65a400;
L8c65a400: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c65a402;
L8c65a402: r[1]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r1 */
 goto L8c65a404;
L8c65a404: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c65a406;
L8c65a406: if((*sr&1u))goto L8c65a40e; /* bt 0x8c65a40e */
 goto L8c65a408;
L8c65a408: r[9]+=1u; /* add #0x1,r9 */
 goto L8c65a40a;
L8c65a40a: *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[11]); /* cmp/ge r11,r9 */
 goto L8c65a40c;
L8c65a40c: if(!(*sr&1u))goto L8c65a3cc; /* bf 0x8c65a3cc */
 goto L8c65a40e;
L8c65a40e: r[2]=rd(m,0x8c65a458u,4); /* mov.l 0x8c65a458,r2 */
 goto L8c65a410;
L8c65a410: target=r[2];*pr=0x8c65a414u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65a414;
L8c65a414: r[14]=r[0]; /* mov r0,r14 */
 goto L8c65a416;
L8c65a416: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65a418;
L8c65a418: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65a41a;
L8c65a41a: if((*sr&1u))goto L8c65a424; /* bt 0x8c65a424 */
 goto L8c65a41c;
L8c65a41c: r[2]=rd(m,0x8c65a460u,4); /* mov.l 0x8c65a460,r2 */
 goto L8c65a41e;
L8c65a41e: r[4]=rd(m,0x8c65a45cu,4); /* mov.l 0x8c65a45c,r4 */
 goto L8c65a420;
L8c65a420: target=r[2];*pr=0x8c65a424u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65a424;
L8c65a424: r[0]=r[14]; /* mov r14,r0 */
 goto L8c65a426;
L8c65a426: r[15]+=4u; /* add #0x4,r15 */
 goto L8c65a428;
L8c65a428: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65a42a;
L8c65a42a: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c65a42c;
L8c65a42c: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c65a42e;
L8c65a42e: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65a430;
L8c65a430: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65a432;
L8c65a432: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65a434;
L8c65a434: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65a436;
L8c65a436: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
