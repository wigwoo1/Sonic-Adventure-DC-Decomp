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
/* Retail sound low-index/MIDI path. Packer varargs follow the recovered SH-4 ABI. */
SA_STAGE_EXPORT int sa_sound_midi_pack(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c643c18;
L8c643c18: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c643c1a;
L8c643c1a: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c643c1c;
L8c643c1c: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c643c1e;
L8c643c1e: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c643c20;
L8c643c20: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c643c22;
L8c643c22: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c643c24;
L8c643c24: r[2]=rd(m,0x8c643d18u,4); /* mov.l 0x8c643d18,r2 */
 goto L8c643c26;
L8c643c26: r[14]=r[5]; /* mov r5,r14 */
 goto L8c643c28;
L8c643c28: r[5]=0u; /* mov #0x0,r5 */
 goto L8c643c2a;
L8c643c2a: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c643c2c;
L8c643c2c: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c643c2e;
L8c643c2e: take=!(*sr&1u);r[13]=r[5];if(take)goto L8c643c38;goto L8c643c32; /* bf/s 0x8c643c38 */
 goto L8c643c32;
L8c643c32: r[13]=rd(m,0x8c643d1cu,4); /* mov.l 0x8c643d1c,r13 */
 goto L8c643c34;
L8c643c34: goto L8c643d5c; /* bra 0x8c643d5c */
 goto L8c643c38;
L8c643c38: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c643c3a;
L8c643c3a: if(!(*sr&1u))goto L8c643c42; /* bf 0x8c643c42 */
 goto L8c643c3c;
L8c643c3c: r[13]=rd(m,0x8c643d20u,4); /* mov.l 0x8c643d20,r13 */
 goto L8c643c3e;
L8c643c3e: goto L8c643d5c; /* bra 0x8c643d5c */
 goto L8c643c42;
L8c643c42: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c643c44;
L8c643c44: r[12]=sx(m,0x8c643d10u,2); /* mov.w 0x8c643d10,r12 */
 goto L8c643c46;
L8c643c46: wr(m,r[15]+0x8u,r[0],1); /* mov.b r0,@(0x8,r15) */
 goto L8c643c48;
L8c643c48: r[0]=32u; /* mov #0x20,r0 */
 goto L8c643c4a;
L8c643c4a: r[4]=sx(m,r[15]+r[0],1); /* mov.b @(r0,r15),r4 */
 goto L8c643c4c;
L8c643c4c: r[4]=r[4]&255u; /* extu.b r4,r4 */
 goto L8c643c4e;
L8c643c4e: r[3]=r[4]; /* mov r4,r3 */
 goto L8c643c50;
L8c643c50: r[3]&=r[12]; /* and r12,r3 */
 goto L8c643c52;
L8c643c52: *sr=(*sr&~1u)|(r[12]==r[3]); /* cmp/eq r12,r3 */
 goto L8c643c54;
L8c643c54: if((*sr&1u))goto L8c643d2c; /* bt 0x8c643d2c */
 goto L8c643c56;
L8c643c56: r[11]=r[15]; /* mov r15,r11 */
 goto L8c643c58;
L8c643c58: r[11]+=32u; /* add #0x20,r11 */
 goto L8c643c5a;
L8c643c5a: r[11]+=1u; /* add #0x1,r11 */
 goto L8c643c5c;
L8c643c5c: r[6]=3u; /* mov #0x3,r6 */
 goto L8c643c5e;
L8c643c5e: *sr=(*sr&~1u)|!(r[6]&r[11]); /* tst r6,r11 */
 goto L8c643c60;
L8c643c60: if((*sr&1u))goto L8c643c6a; /* bt 0x8c643c6a */
 goto L8c643c62;
L8c643c62: r[11]=r[15]; /* mov r15,r11 */
 goto L8c643c64;
L8c643c64: r[11]+=32u; /* add #0x20,r11 */
 goto L8c643c66;
L8c643c66: r[11]+=4u;goto L8c643c70; /* bra 0x8c643c70 */
 goto L8c643c6a;
L8c643c6a: r[11]=r[15]; /* mov r15,r11 */
 goto L8c643c6c;
L8c643c6c: r[11]+=32u; /* add #0x20,r11 */
 goto L8c643c6e;
L8c643c6e: r[11]+=1u; /* add #0x1,r11 */
 goto L8c643c70;
L8c643c70: r[7]=r[11]; /* mov r11,r7 */
 goto L8c643c72;
L8c643c72: r[7]+=4u; /* add #0x4,r7 */
 goto L8c643c74;
L8c643c74: *sr=(*sr&~1u)|!(r[6]&r[7]); /* tst r6,r7 */
 goto L8c643c76;
L8c643c76: take=(*sr&1u);wr(m,r[15],r[7],4);if(take)goto L8c643c80;goto L8c643c7a; /* bt/s 0x8c643c80 */
 goto L8c643c7a;
L8c643c7a: r[7]=r[11]; /* mov r11,r7 */
 goto L8c643c7c;
L8c643c7c: r[7]+=8u;goto L8c643c82; /* bra 0x8c643c82 */
 goto L8c643c80;
L8c643c80: r[7]=rd(m,r[15],4); /* mov.l @r15,r7 */
 goto L8c643c82;
L8c643c82: r[3]=r[7]; /* mov r7,r3 */
 goto L8c643c84;
L8c643c84: r[3]+=4294967292u; /* add #-0x4,r3 */
 goto L8c643c86;
L8c643c86: *sr=(*sr&~1u)|!(r[6]&r[3]); /* tst r6,r3 */
 goto L8c643c88;
L8c643c88: take=(*sr&1u);wr(m,r[15]+0x4u,r[3],4);if(take)goto L8c643c92;goto L8c643c8c; /* bt/s 0x8c643c92 */
 goto L8c643c8c;
L8c643c8c: r[2]=r[7]; /* mov r7,r2 */
 goto L8c643c8e;
L8c643c8e: r[2]+=4294967288u;goto L8c643c94; /* bra 0x8c643c94 */
 goto L8c643c92;
L8c643c92: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c643c94;
L8c643c94: r[1]=sx(m,0x8c643d12u,2); /* mov.w 0x8c643d12,r1 */
 goto L8c643c96;
L8c643c96: r[0]=r[4]; /* mov r4,r0 */
 goto L8c643c98;
L8c643c98: r[0]&=r[12]; /* and r12,r0 */
 goto L8c643c9a;
L8c643c9a: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c643c9c;
L8c643c9c: take=(*sr&1u);r[11]=rd(m,r[2],4);if(take)goto L8c643caa;goto L8c643ca0; /* bt/s 0x8c643caa */
 goto L8c643ca0;
L8c643ca0: r[1]=sx(m,0x8c643d14u,2); /* mov.w 0x8c643d14,r1 */
 goto L8c643ca2;
L8c643ca2: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c643ca4;
L8c643ca4: if((*sr&1u))goto L8c643caa; /* bt 0x8c643caa */
 goto L8c643ca6;
L8c643ca6: goto L8c643cae; /* bra 0x8c643cae */
 goto L8c643caa;
L8c643caa: r[12]=r[5];goto L8c643cd4; /* bra 0x8c643cd4 */
 goto L8c643cae;
L8c643cae: r[5]=r[7]; /* mov r7,r5 */
 goto L8c643cb0;
L8c643cb0: r[5]+=4u; /* add #0x4,r5 */
 goto L8c643cb2;
L8c643cb2: *sr=(*sr&~1u)|!(r[6]&r[5]); /* tst r6,r5 */
 goto L8c643cb4;
L8c643cb4: take=(*sr&1u);wr(m,r[15],r[5],4);if(take)goto L8c643cbe;goto L8c643cb8; /* bt/s 0x8c643cbe */
 goto L8c643cb8;
L8c643cb8: r[5]=r[7]; /* mov r7,r5 */
 goto L8c643cba;
L8c643cba: r[5]+=8u;goto L8c643cc0; /* bra 0x8c643cc0 */
 goto L8c643cbe;
L8c643cbe: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c643cc0;
L8c643cc0: r[12]=r[5]; /* mov r5,r12 */
 goto L8c643cc2;
L8c643cc2: r[12]+=4294967292u; /* add #-0x4,r12 */
 goto L8c643cc4;
L8c643cc4: *sr=(*sr&~1u)|!(r[6]&r[12]); /* tst r6,r12 */
 goto L8c643cc6;
L8c643cc6: take=(*sr&1u);wr(m,r[15]+0x4u,r[12],4);if(take)goto L8c643cd0;goto L8c643cca; /* bt/s 0x8c643cd0 */
 goto L8c643cca;
L8c643cca: r[12]=r[5]; /* mov r5,r12 */
 goto L8c643ccc;
L8c643ccc: r[12]+=4294967288u;goto L8c643cd2; /* bra 0x8c643cd2 */
 goto L8c643cd0;
L8c643cd0: r[12]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r12 */
 goto L8c643cd2;
L8c643cd2: r[12]=rd(m,r[12],4); /* mov.l @r12,r12 */
 goto L8c643cd4;
L8c643cd4: r[2]=(uint32_t)(int32_t)(int8_t)r[12]; /* exts.b r12,r2 */
 goto L8c643cd6;
L8c643cd6: r[3]=(uint32_t)(int32_t)(int8_t)r[11]; /* exts.b r11,r3 */
 goto L8c643cd8;
L8c643cd8: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c643cda;
L8c643cda: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c643cdc;
L8c643cdc: r[3]=rd(m,0x8c643d28u,4); /* mov.l 0x8c643d28,r3 */
 goto L8c643cde;
L8c643cde: r[2]=rd(m,0x8c643d24u,4); /* mov.l 0x8c643d24,r2 */
 goto L8c643ce0;
L8c643ce0: r[15]-=4;wr(m,r[15],r[4],4); /* mov.l r4,@-r15 */
 goto L8c643ce2;
L8c643ce2: target=r[3];*pr=0x8c643ce6u;r[15]-=4;wr(m,r[15],r[2],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643ce6;
L8c643ce6: r[15]+=16u; /* add #0x10,r15 */
 goto L8c643ce8;
L8c643ce8: r[0]=r[11]; /* mov r11,r0 */
 goto L8c643cea;
L8c643cea: r[1]=r[15]; /* mov r15,r1 */
 goto L8c643cec;
L8c643cec: r[1]+=32u; /* add #0x20,r1 */
 goto L8c643cee;
L8c643cee: wr(m,r[14],r[12],1); /* mov.b r12,@r14 */
 goto L8c643cf0;
L8c643cf0: wr(m,r[14]+0x1u,r[0],1); /* mov.b r0,@(0x1,r14) */
 goto L8c643cf2;
L8c643cf2: r[0]=sx(m,r[15]+0x8u,1); /* mov.b @(0x8,r15),r0 */
 goto L8c643cf4;
L8c643cf4: r[0]<<=2; /* shll2 r0 */
 goto L8c643cf6;
L8c643cf6: r[0]<<=2; /* shll2 r0 */
 goto L8c643cf8;
L8c643cf8: r[3]=r[0]; /* mov r0,r3 */
 goto L8c643cfa;
L8c643cfa: r[0]=sx(m,r[1],1); /* mov.b @r1,r0 */
 goto L8c643cfc;
L8c643cfc: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c643cfe;
L8c643cfe: r[0]|=r[3]; /* or r3,r0 */
 goto L8c643d00;
L8c643d00: wr(m,r[14]+0x2u,r[0],1); /* mov.b r0,@(0x2,r14) */
 goto L8c643d02;
L8c643d02: r[0]=32u; /* mov #0x20,r0 */
 goto L8c643d04;
L8c643d04: r[0]=sx(m,r[15]+r[0],1); /* mov.b @(r0,r15),r0 */
 goto L8c643d06;
L8c643d06: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c643d08;
L8c643d08: r[0]>>=2; /* shlr2 r0 */
 goto L8c643d0a;
L8c643d0a: r[0]>>=2; /* shlr2 r0 */
 goto L8c643d0c;
L8c643d0c: r[0]&=0x7u;goto L8c643d34; /* bra 0x8c643d34 */
 goto L8c643d2c;
L8c643d2c: r[0]=r[5]; /* mov r5,r0 */
 goto L8c643d2e;
L8c643d2e: wr(m,r[14],r[5],1); /* mov.b r5,@r14 */
 goto L8c643d30;
L8c643d30: wr(m,r[14]+0x1u,r[0],1); /* mov.b r0,@(0x1,r14) */
 goto L8c643d32;
L8c643d32: wr(m,r[14]+0x2u,r[0],1); /* mov.b r0,@(0x2,r14) */
 goto L8c643d34;
L8c643d34: r[12]=r[14]; /* mov r14,r12 */
 goto L8c643d36;
L8c643d36: wr(m,r[14]+0x3u,r[0],1); /* mov.b r0,@(0x3,r14) */
 goto L8c643d38;
L8c643d38: r[0]=sx(m,r[12]+0x3u,1); /* mov.b @(0x3,r12),r0 */
 goto L8c643d3a;
L8c643d3a: r[2]=rd(m,0x8c643d78u,4); /* mov.l 0x8c643d78,r2 */
 goto L8c643d3c;
L8c643d3c: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c643d3e;
L8c643d3e: r[15]-=4;wr(m,r[15],r[0],4); /* mov.l r0,@-r15 */
 goto L8c643d40;
L8c643d40: r[0]=sx(m,r[12]+0x2u,1); /* mov.b @(0x2,r12),r0 */
 goto L8c643d42;
L8c643d42: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c643d44;
L8c643d44: r[15]-=4;wr(m,r[15],r[0],4); /* mov.l r0,@-r15 */
 goto L8c643d46;
L8c643d46: r[0]=sx(m,r[12]+0x1u,1); /* mov.b @(0x1,r12),r0 */
 goto L8c643d48;
L8c643d48: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c643d4a;
L8c643d4a: r[15]-=4;wr(m,r[15],r[0],4); /* mov.l r0,@-r15 */
 goto L8c643d4c;
L8c643d4c: r[3]=sx(m,r[12],1); /* mov.b @r12,r3 */
 goto L8c643d4e;
L8c643d4e: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c643d50;
L8c643d50: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c643d52;
L8c643d52: r[3]=rd(m,0x8c643d7cu,4); /* mov.l 0x8c643d7c,r3 */
 goto L8c643d54;
L8c643d54: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c643d56;
L8c643d56: target=r[3];*pr=0x8c643d5au;r[15]-=4;wr(m,r[15],r[2],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643d5a;
L8c643d5a: r[15]+=24u; /* add #0x18,r15 */
 goto L8c643d5c;
L8c643d5c: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c643d5e;
L8c643d5e: if((*sr&1u))goto L8c643d68; /* bt 0x8c643d68 */
 goto L8c643d60;
L8c643d60: r[3]=rd(m,0x8c643d84u,4); /* mov.l 0x8c643d84,r3 */
 goto L8c643d62;
L8c643d62: r[4]=rd(m,0x8c643d80u,4); /* mov.l 0x8c643d80,r4 */
 goto L8c643d64;
L8c643d64: target=r[3];*pr=0x8c643d68u;r[5]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643d68;
L8c643d68: r[0]=r[13]; /* mov r13,r0 */
 goto L8c643d6a;
L8c643d6a: r[15]+=12u; /* add #0xc,r15 */
 goto L8c643d6c;
L8c643d6c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c643d6e;
L8c643d6e: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c643d70;
L8c643d70: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c643d72;
L8c643d72: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c643d74;
L8c643d74: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
