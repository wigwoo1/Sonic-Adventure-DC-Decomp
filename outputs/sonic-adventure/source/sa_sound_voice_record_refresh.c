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
/* USA Rev A sound-work record refresh094330. Provisional name; reuses lookup result or selects new slot, then updates record. */
SA_STAGE_EXPORT int sa_sound_voice_record_refresh(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c094330;
L8c094330: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c094332;
L8c094332: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c094334;
L8c094334: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c094336;
L8c094336: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c094338;
L8c094338: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c09433a;
L8c09433a: r[13]=r[4]; /* mov r4,r13 */
 goto L8c09433c;
L8c09433c: wr(m,r[15],r[6],4); /* mov.l r6,@r15 */
 goto L8c09433e;
L8c09433e: wr(m,r[15]+0x8u,r[7],4); /* mov.l r7,@(0x8,r15) */
 goto L8c094340;
L8c094340: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c094342;
L8c094342: *pr=0x8c094346u;r[4]=r[13];status=call(ctx,0x8c094280u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c094280 */
 goto L8c094346;
L8c094346: r[5]=r[0]; /* mov r0,r5 */
 goto L8c094348;
L8c094348: *sr=(*sr&~1u)|((int32_t)r[5]>=0); /* cmp/pz r5 */
 goto L8c09434a;
L8c09434a: take=!(*sr&1u);r[14]=0u;if(take)goto L8c094352;goto L8c09434e; /* bf/s 0x8c094352 */
 goto L8c09434e;
L8c09434e: r[6]=r[14];goto L8c094364; /* bra 0x8c094364 */
 goto L8c094352;
L8c094352: r[5]=r[13]; /* mov r13,r5 */
 goto L8c094354;
L8c094354: *pr=0x8c094358u;r[4]=rd(m,r[15],4);status=call(ctx,0x8c093f02u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c093f02 */
 goto L8c094358;
L8c094358: r[5]=r[0]; /* mov r0,r5 */
 goto L8c09435a;
L8c09435a: *sr=(*sr&~1u)|((int32_t)r[5]>=0); /* cmp/pz r5 */
 goto L8c09435c;
L8c09435c: if((*sr&1u))goto L8c094362; /* bt 0x8c094362 */
 goto L8c09435e;
L8c09435e: r[0]=4294967295u;goto L8c09439c; /* bra 0x8c09439c */
 goto L8c094362;
L8c094362: r[6]=1u; /* mov #0x1,r6 */
 goto L8c094364;
L8c094364: r[4]=52u; /* mov #0x34,r4 */
 goto L8c094366;
L8c094366: r[2]=rd(m,r[15]+0x18u,4); /* mov.l @(0x18,r15),r2 */
 goto L8c094368;
L8c094368: *macl=r[4]*r[5]; /* mul.l r4,r5 */
 goto L8c09436a;
L8c09436a: r[3]=rd(m,0x8c0943c0u,4); /* mov.l 0x8c0943c0,r3 */
 goto L8c09436c;
L8c09436c: r[2]+=1u; /* add #0x1,r2 */
 goto L8c09436e;
L8c09436e: r[0]=36u; /* mov #0x24,r0 */
 goto L8c094370;
L8c094370: fr[4]=0; /* fldi0 fr4 */
 goto L8c094372;
L8c094372: r[4]=*macl; /* sts MACL,r4 */
 goto L8c094374;
L8c094374: r[4]+=r[3]; /* add r3,r4 */
 goto L8c094376;
L8c094376: wr(m,r[4]+0x4u,r[2],4); /* mov.l r2,@(0x4,r4) */
 goto L8c094378;
L8c094378: r[1]=rd(m,r[15],4); /* mov.l @r15,r1 */
 goto L8c09437a;
L8c09437a: wr(m,r[4],r[1],4); /* mov.l r1,@r4 */
 goto L8c09437c;
L8c09437c: r[1]=16u; /* mov #0x10,r1 */
 goto L8c09437e;
L8c09437e: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c094380;
L8c094380: r[6]|=r[1]; /* or r1,r6 */
 goto L8c094382;
L8c094382: wr(m,r[4]+0x8u,r[2],4); /* mov.l r2,@(0x8,r4) */
 goto L8c094384;
L8c094384: wr(m,r[4]+0xcu,r[6],4); /* mov.l r6,@(0xc,r4) */
 goto L8c094386;
L8c094386: wr(m,r[4]+0x10u,r[13],4); /* mov.l r13,@(0x10,r4) */
 goto L8c094388;
L8c094388: wr(m,r[4]+0x14u,r[14],4); /* mov.l r14,@(0x14,r4) */
 goto L8c09438a;
L8c09438a: r[2]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r2 */
 goto L8c09438c;
L8c09438c: wr(m,r[4]+0x18u,r[2],4); /* mov.l r2,@(0x18,r4) */
 goto L8c09438e;
L8c09438e: wr(m,r[4]+0x1cu,r[14],4); /* mov.l r14,@(0x1c,r4) */
 goto L8c094390;
L8c094390: wr(m,r[4]+0x20u,r[14],4); /* mov.l r14,@(0x20,r4) */
 goto L8c094392;
L8c094392: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(4u&1u);unsigned pair=4u&~1u;wr(m,r[0]+r[4],f->bank[bank][pair],4);wr(m,r[0]+r[4]+4,f->bank[bank][pair+1],4);}else wr(m,r[0]+r[4],fr[4],4); /* fmov fr4,@(r0,r4) */
 goto L8c094394;
L8c094394: r[0]=40u; /* mov #0x28,r0 */
 goto L8c094396;
L8c094396: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(4u&1u);unsigned pair=4u&~1u;wr(m,r[0]+r[4],f->bank[bank][pair],4);wr(m,r[0]+r[4]+4,f->bank[bank][pair+1],4);}else wr(m,r[0]+r[4],fr[4],4); /* fmov fr4,@(r0,r4) */
 goto L8c094398;
L8c094398: r[0]=44u; /* mov #0x2c,r0 */
 goto L8c09439a;
L8c09439a: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(4u&1u);unsigned pair=4u&~1u;wr(m,r[0]+r[4],f->bank[bank][pair],4);wr(m,r[0]+r[4]+4,f->bank[bank][pair+1],4);}else wr(m,r[0]+r[4],fr[4],4); /* fmov fr4,@(r0,r4) */
 goto L8c09439c;
L8c09439c: r[15]+=12u; /* add #0xc,r15 */
 goto L8c09439e;
L8c09439e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c0943a0;
L8c0943a0: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c0943a2;
L8c0943a2: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
