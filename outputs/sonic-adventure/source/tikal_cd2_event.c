#include "fpu_cleanup_transfer.h"
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
/* Static C recovery of retail Tikal helper 0cb00cd2..0cb00e74.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int tikal_cd2_event(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;
 goto Lcb00cd2;
Lcb00cd2: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto Lcb00cd4;
Lcb00cd4: r[14]=r[4]; /* mov r4,r14 */
 goto Lcb00cd6;
Lcb00cd6: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto Lcb00cd8;
Lcb00cd8: r[0]=36u; /* mov #0x24,r0 */
 goto Lcb00cda;
Lcb00cda: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto Lcb00cdc;
Lcb00cdc: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto Lcb00cde;
Lcb00cde: r[11]=r[5]; /* mov r5,r11 */
 goto Lcb00ce0;
Lcb00ce0: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto Lcb00ce2;
Lcb00ce2: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto Lcb00ce4;
Lcb00ce4: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto Lcb00ce6;
Lcb00ce6: r[3]=rd(m,r[14]+0x3cu,4); /* mov.l @(0x3c,r14),r3 */
 goto Lcb00ce8;
Lcb00ce8: r[2]=sx(m,r[3]+r[0],1); /* mov.b @(r0,r3),r2 */
 goto Lcb00cea;
Lcb00cea: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto Lcb00cec;
Lcb00cec: take=(*sr&1u);r[13]=r[6];if(take)goto Lcb00d20;goto Lcb00cf0; /* bt/s 0x0cb00d20 */
 goto Lcb00cf0;
Lcb00cf0: r[5]=r[11]; /* mov r11,r5 */
 goto Lcb00cf2;
Lcb00cf2: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00cf4;
Lcb00cf4: *pr=0xcb00cf8u;r[4]=r[14];status=call(ctx,0xcb00c64u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00c64 */
 goto Lcb00cf8;
Lcb00cf8: goto Lcb00e5e; /* bra 0x0cb00e5e */
 goto Lcb00d20;
Lcb00d20: r[2]=rd(m,r[14]+0x3cu,4); /* mov.l @(0x3c,r14),r2 */
 goto Lcb00d22;
Lcb00d22: r[0]=76u; /* mov #0x4c,r0 */
 goto Lcb00d24;
Lcb00d24: r[12]=4294967264u; /* mov #-0x20,r12 */
 goto Lcb00d26;
Lcb00d26: r[3]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto Lcb00d28;
Lcb00d28: r[12]<<=8; /* shll8 r12 */
 goto Lcb00d2a;
Lcb00d2a: r[12]+=4294967295u; /* add #-0x1,r12 */
 goto Lcb00d2c;
Lcb00d2c: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto Lcb00d2e;
Lcb00d2e: if((*sr&1u))goto Lcb00d3c; /* bt 0x0cb00d3c */
 goto Lcb00d30;
Lcb00d30: r[3]=9u; /* mov #0x9,r3 */
 goto Lcb00d32;
Lcb00d32: wr(m,r[14],r[3],1); /* mov.b r3,@r14 */
 goto Lcb00d34;
Lcb00d34: r[0]=sx(m,r[14]+0x4u,2); /* mov.w @(0x4,r14),r0 */
 goto Lcb00d36;
Lcb00d36: r[0]&=r[12]; /* and r12,r0 */
 goto Lcb00d38;
Lcb00d38: wr(m,r[14]+0x4u,r[0],2);goto Lcb00e62; /* bra 0x0cb00e62 */
 goto Lcb00d3c;
Lcb00d3c: r[3]=16u; /* mov #0x10,r3 */
 goto Lcb00d3e;
Lcb00d3e: r[0]=sx(m,r[14]+0x4u,2); /* mov.w @(0x4,r14),r0 */
 goto Lcb00d40;
Lcb00d40: r[3]<<=8; /* shll8 r3 */
 goto Lcb00d42;
Lcb00d42: *sr=(*sr&~1u)|!(r[3]&r[0]); /* tst r3,r0 */
 goto Lcb00d44;
Lcb00d44: if(!(*sr&1u))goto Lcb00d4a; /* bf 0x0cb00d4a */
 goto Lcb00d46;
Lcb00d46: goto Lcb00e5e; /* bra 0x0cb00e5e */
 goto Lcb00d4a;
Lcb00d4a: r[0]=sx(m,r[14]+0x1u,1); /* mov.b @(0x1,r14),r0 */
 goto Lcb00d4c;
Lcb00d4c: r[6]=3u; /* mov #0x3,r6 */
 goto Lcb00d4e;
Lcb00d4e: r[9]=rd(m,0x0cb00e00u,4); /* mov.l 0x0cb00e00,r9 */
 goto Lcb00d50;
Lcb00d50: r[5]=8u; /* mov #0x8,r5 */
 goto Lcb00d52;
Lcb00d52: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto Lcb00d54;
Lcb00d54: r[10]=5u; /* mov #0x5,r10 */
 goto Lcb00d56;
Lcb00d56: take=(*sr&1u);r[4]=0u;if(take)goto Lcb00d92;goto Lcb00d5a; /* bt/s 0x0cb00d92 */
 goto Lcb00d5a;
Lcb00d5a: *sr=(*sr&~1u)|(r[0]==0x8u); /* cmp/eq #0x8,r0 */
 goto Lcb00d5c;
Lcb00d5c: if((*sr&1u))goto Lcb00d92; /* bt 0x0cb00d92 */
 goto Lcb00d5e;
Lcb00d5e: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto Lcb00d60;
Lcb00d60: if((*sr&1u))goto Lcb00d92; /* bt 0x0cb00d92 */
 goto Lcb00d62;
Lcb00d62: *sr=(*sr&~1u)|(r[0]==0x7u); /* cmp/eq #0x7,r0 */
 goto Lcb00d64;
Lcb00d64: if((*sr&1u))goto Lcb00d92; /* bt 0x0cb00d92 */
 goto Lcb00d66;
Lcb00d66: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto Lcb00d68;
Lcb00d68: if((*sr&1u))goto Lcb00d9a; /* bt 0x0cb00d9a */
 goto Lcb00d6a;
Lcb00d6a: *sr=(*sr&~1u)|(r[0]==0x5u); /* cmp/eq #0x5,r0 */
 goto Lcb00d6c;
Lcb00d6c: if((*sr&1u))goto Lcb00d9a; /* bt 0x0cb00d9a */
 goto Lcb00d6e;
Lcb00d6e: *sr=(*sr&~1u)|(r[0]==0xbu); /* cmp/eq #0xb,r0 */
 goto Lcb00d70;
Lcb00d70: if((*sr&1u))goto Lcb00d9a; /* bt 0x0cb00d9a */
 goto Lcb00d72;
Lcb00d72: *sr=(*sr&~1u)|(r[0]==0xcu); /* cmp/eq #0xc,r0 */
 goto Lcb00d74;
Lcb00d74: if((*sr&1u))goto Lcb00da4; /* bt 0x0cb00da4 */
 goto Lcb00d76;
Lcb00d76: *sr=(*sr&~1u)|(r[0]==0x11u); /* cmp/eq #0x11,r0 */
 goto Lcb00d78;
Lcb00d78: if((*sr&1u))goto Lcb00dba; /* bt 0x0cb00dba */
 goto Lcb00d7a;
Lcb00d7a: *sr=(*sr&~1u)|(r[0]==0x9u); /* cmp/eq #0x9,r0 */
 goto Lcb00d7c;
Lcb00d7c: if((*sr&1u))goto Lcb00dd0; /* bt 0x0cb00dd0 */
 goto Lcb00d7e;
Lcb00d7e: *sr=(*sr&~1u)|(r[0]==0xau); /* cmp/eq #0xa,r0 */
 goto Lcb00d80;
Lcb00d80: if((*sr&1u))goto Lcb00dd0; /* bt 0x0cb00dd0 */
 goto Lcb00d82;
Lcb00d82: *sr=(*sr&~1u)|(r[0]==0x18u); /* cmp/eq #0x18,r0 */
 goto Lcb00d84;
Lcb00d84: if((*sr&1u))goto Lcb00de6; /* bt 0x0cb00de6 */
 goto Lcb00d86;
Lcb00d86: *sr=(*sr&~1u)|(r[0]==0x1au); /* cmp/eq #0x1a,r0 */
 goto Lcb00d88;
Lcb00d88: if((*sr&1u))goto Lcb00e20; /* bt 0x0cb00e20 */
 goto Lcb00d8a;
Lcb00d8a: *sr=(*sr&~1u)|(r[0]==0x32u); /* cmp/eq #0x32,r0 */
 goto Lcb00d8c;
Lcb00d8c: if((*sr&1u))goto Lcb00e40; /* bt 0x0cb00e40 */
 goto Lcb00d8e;
Lcb00d8e: goto Lcb00e5e; /* bra 0x0cb00e5e */
 goto Lcb00d92;
Lcb00d92: r[3]=4u; /* mov #0x4,r3 */
 goto Lcb00d94;
Lcb00d94: wr(m,r[14],r[3],1); /* mov.b r3,@r14 */
 goto Lcb00d96;
Lcb00d96: goto Lcb00d9c; /* bra 0x0cb00d9c */
 goto Lcb00d9a;
Lcb00d9a: wr(m,r[14],r[10],1); /* mov.b r10,@r14 */
 goto Lcb00d9c;
Lcb00d9c: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb00d9e;
Lcb00d9e: r[0]<<=2; /* shll2 r0 */
 goto Lcb00da0;
Lcb00da0: wr(m,r[13]+r[0],r[6],2);goto Lcb00e56; /* bra 0x0cb00e56 */
 goto Lcb00da4;
Lcb00da4: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb00da6;
Lcb00da6: wr(m,r[14],r[5],1); /* mov.b r5,@r14 */
 goto Lcb00da8;
Lcb00da8: r[0]<<=2; /* shll2 r0 */
 goto Lcb00daa;
Lcb00daa: r[5]=r[13]; /* mov r13,r5 */
 goto Lcb00dac;
Lcb00dac: wr(m,r[13]+r[0],r[4],2); /* mov.w r4,@(r0,r13) */
 goto Lcb00dae;
Lcb00dae: wr(m,r[14]+0x1cu,r[4],4); /* mov.l r4,@(0x1c,r14) */
 goto Lcb00db0;
Lcb00db0: wr(m,r[14]+0x14u,r[4],4); /* mov.l r4,@(0x14,r14) */
 goto Lcb00db2;
Lcb00db2: target=r[9];*pr=0xcb00db6u;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto Lcb00db6;
Lcb00db6: goto Lcb00e56; /* bra 0x0cb00e56 */
 goto Lcb00dba;
Lcb00dba: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb00dbc;
Lcb00dbc: wr(m,r[14],r[5],1); /* mov.b r5,@r14 */
 goto Lcb00dbe;
Lcb00dbe: r[0]<<=2; /* shll2 r0 */
 goto Lcb00dc0;
Lcb00dc0: r[5]=r[13]; /* mov r13,r5 */
 goto Lcb00dc2;
Lcb00dc2: wr(m,r[13]+r[0],r[10],2); /* mov.w r10,@(r0,r13) */
 goto Lcb00dc4;
Lcb00dc4: wr(m,r[14]+0x1cu,r[4],4); /* mov.l r4,@(0x1c,r14) */
 goto Lcb00dc6;
Lcb00dc6: wr(m,r[14]+0x14u,r[4],4); /* mov.l r4,@(0x14,r14) */
 goto Lcb00dc8;
Lcb00dc8: target=r[9];*pr=0xcb00dccu;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto Lcb00dcc;
Lcb00dcc: goto Lcb00e56; /* bra 0x0cb00e56 */
 goto Lcb00dd0;
Lcb00dd0: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb00dd2;
Lcb00dd2: wr(m,r[14],r[5],1); /* mov.b r5,@r14 */
 goto Lcb00dd4;
Lcb00dd4: r[0]<<=2; /* shll2 r0 */
 goto Lcb00dd6;
Lcb00dd6: r[5]=r[13]; /* mov r13,r5 */
 goto Lcb00dd8;
Lcb00dd8: wr(m,r[13]+r[0],r[4],2); /* mov.w r4,@(r0,r13) */
 goto Lcb00dda;
Lcb00dda: wr(m,r[14]+0x1cu,r[4],4); /* mov.l r4,@(0x1c,r14) */
 goto Lcb00ddc;
Lcb00ddc: wr(m,r[14]+0x14u,r[4],4); /* mov.l r4,@(0x14,r14) */
 goto Lcb00dde;
Lcb00dde: target=r[9];*pr=0xcb00de2u;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto Lcb00de2;
Lcb00de2: goto Lcb00e56; /* bra 0x0cb00e56 */
 goto Lcb00de6;
Lcb00de6: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb00de8;
Lcb00de8: r[2]=1u; /* mov #0x1,r2 */
 goto Lcb00dea;
Lcb00dea: r[0]<<=2; /* shll2 r0 */
 goto Lcb00dec;
Lcb00dec: wr(m,r[14],r[2],1); /* mov.b r2,@r14 */
 goto Lcb00dee;
Lcb00dee: wr(m,r[13]+r[0],r[4],2);goto Lcb00e56; /* bra 0x0cb00e56 */
 goto Lcb00e20;
Lcb00e20: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto Lcb00e22;
Lcb00e22: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto Lcb00e24;
Lcb00e24: if((*sr&1u))goto Lcb00e34; /* bt 0x0cb00e34 */
 goto Lcb00e26;
Lcb00e26: r[0]=4294967236u; /* mov #-0x3c,r0 */
 goto Lcb00e28;
Lcb00e28: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto Lcb00e2a;
Lcb00e2a: fr[3]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr3 */
 goto Lcb00e2c;
Lcb00e2c: r[0]=56u; /* mov #0x38,r0 */
 goto Lcb00e2e;
Lcb00e2e: fr[2]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr2 */
 goto Lcb00e30;
Lcb00e30: status=gt(f,sr,fr[2],fr[3]);if(status)return status; /* fcmp/gt fr3,fr2 */
 goto Lcb00e32;
Lcb00e32: if(!(*sr&1u))goto Lcb00e5e; /* bf 0x0cb00e5e */
 goto Lcb00e34;
Lcb00e34: r[0]=sx(m,r[14]+0x4u,2); /* mov.w @(0x4,r14),r0 */
 goto Lcb00e36;
Lcb00e36: r[3]=32u; /* mov #0x20,r3 */
 goto Lcb00e38;
Lcb00e38: r[3]<<=8; /* shll8 r3 */
 goto Lcb00e3a;
Lcb00e3a: r[0]|=r[3]; /* or r3,r0 */
 goto Lcb00e3c;
Lcb00e3c: wr(m,r[14]+0x4u,r[0],2);goto Lcb00e5e; /* bra 0x0cb00e5e */
 goto Lcb00e40;
Lcb00e40: r[3]=rd(m,0x0cb00f20u,4); /* mov.l 0x0cb00f20,r3 */
 goto Lcb00e42;
Lcb00e42: r[7]=0u; /* mov #0x0,r7 */
 goto Lcb00e44;
Lcb00e44: r[5]=r[7]; /* mov r7,r5 */
 goto Lcb00e46;
Lcb00e46: r[6]=r[7]; /* mov r7,r6 */
 goto Lcb00e48;
Lcb00e48: target=r[3];*pr=0xcb00e4cu;r[4]=23u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00e4c;
Lcb00e4c: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb00e4e;
Lcb00e4e: r[0]<<=2; /* shll2 r0 */
 goto Lcb00e50;
Lcb00e50: r[2]=10u; /* mov #0xa,r2 */
 goto Lcb00e52;
Lcb00e52: wr(m,r[14],r[2],1); /* mov.b r2,@r14 */
 goto Lcb00e54;
Lcb00e54: wr(m,r[13]+r[0],r[10],2); /* mov.w r10,@(r0,r13) */
 goto Lcb00e56;
Lcb00e56: r[0]=sx(m,r[14]+0x4u,2); /* mov.w @(0x4,r14),r0 */
 goto Lcb00e58;
Lcb00e58: r[0]&=r[12]; /* and r12,r0 */
 goto Lcb00e5a;
Lcb00e5a: wr(m,r[14]+0x4u,r[0],2);goto Lcb00e62; /* bra 0x0cb00e62 */
 goto Lcb00e5e;
Lcb00e5e: r[0]=0u;goto Lcb00e64; /* bra 0x0cb00e64 */
 goto Lcb00e62;
Lcb00e62: r[0]=1u; /* mov #0x1,r0 */
 goto Lcb00e64;
Lcb00e64: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto Lcb00e66;
Lcb00e66: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto Lcb00e68;
Lcb00e68: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto Lcb00e6a;
Lcb00e6a: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto Lcb00e6c;
Lcb00e6c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto Lcb00e6e;
Lcb00e6e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto Lcb00e70;
Lcb00e70: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
