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
/* Retail type-1 sound command with signed byte channel validation. */
SA_STAGE_EXPORT int sa_sound_command_type1(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c643b08;
L8c643b08: r[2]=rd(m,0x8c643b74u,4); /* mov.l 0x8c643b74,r2 */
 goto L8c643b0a;
L8c643b0a: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c643b0c;
L8c643b0c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c643b0e;
L8c643b0e: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c643b10;
L8c643b10: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c643b12;
L8c643b12: if(!(*sr&1u))goto L8c643b1a; /* bf 0x8c643b1a */
 goto L8c643b14;
L8c643b14: r[14]=rd(m,0x8c643b78u,4); /* mov.l 0x8c643b78,r14 */
 goto L8c643b16;
L8c643b16: goto L8c643b5e; /* bra 0x8c643b5e */
 goto L8c643b1a;
L8c643b1a: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c643b1c;
L8c643b1c: if(!(*sr&1u))goto L8c643b24; /* bf 0x8c643b24 */
 goto L8c643b1e;
L8c643b1e: r[14]=rd(m,0x8c643b7cu,4); /* mov.l 0x8c643b7c,r14 */
 goto L8c643b20;
L8c643b20: goto L8c643b5e; /* bra 0x8c643b5e */
 goto L8c643b24;
L8c643b24: r[14]=(uint32_t)(int32_t)(int8_t)r[7]; /* exts.b r7,r14 */
 goto L8c643b26;
L8c643b26: *sr=(*sr&~1u)|((int32_t)r[14]>=0); /* cmp/pz r14 */
 goto L8c643b28;
L8c643b28: if(!(*sr&1u))goto L8c643b30; /* bf 0x8c643b30 */
 goto L8c643b2a;
L8c643b2a: r[2]=15u; /* mov #0xf,r2 */
 goto L8c643b2c;
L8c643b2c: *sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[2]); /* cmp/gt r2,r14 */
 goto L8c643b2e;
L8c643b2e: if(!(*sr&1u))goto L8c643b36; /* bf 0x8c643b36 */
 goto L8c643b30;
L8c643b30: r[14]=rd(m,0x8c643b80u,4); /* mov.l 0x8c643b80,r14 */
 goto L8c643b32;
L8c643b32: goto L8c643b5e; /* bra 0x8c643b5e */
 goto L8c643b36;
L8c643b36: r[2]=rd(m,0x8c643b84u,4); /* mov.l 0x8c643b84,r2 */
 goto L8c643b38;
L8c643b38: r[3]=1u; /* mov #0x1,r3 */
 goto L8c643b3a;
L8c643b3a: r[1]=rd(m,0x8c643b88u,4); /* mov.l 0x8c643b88,r1 */
 goto L8c643b3c;
L8c643b3c: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c643b3e;
L8c643b3e: r[2]+=6u; /* add #0x6,r2 */
 goto L8c643b40;
L8c643b40: r[3]=rd(m,0x8c643b8cu,4); /* mov.l 0x8c643b8c,r3 */
 goto L8c643b42;
L8c643b42: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c643b44;
L8c643b44: r[4]=rd(m,0x8c643b84u,4); /* mov.l 0x8c643b84,r4 */
 goto L8c643b46;
L8c643b46: wr(m,r[1],r[0],1); /* mov.b r0,@r1 */
 goto L8c643b48;
L8c643b48: r[1]+=3u; /* add #0x3,r1 */
 goto L8c643b4a;
L8c643b4a: wr(m,r[3],r[5],1); /* mov.b r5,@r3 */
 goto L8c643b4c;
L8c643b4c: r[3]=r[14]; /* mov r14,r3 */
 goto L8c643b4e;
L8c643b4e: r[3]<<=2; /* shll2 r3 */
 goto L8c643b50;
L8c643b50: wr(m,r[2],r[6],1); /* mov.b r6,@r2 */
 goto L8c643b52;
L8c643b52: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c643b54;
L8c643b54: wr(m,r[1],r[3],1); /* mov.b r3,@r1 */
 goto L8c643b56;
L8c643b56: r[3]=rd(m,0x8c643b90u,4); /* mov.l 0x8c643b90,r3 */
 goto L8c643b58;
L8c643b58: target=r[3];*pr=0x8c643b5cu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643b5c;
L8c643b5c: r[14]=r[0]; /* mov r0,r14 */
 goto L8c643b5e;
L8c643b5e: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c643b60;
L8c643b60: if((*sr&1u))goto L8c643b6a; /* bt 0x8c643b6a */
 goto L8c643b62;
L8c643b62: r[3]=rd(m,0x8c643b98u,4); /* mov.l 0x8c643b98,r3 */
 goto L8c643b64;
L8c643b64: r[4]=rd(m,0x8c643b94u,4); /* mov.l 0x8c643b94,r4 */
 goto L8c643b66;
L8c643b66: target=r[3];*pr=0x8c643b6au;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643b6a;
L8c643b6a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c643b6c;
L8c643b6c: r[0]=r[14]; /* mov r14,r0 */
 goto L8c643b6e;
L8c643b6e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
