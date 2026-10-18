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
SA_STAGE_EXPORT int sa_sound_work_dispatch(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c666a68;
L8c666a68: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c666a6a;
L8c666a6a: r[14]=rd(m,0x8c666b20u,4); /* mov.l 0x8c666b20,r14 */
 goto L8c666a6c;
L8c666a6c: r[0]=sx(m,0x8c666b14u,2); /* mov.w 0x8c666b14,r0 */
 goto L8c666a6e;
L8c666a6e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c666a70;
L8c666a70: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c666a72;
L8c666a72: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c666a74;
L8c666a74: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c666a76;
L8c666a76: if((*sr&1u))goto L8c666ac8; /* bt 0x8c666ac8 */
 goto L8c666a78;
L8c666a78: r[0]=sx(m,0x8c666b16u,2); /* mov.w 0x8c666b16,r0 */
 goto L8c666a7a;
L8c666a7a: r[5]=1u; /* mov #0x1,r5 */
 goto L8c666a7c;
L8c666a7c: wr(m,r[4]+r[0],r[5],4); /* mov.l r5,@(r0,r4) */
 goto L8c666a7e;
L8c666a7e: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c666a80;
L8c666a80: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c666a82;
L8c666a82: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c666a84;
L8c666a84: *sr=(*sr&~1u)|!(r[2]&r[5]); /* tst r2,r5 */
 goto L8c666a86;
L8c666a86: if((*sr&1u))goto L8c666a92; /* bt 0x8c666a92 */
 goto L8c666a88;
L8c666a88: r[0]=sx(m,0x8c666b14u,2); /* mov.w 0x8c666b14,r0 */
 goto L8c666a8a;
L8c666a8a: r[1]=rd(m,r[14],4); /* mov.l @r14,r1 */
 goto L8c666a8c;
L8c666a8c: r[3]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r3 */
 goto L8c666a8e;
L8c666a8e: r[3]+=1u; /* add #0x1,r3 */
 goto L8c666a90;
L8c666a90: wr(m,r[1]+r[0],r[3],4); /* mov.l r3,@(r0,r1) */
 goto L8c666a92;
L8c666a92: r[0]=sx(m,0x8c666b14u,2); /* mov.w 0x8c666b14,r0 */
 goto L8c666a94;
L8c666a94: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c666a96;
L8c666a96: r[3]=sx(m,0x8c666b18u,2); /* mov.w 0x8c666b18,r3 */
 goto L8c666a98;
L8c666a98: r[7]=rd(m,0x8c666b24u,4); /* mov.l 0x8c666b24,r7 */
 goto L8c666a9a;
L8c666a9a: r[3]+=r[2]; /* add r2,r3 */
 goto L8c666a9c;
L8c666a9c: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c666a9e;
L8c666a9e: r[6]=rd(m,r[14],4); /* mov.l @r14,r6 */
 goto L8c666aa0;
L8c666aa0: r[5]=rd(m,r[14],4); /* mov.l @r14,r5 */
 goto L8c666aa2;
L8c666aa2: r[6]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r6 */
 goto L8c666aa4;
L8c666aa4: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c666aa6;
L8c666aa6: r[5]=rd(m,r[5]+r[0],4); /* mov.l @(r0,r5),r5 */
 goto L8c666aa8;
L8c666aa8: r[0]+=32u; /* add #0x20,r0 */
 goto L8c666aaa;
L8c666aaa: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c666aac;
L8c666aac: r[6]<<=2; /* shll2 r6 */
 goto L8c666aae;
L8c666aae: r[3]=rd(m,0x8c666b28u,4); /* mov.l 0x8c666b28,r3 */
 goto L8c666ab0;
L8c666ab0: r[6]<<=2; /* shll2 r6 */
 goto L8c666ab2;
L8c666ab2: target=r[3];*pr=0x8c666ab6u;r[4]=rd(m,r[4]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666ab6;
L8c666ab6: r[15]+=4u; /* add #0x4,r15 */
 goto L8c666ab8;
L8c666ab8: r[0]=sx(m,0x8c666b1au,2); /* mov.w 0x8c666b1a,r0 */
 goto L8c666aba;
L8c666aba: r[5]=rd(m,r[14],4); /* mov.l @r14,r5 */
 goto L8c666abc;
L8c666abc: r[3]=rd(m,0x8c666b2cu,4); /* mov.l 0x8c666b2c,r3 */
 goto L8c666abe;
L8c666abe: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c666ac0;
L8c666ac0: r[5]=rd(m,r[5]+r[0],4); /* mov.l @(r0,r5),r5 */
 goto L8c666ac2;
L8c666ac2: r[0]+=4294967220u; /* add #-0x4c,r0 */
 goto L8c666ac4;
L8c666ac4: target=r[3];*pr=0x8c666ac8u;r[4]=rd(m,r[4]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666ac8;
L8c666ac8: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c666aca;
L8c666aca: r[0]=0u; /* mov #0x0,r0 */
 goto L8c666acc;
L8c666acc: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
