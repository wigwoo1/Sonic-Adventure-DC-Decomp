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
/* Static C recovery of retail Tikal helper 8c10dbc4..8c10e304.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_format_engine(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10dbc4;
L8c10dbc4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c10dbc6;
L8c10dbc6: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c10dbc8;
L8c10dbc8: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c10dbca;
L8c10dbca: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c10dbcc;
L8c10dbcc: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c10dbce;
L8c10dbce: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c10dbd0;
L8c10dbd0: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c10dbd2;
L8c10dbd2: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10dbd4;
L8c10dbd4: r[0]=sx(m,0x8c10dca4u,2); /* mov.w 0x8c10dca4,r0 */
 goto L8c10dbd6;
L8c10dbd6: r[15]+=r[0]; /* add r0,r15 */
 goto L8c10dbd8;
L8c10dbd8: r[14]=sx(m,0x8c10dca8u,2); /* mov.w 0x8c10dca8,r14 */
 goto L8c10dbda;
L8c10dbda: r[11]=0u; /* mov #0x0,r11 */
 goto L8c10dbdc;
L8c10dbdc: r[10]=sx(m,0x8c10dca6u,2); /* mov.w 0x8c10dca6,r10 */
 goto L8c10dbde;
L8c10dbde: r[0]=r[11]; /* mov r11,r0 */
 goto L8c10dbe0;
L8c10dbe0: r[14]+=r[15]; /* add r15,r14 */
 goto L8c10dbe2;
L8c10dbe2: r[12]=r[7]; /* mov r7,r12 */
 goto L8c10dbe4;
L8c10dbe4: r[10]+=r[15]; /* add r15,r10 */
 goto L8c10dbe6;
L8c10dbe6: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c10dbe8;
L8c10dbe8: wr(m,r[10],r[11],4); /* mov.l r11,@r10 */
 goto L8c10dbea;
L8c10dbea: wr(m,r[10]+0x4u,r[11],4); /* mov.l r11,@(0x4,r10) */
 goto L8c10dbec;
L8c10dbec: wr(m,r[10]+0x8u,r[11],4); /* mov.l r11,@(0x8,r10) */
 goto L8c10dbee;
L8c10dbee: wr(m,r[14],r[11],1); /* mov.b r11,@r14 */
 goto L8c10dbf0;
L8c10dbf0: wr(m,r[14]+0x1u,r[0],1); /* mov.b r0,@(0x1,r14) */
 goto L8c10dbf2;
L8c10dbf2: wr(m,r[14]+0x2u,r[0],2); /* mov.w r0,@(0x2,r14) */
 goto L8c10dbf4;
L8c10dbf4: wr(m,r[14]+0x4u,r[0],4); /* mov.l r0,@(0x4,r14) */
 goto L8c10dbf6;
L8c10dbf6: wr(m,r[14]+0x8u,r[0],4); /* mov.l r0,@(0x8,r14) */
 goto L8c10dbf8;
L8c10dbf8: wr(m,r[14]+0x10u,r[0],4); /* mov.l r0,@(0x10,r14) */
 goto L8c10dbfa;
L8c10dbfa: wr(m,r[14]+0x14u,r[0],4); /* mov.l r0,@(0x14,r14) */
 goto L8c10dbfc;
L8c10dbfc: wr(m,r[14]+0x18u,r[0],4); /* mov.l r0,@(0x18,r14) */
 goto L8c10dbfe;
L8c10dbfe: wr(m,r[14]+0x1cu,r[0],4); /* mov.l r0,@(0x1c,r14) */
 goto L8c10dc00;
L8c10dc00: wr(m,r[14]+0x20u,r[0],4); /* mov.l r0,@(0x20,r14) */
 goto L8c10dc02;
L8c10dc02: wr(m,r[14]+0x24u,r[0],4); /* mov.l r0,@(0x24,r14) */
 goto L8c10dc04;
L8c10dc04: take=!(*sr&1u);wr(m,r[14]+0x28u,r[0],4);if(take)goto L8c10dc3e;goto L8c10dc08; /* bf/s 0x8c10dc3e */
 goto L8c10dc08;
L8c10dc08: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c10dc0a;
L8c10dc0a: if(!(*sr&1u))goto L8c10dc14; /* bf 0x8c10dc14 */
 goto L8c10dc0c;
L8c10dc0c: r[2]=rd(m,0x8c10dcb4u,4); /* mov.l 0x8c10dcb4,r2 */
 goto L8c10dc0e;
L8c10dc0e: r[3]=sx(m,0x8c10dcaau,2); /* mov.w 0x8c10dcaa,r3 */
 goto L8c10dc10;
L8c10dc10: wr(m,r[2],r[3],4);goto L8c10dc52; /* bra 0x8c10dc52 */
 goto L8c10dc14;
L8c10dc14: r[0]=16u; /* mov #0x10,r0 */
 goto L8c10dc16;
L8c10dc16: r[0]=sx(m,r[6]+r[0],1); /* mov.b @(r0,r6),r0 */
 goto L8c10dc18;
L8c10dc18: *sr=(*sr&~1u)|!(r[0]&0x7u); /* tst #0x7,r0 */
 goto L8c10dc1a;
L8c10dc1a: if(!(*sr&1u))goto L8c10dc24; /* bf 0x8c10dc24 */
 goto L8c10dc1c;
L8c10dc1c: r[3]=rd(m,0x8c10dcb4u,4); /* mov.l 0x8c10dcb4,r3 */
 goto L8c10dc1e;
L8c10dc1e: r[2]=sx(m,0x8c10dcacu,2); /* mov.w 0x8c10dcac,r2 */
 goto L8c10dc20;
L8c10dc20: wr(m,r[3],r[2],4);goto L8c10dc52; /* bra 0x8c10dc52 */
 goto L8c10dc24;
L8c10dc24: r[0]=16u; /* mov #0x10,r0 */
 goto L8c10dc26;
L8c10dc26: r[7]=sx(m,r[6]+r[0],1); /* mov.b @(r0,r6),r7 */
 goto L8c10dc28;
L8c10dc28: r[3]=4u; /* mov #0x4,r3 */
 goto L8c10dc2a;
L8c10dc2a: *sr=(*sr&~1u)|!(r[7]&r[3]); /* tst r7,r3 */
 goto L8c10dc2c;
L8c10dc2c: if(!(*sr&1u))goto L8c10dc3c; /* bf 0x8c10dc3c */
 goto L8c10dc2e;
L8c10dc2e: r[1]=1u; /* mov #0x1,r1 */
 goto L8c10dc30;
L8c10dc30: *sr=(*sr&~1u)|!(r[7]&r[1]); /* tst r7,r1 */
 goto L8c10dc32;
L8c10dc32: if((*sr&1u))goto L8c10dc3c; /* bt 0x8c10dc3c */
 goto L8c10dc34;
L8c10dc34: r[3]=rd(m,0x8c10dcb4u,4); /* mov.l 0x8c10dcb4,r3 */
 goto L8c10dc36;
L8c10dc36: r[2]=sx(m,0x8c10dcaeu,2); /* mov.w 0x8c10dcae,r2 */
 goto L8c10dc38;
L8c10dc38: wr(m,r[3],r[2],4);goto L8c10dc52; /* bra 0x8c10dc52 */
 goto L8c10dc3c;
L8c10dc3c: wr(m,r[10]+0xcu,r[5],4); /* mov.l r5,@(0xc,r10) */
 goto L8c10dc3e;
L8c10dc3e: r[0]=48u; /* mov #0x30,r0 */
 goto L8c10dc40;
L8c10dc40: wr(m,r[10],r[4],4); /* mov.l r4,@r10 */
 goto L8c10dc42;
L8c10dc42: wr(m,r[10]+0x4u,r[6],4); /* mov.l r6,@(0x4,r10) */
 goto L8c10dc44;
L8c10dc44: wr(m,r[10]+0x8u,r[11],4); /* mov.l r11,@(0x8,r10) */
 goto L8c10dc46;
L8c10dc46: wr(m,r[14],r[11],1); /* mov.b r11,@r14 */
 goto L8c10dc48;
L8c10dc48: wr(m,r[15]+0x8u,r[0],1); /* mov.b r0,@(0x8,r15) */
 goto L8c10dc4a;
L8c10dc4a: *pr=0x8c10dc4eu;r[4]=r[10];status=call(ctx,0x8c10e576u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c10e576 */
 goto L8c10dc4e;
L8c10dc4e: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c10dc50;
L8c10dc50: if((*sr&1u))goto L8c10dc56; /* bt 0x8c10dc56 */
 goto L8c10dc52;
L8c10dc52: r[0]=4294967295u;goto L8c10e2ec; /* bra 0x8c10e2ec */
 goto L8c10dc56;
L8c10dc56: r[9]=r[15]; /* mov r15,r9 */
 goto L8c10dc58;
L8c10dc58: r[9]+=20u; /* add #0x14,r9 */
 goto L8c10dc5a;
L8c10dc5a: r[8]=sx(m,0x8c10dcb0u,2); /* mov.w 0x8c10dcb0,r8 */
 goto L8c10dc5c;
L8c10dc5c: r[3]=r[9]; /* mov r9,r3 */
 goto L8c10dc5e;
L8c10dc5e: wr(m,r[15]+0x10u,r[3],4); /* mov.l r3,@(0x10,r15) */
 goto L8c10dc60;
L8c10dc60: r[13]=3u;goto L8c10e2ce; /* bra 0x8c10e2ce */
 goto L8c10dc64;
L8c10dc64: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10dc66;
L8c10dc66: *sr=(*sr&~1u)|(r[0]==0x00000025u); /* cmp/eq #0x25,r0 */
 goto L8c10dc68;
L8c10dc68: if((*sr&1u))goto L8c10dc6e; /* bt 0x8c10dc6e */
 goto L8c10dc6a;
L8c10dc6a: goto L8c10e2aa; /* bra 0x8c10e2aa */
 goto L8c10dc6e;
L8c10dc6e: wr(m,r[14],r[11],1); /* mov.b r11,@r14 */
 goto L8c10dc70;
L8c10dc70: r[12]+=1u; /* add #0x1,r12 */
 goto L8c10dc72;
L8c10dc72: wr(m,r[14]+0x10u,r[9],4); /* mov.l r9,@(0x10,r14) */
 goto L8c10dc74;
L8c10dc74: wr(m,r[14]+0x14u,r[9],4); /* mov.l r9,@(0x14,r14) */
 goto L8c10dc76;
L8c10dc76: wr(m,r[14]+0x18u,r[11],4); /* mov.l r11,@(0x18,r14) */
 goto L8c10dc78;
L8c10dc78: wr(m,r[14]+0x1cu,r[11],4); /* mov.l r11,@(0x1c,r14) */
 goto L8c10dc7a;
L8c10dc7a: wr(m,r[14]+0x20u,r[11],4); /* mov.l r11,@(0x20,r14) */
 goto L8c10dc7c;
L8c10dc7c: wr(m,r[14]+0x24u,r[11],4); /* mov.l r11,@(0x24,r14) */
 goto L8c10dc7e;
L8c10dc7e: wr(m,r[14]+0x28u,r[11],4);goto L8c10dcce; /* bra 0x8c10dcce */
 goto L8c10dc82;
L8c10dc82: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10dc84;
L8c10dc84: *sr=(*sr&~1u)|(r[0]==0x0000002du); /* cmp/eq #0x2d,r0 */
 goto L8c10dc86;
L8c10dc86: if((*sr&1u))goto L8c10dc98; /* bt 0x8c10dc98 */
 goto L8c10dc88;
L8c10dc88: *sr=(*sr&~1u)|(r[0]==0x0000002bu); /* cmp/eq #0x2b,r0 */
 goto L8c10dc8a;
L8c10dc8a: if((*sr&1u))goto L8c10dc9e; /* bt 0x8c10dc9e */
 goto L8c10dc8c;
L8c10dc8c: *sr=(*sr&~1u)|(r[0]==0x00000020u); /* cmp/eq #0x20,r0 */
 goto L8c10dc8e;
L8c10dc8e: if((*sr&1u))goto L8c10dcb8; /* bt 0x8c10dcb8 */
 goto L8c10dc90;
L8c10dc90: *sr=(*sr&~1u)|(r[0]==0x00000023u); /* cmp/eq #0x23,r0 */
 goto L8c10dc92;
L8c10dc92: if((*sr&1u))goto L8c10dcc6; /* bt 0x8c10dcc6 */
 goto L8c10dc94;
L8c10dc94: goto L8c10dccc; /* bra 0x8c10dccc */
 goto L8c10dc98;
L8c10dc98: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c10dc9a;
L8c10dc9a: r[0]|=0x2u;goto L8c10dcca; /* bra 0x8c10dcca */
 goto L8c10dc9e;
L8c10dc9e: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c10dca0;
L8c10dca0: r[0]|=0x8u;goto L8c10dcca; /* bra 0x8c10dcca */
 goto L8c10dcb8;
L8c10dcb8: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c10dcba;
L8c10dcba: *sr=(*sr&~1u)|!(r[0]&0x8u); /* tst #0x8,r0 */
 goto L8c10dcbc;
L8c10dcbc: if(!(*sr&1u))goto L8c10dccc; /* bf 0x8c10dccc */
 goto L8c10dcbe;
L8c10dcbe: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c10dcc0;
L8c10dcc0: r[0]|=0x10u; /* or #0x10,r0 */
 goto L8c10dcc2;
L8c10dcc2: wr(m,r[14],r[0],1);goto L8c10dccc; /* bra 0x8c10dccc */
 goto L8c10dcc6;
L8c10dcc6: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c10dcc8;
L8c10dcc8: r[0]|=0x4u; /* or #0x4,r0 */
 goto L8c10dcca;
L8c10dcca: wr(m,r[14],r[0],1); /* mov.b r0,@r14 */
 goto L8c10dccc;
L8c10dccc: r[12]+=1u; /* add #0x1,r12 */
 goto L8c10dcce;
L8c10dcce: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10dcd0;
L8c10dcd0: *sr=(*sr&~1u)|(r[0]==0x0000002du); /* cmp/eq #0x2d,r0 */
 goto L8c10dcd2;
L8c10dcd2: take=(*sr&1u);r[4]=r[0];if(take)goto L8c10dc82;goto L8c10dcd6; /* bt/s 0x8c10dc82 */
 goto L8c10dcd6;
L8c10dcd6: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10dcd8;
L8c10dcd8: *sr=(*sr&~1u)|(r[0]==0x0000002bu); /* cmp/eq #0x2b,r0 */
 goto L8c10dcda;
L8c10dcda: if((*sr&1u))goto L8c10dc82; /* bt 0x8c10dc82 */
 goto L8c10dcdc;
L8c10dcdc: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10dcde;
L8c10dcde: *sr=(*sr&~1u)|(r[0]==0x00000020u); /* cmp/eq #0x20,r0 */
 goto L8c10dce0;
L8c10dce0: if((*sr&1u))goto L8c10dc82; /* bt 0x8c10dc82 */
 goto L8c10dce2;
L8c10dce2: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10dce4;
L8c10dce4: *sr=(*sr&~1u)|(r[0]==0x00000023u); /* cmp/eq #0x23,r0 */
 goto L8c10dce6;
L8c10dce6: if((*sr&1u))goto L8c10dc82; /* bt 0x8c10dc82 */
 goto L8c10dce8;
L8c10dce8: r[0]=32u; /* mov #0x20,r0 */
 goto L8c10dcea;
L8c10dcea: wr(m,r[14]+0x1u,r[0],1); /* mov.b r0,@(0x1,r14) */
 goto L8c10dcec;
L8c10dcec: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10dcee;
L8c10dcee: *sr=(*sr&~1u)|(r[0]==0x00000030u); /* cmp/eq #0x30,r0 */
 goto L8c10dcf0;
L8c10dcf0: if(!(*sr&1u))goto L8c10dcf8; /* bf 0x8c10dcf8 */
 goto L8c10dcf2;
L8c10dcf2: r[0]=48u; /* mov #0x30,r0 */
 goto L8c10dcf4;
L8c10dcf4: r[12]+=1u; /* add #0x1,r12 */
 goto L8c10dcf6;
L8c10dcf6: wr(m,r[14]+0x1u,r[0],1); /* mov.b r0,@(0x1,r14) */
 goto L8c10dcf8;
L8c10dcf8: wr(m,r[14]+0x4u,r[11],4); /* mov.l r11,@(0x4,r14) */
 goto L8c10dcfa;
L8c10dcfa: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10dcfc;
L8c10dcfc: *sr=(*sr&~1u)|(r[0]==0x0000002au); /* cmp/eq #0x2a,r0 */
 goto L8c10dcfe;
L8c10dcfe: if(!(*sr&1u))goto L8c10dd56; /* bf 0x8c10dd56 */
 goto L8c10dd00;
L8c10dd00: r[0]=sx(m,0x8c10dd8eu,2); /* mov.w 0x8c10dd8e,r0 */
 goto L8c10dd02;
L8c10dd02: r[4]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r4 */
 goto L8c10dd04;
L8c10dd04: r[4]+=4u; /* add #0x4,r4 */
 goto L8c10dd06;
L8c10dd06: r[3]=r[4]; /* mov r4,r3 */
 goto L8c10dd08;
L8c10dd08: *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c10dd0a;
L8c10dd0a: if((*sr&1u))goto L8c10dd14; /* bt 0x8c10dd14 */
 goto L8c10dd0c;
L8c10dd0c: r[0]=sx(m,0x8c10dd8eu,2); /* mov.w 0x8c10dd8e,r0 */
 goto L8c10dd0e;
L8c10dd0e: r[2]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r2 */
 goto L8c10dd10;
L8c10dd10: r[2]+=8u;goto L8c10dd16; /* bra 0x8c10dd16 */
 goto L8c10dd14;
L8c10dd14: r[2]=r[4]; /* mov r4,r2 */
 goto L8c10dd16;
L8c10dd16: r[5]=r[2]; /* mov r2,r5 */
 goto L8c10dd18;
L8c10dd18: r[5]+=4294967292u; /* add #-0x4,r5 */
 goto L8c10dd1a;
L8c10dd1a: r[1]=r[5]; /* mov r5,r1 */
 goto L8c10dd1c;
L8c10dd1c: r[0]=sx(m,0x8c10dd8eu,2); /* mov.w 0x8c10dd8e,r0 */
 goto L8c10dd1e;
L8c10dd1e: *sr=(*sr&~1u)|!(r[13]&r[1]); /* tst r13,r1 */
 goto L8c10dd20;
L8c10dd20: take=(*sr&1u);wr(m,r[15]+r[0],r[2],4);if(take)goto L8c10dd2a;goto L8c10dd24; /* bt/s 0x8c10dd2a */
 goto L8c10dd24;
L8c10dd24: r[3]=r[2]; /* mov r2,r3 */
 goto L8c10dd26;
L8c10dd26: r[3]+=4294967288u;goto L8c10dd2c; /* bra 0x8c10dd2c */
 goto L8c10dd2a;
L8c10dd2a: r[3]=r[5]; /* mov r5,r3 */
 goto L8c10dd2c;
L8c10dd2c: r[1]=rd(m,r[3],4); /* mov.l @r3,r1 */
 goto L8c10dd2e;
L8c10dd2e: r[3]=r[1]; /* mov r1,r3 */
 goto L8c10dd30;
L8c10dd30: *sr=(*sr&~1u)|((int32_t)r[3]>=0); /* cmp/pz r3 */
 goto L8c10dd32;
L8c10dd32: take=(*sr&1u);wr(m,r[14]+0x4u,r[1],4);if(take)goto L8c10dd42;goto L8c10dd36; /* bt/s 0x8c10dd42 */
 goto L8c10dd36;
L8c10dd36: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c10dd38;
L8c10dd38: r[0]|=0x2u; /* or #0x2,r0 */
 goto L8c10dd3a;
L8c10dd3a: wr(m,r[14],r[0],1); /* mov.b r0,@r14 */
 goto L8c10dd3c;
L8c10dd3c: r[3]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r3 */
 goto L8c10dd3e;
L8c10dd3e: r[3]=0u-r[3]; /* neg r3,r3 */
 goto L8c10dd40;
L8c10dd40: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c10dd42;
L8c10dd42: r[1]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r1 */
 goto L8c10dd44;
L8c10dd44: r[2]=sx(m,0x8c10dd90u,2); /* mov.w 0x8c10dd90,r2 */
 goto L8c10dd46;
L8c10dd46: *sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[2]); /* cmp/gt r2,r1 */
 goto L8c10dd48;
L8c10dd48: take=!(*sr&1u);r[12]+=1u;if(take)goto L8c10dd56;goto L8c10dd4c; /* bf/s 0x8c10dd56 */
 goto L8c10dd4c;
L8c10dd4c: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c10dd4e;
L8c10dd4e: r[1]=rd(m,0x8c10dd94u,4); /* mov.l 0x8c10dd94,r1 */
 goto L8c10dd50;
L8c10dd50: r[0]|=0x1u; /* or #0x1,r0 */
 goto L8c10dd52;
L8c10dd52: wr(m,r[14],r[0],1); /* mov.b r0,@r14 */
 goto L8c10dd54;
L8c10dd54: wr(m,r[1],r[8],4); /* mov.l r8,@r1 */
 goto L8c10dd56;
L8c10dd56: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10dd58;
L8c10dd58: r[1]=rd(m,0x8c10dd98u,4); /* mov.l 0x8c10dd98,r1 */
 goto L8c10dd5a;
L8c10dd5a: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c10dd5c;
L8c10dd5c: r[0]=sx(m,r[1]+r[0],1); /* mov.b @(r0,r1),r0 */
 goto L8c10dd5e;
L8c10dd5e: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c10dd60;
L8c10dd60: *sr=(*sr&~1u)|!(r[0]&0x4u); /* tst #0x4,r0 */
 goto L8c10dd62;
L8c10dd62: if((*sr&1u))goto L8c10ddb8; /* bt 0x8c10ddb8 */
 goto L8c10dd64;
L8c10dd64: wr(m,r[14]+0x4u,r[11],4);goto L8c10ddaa; /* bra 0x8c10ddaa */
 goto L8c10dd68;
L8c10dd68: r[4]=sx(m,r[12],1); /* mov.b @r12,r4 */
 goto L8c10dd6a;
L8c10dd6a: r[1]=sx(m,0x8c10dd90u,2); /* mov.w 0x8c10dd90,r1 */
 goto L8c10dd6c;
L8c10dd6c: r[3]=rd(m,0x8c10dd9cu,4); /* mov.l 0x8c10dd9c,r3 */
 goto L8c10dd6e;
L8c10dd6e: r[4]+=4294967248u; /* add #-0x30,r4 */
 goto L8c10dd70;
L8c10dd70: r[1]-=r[4]; /* sub r4,r1 */
 goto L8c10dd72;
L8c10dd72: target=r[3];*pr=0x8c10dd76u;r[0]=10u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10dd76;
L8c10dd76: r[2]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r2 */
 goto L8c10dd78;
L8c10dd78: *sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[2]); /* cmp/ge r2,r0 */
 goto L8c10dd7a;
L8c10dd7a: take=!(*sr&1u);r[12]+=1u;if(take)goto L8c10dda0;goto L8c10dd7e; /* bf/s 0x8c10dda0 */
 goto L8c10dd7e;
L8c10dd7e: r[1]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r1 */
 goto L8c10dd80;
L8c10dd80: r[3]=r[1]; /* mov r1,r3 */
 goto L8c10dd82;
L8c10dd82: r[1]<<=2; /* shll2 r1 */
 goto L8c10dd84;
L8c10dd84: r[1]+=r[3]; /* add r3,r1 */
 goto L8c10dd86;
L8c10dd86: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c10dd88;
L8c10dd88: r[1]+=r[4]; /* add r4,r1 */
 goto L8c10dd8a;
L8c10dd8a: wr(m,r[14]+0x4u,r[1],4);goto L8c10ddaa; /* bra 0x8c10ddaa */
 goto L8c10dda0;
L8c10dda0: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c10dda2;
L8c10dda2: r[3]=rd(m,0x8c10de54u,4); /* mov.l 0x8c10de54,r3 */
 goto L8c10dda4;
L8c10dda4: r[0]|=0x1u; /* or #0x1,r0 */
 goto L8c10dda6;
L8c10dda6: wr(m,r[14],r[0],1); /* mov.b r0,@r14 */
 goto L8c10dda8;
L8c10dda8: wr(m,r[3],r[8],4); /* mov.l r8,@r3 */
 goto L8c10ddaa;
L8c10ddaa: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10ddac;
L8c10ddac: r[1]=rd(m,0x8c10de58u,4); /* mov.l 0x8c10de58,r1 */
 goto L8c10ddae;
L8c10ddae: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c10ddb0;
L8c10ddb0: r[0]=sx(m,r[1]+r[0],1); /* mov.b @(r0,r1),r0 */
 goto L8c10ddb2;
L8c10ddb2: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c10ddb4;
L8c10ddb4: *sr=(*sr&~1u)|!(r[0]&0x4u); /* tst #0x4,r0 */
 goto L8c10ddb6;
L8c10ddb6: if(!(*sr&1u))goto L8c10dd68; /* bf 0x8c10dd68 */
 goto L8c10ddb8;
L8c10ddb8: r[2]=4294967295u; /* mov #-0x1,r2 */
 goto L8c10ddba;
L8c10ddba: wr(m,r[14]+0x8u,r[2],4); /* mov.l r2,@(0x8,r14) */
 goto L8c10ddbc;
L8c10ddbc: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10ddbe;
L8c10ddbe: *sr=(*sr&~1u)|(r[0]==0x0000002eu); /* cmp/eq #0x2e,r0 */
 goto L8c10ddc0;
L8c10ddc0: if(!(*sr&1u))goto L8c10de78; /* bf 0x8c10de78 */
 goto L8c10ddc2;
L8c10ddc2: r[12]+=1u; /* add #0x1,r12 */
 goto L8c10ddc4;
L8c10ddc4: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10ddc6;
L8c10ddc6: *sr=(*sr&~1u)|(r[0]==0x0000002au); /* cmp/eq #0x2a,r0 */
 goto L8c10ddc8;
L8c10ddc8: if(!(*sr&1u))goto L8c10de18; /* bf 0x8c10de18 */
 goto L8c10ddca;
L8c10ddca: r[0]=sx(m,0x8c10de50u,2); /* mov.w 0x8c10de50,r0 */
 goto L8c10ddcc;
L8c10ddcc: r[4]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r4 */
 goto L8c10ddce;
L8c10ddce: r[4]+=4u; /* add #0x4,r4 */
 goto L8c10ddd0;
L8c10ddd0: r[3]=r[4]; /* mov r4,r3 */
 goto L8c10ddd2;
L8c10ddd2: *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c10ddd4;
L8c10ddd4: if((*sr&1u))goto L8c10ddde; /* bt 0x8c10ddde */
 goto L8c10ddd6;
L8c10ddd6: r[0]=sx(m,0x8c10de50u,2); /* mov.w 0x8c10de50,r0 */
 goto L8c10ddd8;
L8c10ddd8: r[2]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r2 */
 goto L8c10ddda;
L8c10ddda: r[2]+=8u;goto L8c10dde0; /* bra 0x8c10dde0 */
 goto L8c10ddde;
L8c10ddde: r[2]=r[4]; /* mov r4,r2 */
 goto L8c10dde0;
L8c10dde0: r[5]=r[2]; /* mov r2,r5 */
 goto L8c10dde2;
L8c10dde2: r[5]+=4294967292u; /* add #-0x4,r5 */
 goto L8c10dde4;
L8c10dde4: r[1]=r[5]; /* mov r5,r1 */
 goto L8c10dde6;
L8c10dde6: r[0]=sx(m,0x8c10de50u,2); /* mov.w 0x8c10de50,r0 */
 goto L8c10dde8;
L8c10dde8: *sr=(*sr&~1u)|!(r[13]&r[1]); /* tst r13,r1 */
 goto L8c10ddea;
L8c10ddea: take=(*sr&1u);wr(m,r[15]+r[0],r[2],4);if(take)goto L8c10ddf4;goto L8c10ddee; /* bt/s 0x8c10ddf4 */
 goto L8c10ddee;
L8c10ddee: r[3]=r[2]; /* mov r2,r3 */
 goto L8c10ddf0;
L8c10ddf0: r[3]+=4294967288u;goto L8c10ddf6; /* bra 0x8c10ddf6 */
 goto L8c10ddf4;
L8c10ddf4: r[3]=r[5]; /* mov r5,r3 */
 goto L8c10ddf6;
L8c10ddf6: r[1]=rd(m,r[3],4); /* mov.l @r3,r1 */
 goto L8c10ddf8;
L8c10ddf8: r[3]=r[1]; /* mov r1,r3 */
 goto L8c10ddfa;
L8c10ddfa: *sr=(*sr&~1u)|((int32_t)r[3]>=0); /* cmp/pz r3 */
 goto L8c10ddfc;
L8c10ddfc: take=(*sr&1u);wr(m,r[14]+0x8u,r[1],4);if(take)goto L8c10de04;goto L8c10de00; /* bt/s 0x8c10de04 */
 goto L8c10de00;
L8c10de00: r[1]=4294967295u; /* mov #-0x1,r1 */
 goto L8c10de02;
L8c10de02: wr(m,r[14]+0x8u,r[1],4); /* mov.l r1,@(0x8,r14) */
 goto L8c10de04;
L8c10de04: r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto L8c10de06;
L8c10de06: r[3]=sx(m,0x8c10de52u,2); /* mov.w 0x8c10de52,r3 */
 goto L8c10de08;
L8c10de08: *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[3]); /* cmp/gt r3,r2 */
 goto L8c10de0a;
L8c10de0a: take=!(*sr&1u);r[12]+=1u;if(take)goto L8c10de18;goto L8c10de0e; /* bf/s 0x8c10de18 */
 goto L8c10de0e;
L8c10de0e: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c10de10;
L8c10de10: r[2]=rd(m,0x8c10de54u,4); /* mov.l 0x8c10de54,r2 */
 goto L8c10de12;
L8c10de12: r[0]|=0x1u; /* or #0x1,r0 */
 goto L8c10de14;
L8c10de14: wr(m,r[14],r[0],1); /* mov.b r0,@r14 */
 goto L8c10de16;
L8c10de16: wr(m,r[2],r[8],4); /* mov.l r8,@r2 */
 goto L8c10de18;
L8c10de18: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10de1a;
L8c10de1a: r[1]=rd(m,0x8c10de58u,4); /* mov.l 0x8c10de58,r1 */
 goto L8c10de1c;
L8c10de1c: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c10de1e;
L8c10de1e: r[0]=sx(m,r[1]+r[0],1); /* mov.b @(r0,r1),r0 */
 goto L8c10de20;
L8c10de20: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c10de22;
L8c10de22: *sr=(*sr&~1u)|!(r[0]&0x4u); /* tst #0x4,r0 */
 goto L8c10de24;
L8c10de24: if((*sr&1u))goto L8c10de78; /* bt 0x8c10de78 */
 goto L8c10de26;
L8c10de26: wr(m,r[14]+0x8u,r[11],4);goto L8c10de6a; /* bra 0x8c10de6a */
 goto L8c10de2a;
L8c10de2a: r[4]=sx(m,r[12],1); /* mov.b @r12,r4 */
 goto L8c10de2c;
L8c10de2c: r[1]=sx(m,0x8c10de52u,2); /* mov.w 0x8c10de52,r1 */
 goto L8c10de2e;
L8c10de2e: r[3]=rd(m,0x8c10de5cu,4); /* mov.l 0x8c10de5c,r3 */
 goto L8c10de30;
L8c10de30: r[4]+=4294967248u; /* add #-0x30,r4 */
 goto L8c10de32;
L8c10de32: r[1]-=r[4]; /* sub r4,r1 */
 goto L8c10de34;
L8c10de34: target=r[3];*pr=0x8c10de38u;r[0]=10u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10de38;
L8c10de38: r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto L8c10de3a;
L8c10de3a: *sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[2]); /* cmp/ge r2,r0 */
 goto L8c10de3c;
L8c10de3c: take=!(*sr&1u);r[12]+=1u;if(take)goto L8c10de60;goto L8c10de40; /* bf/s 0x8c10de60 */
 goto L8c10de40;
L8c10de40: r[1]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r1 */
 goto L8c10de42;
L8c10de42: r[3]=r[1]; /* mov r1,r3 */
 goto L8c10de44;
L8c10de44: r[1]<<=2; /* shll2 r1 */
 goto L8c10de46;
L8c10de46: r[1]+=r[3]; /* add r3,r1 */
 goto L8c10de48;
L8c10de48: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c10de4a;
L8c10de4a: r[1]+=r[4]; /* add r4,r1 */
 goto L8c10de4c;
L8c10de4c: wr(m,r[14]+0x8u,r[1],4);goto L8c10de6a; /* bra 0x8c10de6a */
 goto L8c10de60;
L8c10de60: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c10de62;
L8c10de62: r[3]=rd(m,0x8c10df34u,4); /* mov.l 0x8c10df34,r3 */
 goto L8c10de64;
L8c10de64: r[0]|=0x1u; /* or #0x1,r0 */
 goto L8c10de66;
L8c10de66: wr(m,r[14],r[0],1); /* mov.b r0,@r14 */
 goto L8c10de68;
L8c10de68: wr(m,r[3],r[8],4); /* mov.l r8,@r3 */
 goto L8c10de6a;
L8c10de6a: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10de6c;
L8c10de6c: r[1]=rd(m,0x8c10df38u,4); /* mov.l 0x8c10df38,r1 */
 goto L8c10de6e;
L8c10de6e: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c10de70;
L8c10de70: r[0]=sx(m,r[1]+r[0],1); /* mov.b @(r0,r1),r0 */
 goto L8c10de72;
L8c10de72: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c10de74;
L8c10de74: *sr=(*sr&~1u)|!(r[0]&0x4u); /* tst #0x4,r0 */
 goto L8c10de76;
L8c10de76: if(!(*sr&1u))goto L8c10de2a; /* bf 0x8c10de2a */
 goto L8c10de78;
L8c10de78: r[2]=32u; /* mov #0x20,r2 */
 goto L8c10de7a;
L8c10de7a: wr(m,r[14]+0xcu,r[2],4); /* mov.l r2,@(0xc,r14) */
 goto L8c10de7c;
L8c10de7c: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10de7e;
L8c10de7e: *sr=(*sr&~1u)|(r[0]==0x00000068u); /* cmp/eq #0x68,r0 */
 goto L8c10de80;
L8c10de80: take=(*sr&1u);r[4]=r[0];if(take)goto L8c10de90;goto L8c10de84; /* bt/s 0x8c10de90 */
 goto L8c10de84;
L8c10de84: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10de86;
L8c10de86: *sr=(*sr&~1u)|(r[0]==0x0000006cu); /* cmp/eq #0x6c,r0 */
 goto L8c10de88;
L8c10de88: if((*sr&1u))goto L8c10de90; /* bt 0x8c10de90 */
 goto L8c10de8a;
L8c10de8a: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10de8c;
L8c10de8c: *sr=(*sr&~1u)|(r[0]==0x0000004cu); /* cmp/eq #0x4c,r0 */
 goto L8c10de8e;
L8c10de8e: if(!(*sr&1u))goto L8c10de94; /* bf 0x8c10de94 */
 goto L8c10de90;
L8c10de90: r[2]=sx(m,r[12],1);r[12]+=1; /* mov.b @r12+,r2 */
 goto L8c10de92;
L8c10de92: wr(m,r[14]+0xcu,r[2],4); /* mov.l r2,@(0xc,r14) */
 goto L8c10de94;
L8c10de94: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10de96;
L8c10de96: *sr=(*sr&~1u)|(r[0]==0x00000064u); /* cmp/eq #0x64,r0 */
 goto L8c10de98;
L8c10de98: if((*sr&1u))goto L8c10df02; /* bt 0x8c10df02 */
 goto L8c10de9a;
L8c10de9a: *sr=(*sr&~1u)|(r[0]==0x00000069u); /* cmp/eq #0x69,r0 */
 goto L8c10de9c;
L8c10de9c: if((*sr&1u))goto L8c10df02; /* bt 0x8c10df02 */
 goto L8c10de9e;
L8c10de9e: *sr=(*sr&~1u)|(r[0]==0x0000006fu); /* cmp/eq #0x6f,r0 */
 goto L8c10dea0;
L8c10dea0: if((*sr&1u))goto L8c10df02; /* bt 0x8c10df02 */
 goto L8c10dea2;
L8c10dea2: *sr=(*sr&~1u)|(r[0]==0x00000075u); /* cmp/eq #0x75,r0 */
 goto L8c10dea4;
L8c10dea4: if((*sr&1u))goto L8c10df02; /* bt 0x8c10df02 */
 goto L8c10dea6;
L8c10dea6: *sr=(*sr&~1u)|(r[0]==0x00000078u); /* cmp/eq #0x78,r0 */
 goto L8c10dea8;
L8c10dea8: if((*sr&1u))goto L8c10df02; /* bt 0x8c10df02 */
 goto L8c10deaa;
L8c10deaa: *sr=(*sr&~1u)|(r[0]==0x00000058u); /* cmp/eq #0x58,r0 */
 goto L8c10deac;
L8c10deac: if((*sr&1u))goto L8c10df02; /* bt 0x8c10df02 */
 goto L8c10deae;
L8c10deae: *sr=(*sr&~1u)|(r[0]==0x00000066u); /* cmp/eq #0x66,r0 */
 goto L8c10deb0;
L8c10deb0: if(!(*sr&1u))goto L8c10deb6; /* bf 0x8c10deb6 */
 goto L8c10deb2;
L8c10deb2: goto L8c10e040; /* bra 0x8c10e040 */
 goto L8c10deb6;
L8c10deb6: *sr=(*sr&~1u)|(r[0]==0x00000065u); /* cmp/eq #0x65,r0 */
 goto L8c10deb8;
L8c10deb8: if(!(*sr&1u))goto L8c10debe; /* bf 0x8c10debe */
 goto L8c10deba;
L8c10deba: goto L8c10e040; /* bra 0x8c10e040 */
 goto L8c10debe;
L8c10debe: *sr=(*sr&~1u)|(r[0]==0x00000045u); /* cmp/eq #0x45,r0 */
 goto L8c10dec0;
L8c10dec0: if(!(*sr&1u))goto L8c10dec6; /* bf 0x8c10dec6 */
 goto L8c10dec2;
L8c10dec2: goto L8c10e040; /* bra 0x8c10e040 */
 goto L8c10dec6;
L8c10dec6: *sr=(*sr&~1u)|(r[0]==0x00000067u); /* cmp/eq #0x67,r0 */
 goto L8c10dec8;
L8c10dec8: if(!(*sr&1u))goto L8c10dece; /* bf 0x8c10dece */
 goto L8c10deca;
L8c10deca: goto L8c10e040; /* bra 0x8c10e040 */
 goto L8c10dece;
L8c10dece: *sr=(*sr&~1u)|(r[0]==0x00000047u); /* cmp/eq #0x47,r0 */
 goto L8c10ded0;
L8c10ded0: if(!(*sr&1u))goto L8c10ded6; /* bf 0x8c10ded6 */
 goto L8c10ded2;
L8c10ded2: goto L8c10e040; /* bra 0x8c10e040 */
 goto L8c10ded6;
L8c10ded6: *sr=(*sr&~1u)|(r[0]==0x00000063u); /* cmp/eq #0x63,r0 */
 goto L8c10ded8;
L8c10ded8: if(!(*sr&1u))goto L8c10dede; /* bf 0x8c10dede */
 goto L8c10deda;
L8c10deda: goto L8c10e062; /* bra 0x8c10e062 */
 goto L8c10dede;
L8c10dede: *sr=(*sr&~1u)|(r[0]==0x00000073u); /* cmp/eq #0x73,r0 */
 goto L8c10dee0;
L8c10dee0: if(!(*sr&1u))goto L8c10dee6; /* bf 0x8c10dee6 */
 goto L8c10dee2;
L8c10dee2: goto L8c10e094; /* bra 0x8c10e094 */
 goto L8c10dee6;
L8c10dee6: *sr=(*sr&~1u)|(r[0]==0x00000070u); /* cmp/eq #0x70,r0 */
 goto L8c10dee8;
L8c10dee8: if(!(*sr&1u))goto L8c10deee; /* bf 0x8c10deee */
 goto L8c10deea;
L8c10deea: goto L8c10e0ec; /* bra 0x8c10e0ec */
 goto L8c10deee;
L8c10deee: *sr=(*sr&~1u)|(r[0]==0x0000006eu); /* cmp/eq #0x6e,r0 */
 goto L8c10def0;
L8c10def0: if(!(*sr&1u))goto L8c10def6; /* bf 0x8c10def6 */
 goto L8c10def2;
L8c10def2: goto L8c10e148; /* bra 0x8c10e148 */
 goto L8c10def6;
L8c10def6: *sr=(*sr&~1u)|(r[0]==0x00000025u); /* cmp/eq #0x25,r0 */
 goto L8c10def8;
L8c10def8: if(!(*sr&1u))goto L8c10defe; /* bf 0x8c10defe */
 goto L8c10defa;
L8c10defa: goto L8c10e17c; /* bra 0x8c10e17c */
 goto L8c10defe;
L8c10defe: goto L8c10e194; /* bra 0x8c10e194 */
 goto L8c10df02;
L8c10df02: r[0]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r0 */
 goto L8c10df04;
L8c10df04: *sr=(*sr&~1u)|(r[0]==0x0000006cu); /* cmp/eq #0x6c,r0 */
 goto L8c10df06;
L8c10df06: if(!(*sr&1u))goto L8c10df3c; /* bf 0x8c10df3c */
 goto L8c10df08;
L8c10df08: r[0]=sx(m,0x8c10df32u,2); /* mov.w 0x8c10df32,r0 */
 goto L8c10df0a;
L8c10df0a: r[6]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r6 */
 goto L8c10df0c;
L8c10df0c: r[6]+=4u; /* add #0x4,r6 */
 goto L8c10df0e;
L8c10df0e: r[4]=r[6]; /* mov r6,r4 */
 goto L8c10df10;
L8c10df10: *sr=(*sr&~1u)|!(r[13]&r[4]); /* tst r13,r4 */
 goto L8c10df12;
L8c10df12: if((*sr&1u))goto L8c10df1a; /* bt 0x8c10df1a */
 goto L8c10df14;
L8c10df14: r[0]=sx(m,0x8c10df32u,2); /* mov.w 0x8c10df32,r0 */
 goto L8c10df16;
L8c10df16: r[4]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r4 */
 goto L8c10df18;
L8c10df18: r[4]+=8u; /* add #0x8,r4 */
 goto L8c10df1a;
L8c10df1a: r[0]=sx(m,0x8c10df32u,2); /* mov.w 0x8c10df32,r0 */
 goto L8c10df1c;
L8c10df1c: r[5]=r[4]; /* mov r4,r5 */
 goto L8c10df1e;
L8c10df1e: r[5]+=4294967292u; /* add #-0x4,r5 */
 goto L8c10df20;
L8c10df20: wr(m,r[15]+r[0],r[4],4); /* mov.l r4,@(r0,r15) */
 goto L8c10df22;
L8c10df22: r[4]=r[5]; /* mov r5,r4 */
 goto L8c10df24;
L8c10df24: *sr=(*sr&~1u)|!(r[13]&r[4]); /* tst r13,r4 */
 goto L8c10df26;
L8c10df26: if((*sr&1u))goto L8c10df2e; /* bt 0x8c10df2e */
 goto L8c10df28;
L8c10df28: r[0]=sx(m,0x8c10df32u,2); /* mov.w 0x8c10df32,r0 */
 goto L8c10df2a;
L8c10df2a: r[4]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r4 */
 goto L8c10df2c;
L8c10df2c: r[4]+=4294967288u; /* add #-0x8,r4 */
 goto L8c10df2e;
L8c10df2e: r[4]=rd(m,r[4],4);goto L8c10e022; /* bra 0x8c10e022 */
 goto L8c10df3c;
L8c10df3c: *sr=(*sr&~1u)|(r[0]==0x00000068u); /* cmp/eq #0x68,r0 */
 goto L8c10df3e;
L8c10df3e: if(!(*sr&1u))goto L8c10dfb2; /* bf 0x8c10dfb2 */
 goto L8c10df40;
L8c10df40: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10df42;
L8c10df42: *sr=(*sr&~1u)|(r[0]==0x00000075u); /* cmp/eq #0x75,r0 */
 goto L8c10df44;
L8c10df44: take=(*sr&1u);r[4]=r[0];if(take)goto L8c10df5a;goto L8c10df48; /* bt/s 0x8c10df5a */
 goto L8c10df48;
L8c10df48: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10df4a;
L8c10df4a: *sr=(*sr&~1u)|(r[0]==0x00000058u); /* cmp/eq #0x58,r0 */
 goto L8c10df4c;
L8c10df4c: if((*sr&1u))goto L8c10df5a; /* bt 0x8c10df5a */
 goto L8c10df4e;
L8c10df4e: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10df50;
L8c10df50: *sr=(*sr&~1u)|(r[0]==0x00000078u); /* cmp/eq #0x78,r0 */
 goto L8c10df52;
L8c10df52: if((*sr&1u))goto L8c10df5a; /* bt 0x8c10df5a */
 goto L8c10df54;
L8c10df54: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10df56;
L8c10df56: *sr=(*sr&~1u)|(r[0]==0x0000006fu); /* cmp/eq #0x6f,r0 */
 goto L8c10df58;
L8c10df58: if(!(*sr&1u))goto L8c10df86; /* bf 0x8c10df86 */
 goto L8c10df5a;
L8c10df5a: r[0]=sx(m,0x8c10dff8u,2); /* mov.w 0x8c10dff8,r0 */
 goto L8c10df5c;
L8c10df5c: r[5]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r5 */
 goto L8c10df5e;
L8c10df5e: r[5]+=4u; /* add #0x4,r5 */
 goto L8c10df60;
L8c10df60: r[4]=r[5]; /* mov r5,r4 */
 goto L8c10df62;
L8c10df62: *sr=(*sr&~1u)|!(r[13]&r[4]); /* tst r13,r4 */
 goto L8c10df64;
L8c10df64: if((*sr&1u))goto L8c10df6c; /* bt 0x8c10df6c */
 goto L8c10df66;
L8c10df66: r[0]=sx(m,0x8c10dff8u,2); /* mov.w 0x8c10dff8,r0 */
 goto L8c10df68;
L8c10df68: r[4]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r4 */
 goto L8c10df6a;
L8c10df6a: r[4]+=8u; /* add #0x8,r4 */
 goto L8c10df6c;
L8c10df6c: r[0]=sx(m,0x8c10dff8u,2); /* mov.w 0x8c10dff8,r0 */
 goto L8c10df6e;
L8c10df6e: r[6]=r[4]; /* mov r4,r6 */
 goto L8c10df70;
L8c10df70: r[6]+=4294967292u; /* add #-0x4,r6 */
 goto L8c10df72;
L8c10df72: wr(m,r[15]+r[0],r[4],4); /* mov.l r4,@(r0,r15) */
 goto L8c10df74;
L8c10df74: r[4]=r[6]; /* mov r6,r4 */
 goto L8c10df76;
L8c10df76: *sr=(*sr&~1u)|!(r[13]&r[4]); /* tst r13,r4 */
 goto L8c10df78;
L8c10df78: if((*sr&1u))goto L8c10df80; /* bt 0x8c10df80 */
 goto L8c10df7a;
L8c10df7a: r[0]=sx(m,0x8c10dff8u,2); /* mov.w 0x8c10dff8,r0 */
 goto L8c10df7c;
L8c10df7c: r[4]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r4 */
 goto L8c10df7e;
L8c10df7e: r[4]+=4294967288u; /* add #-0x8,r4 */
 goto L8c10df80;
L8c10df80: r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c10df82;
L8c10df82: r[4]=r[4]&65535u;goto L8c10e022; /* bra 0x8c10e022 */
 goto L8c10df86;
L8c10df86: r[0]=sx(m,0x8c10dff8u,2); /* mov.w 0x8c10dff8,r0 */
 goto L8c10df88;
L8c10df88: r[5]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r5 */
 goto L8c10df8a;
L8c10df8a: r[5]+=4u; /* add #0x4,r5 */
 goto L8c10df8c;
L8c10df8c: r[4]=r[5]; /* mov r5,r4 */
 goto L8c10df8e;
L8c10df8e: *sr=(*sr&~1u)|!(r[13]&r[4]); /* tst r13,r4 */
 goto L8c10df90;
L8c10df90: if((*sr&1u))goto L8c10df98; /* bt 0x8c10df98 */
 goto L8c10df92;
L8c10df92: r[0]=sx(m,0x8c10dff8u,2); /* mov.w 0x8c10dff8,r0 */
 goto L8c10df94;
L8c10df94: r[4]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r4 */
 goto L8c10df96;
L8c10df96: r[4]+=8u; /* add #0x8,r4 */
 goto L8c10df98;
L8c10df98: r[0]=sx(m,0x8c10dff8u,2); /* mov.w 0x8c10dff8,r0 */
 goto L8c10df9a;
L8c10df9a: r[6]=r[4]; /* mov r4,r6 */
 goto L8c10df9c;
L8c10df9c: r[6]+=4294967292u; /* add #-0x4,r6 */
 goto L8c10df9e;
L8c10df9e: wr(m,r[15]+r[0],r[4],4); /* mov.l r4,@(r0,r15) */
 goto L8c10dfa0;
L8c10dfa0: r[4]=r[6]; /* mov r6,r4 */
 goto L8c10dfa2;
L8c10dfa2: *sr=(*sr&~1u)|!(r[13]&r[4]); /* tst r13,r4 */
 goto L8c10dfa4;
L8c10dfa4: if((*sr&1u))goto L8c10dfac; /* bt 0x8c10dfac */
 goto L8c10dfa6;
L8c10dfa6: r[0]=sx(m,0x8c10dff8u,2); /* mov.w 0x8c10dff8,r0 */
 goto L8c10dfa8;
L8c10dfa8: r[4]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r4 */
 goto L8c10dfaa;
L8c10dfaa: r[4]+=4294967288u; /* add #-0x8,r4 */
 goto L8c10dfac;
L8c10dfac: r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c10dfae;
L8c10dfae: r[4]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c10e022; /* bra 0x8c10e022 */
 goto L8c10dfb2;
L8c10dfb2: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10dfb4;
L8c10dfb4: *sr=(*sr&~1u)|(r[0]==0x00000075u); /* cmp/eq #0x75,r0 */
 goto L8c10dfb6;
L8c10dfb6: take=(*sr&1u);r[4]=r[0];if(take)goto L8c10dfcc;goto L8c10dfba; /* bt/s 0x8c10dfcc */
 goto L8c10dfba;
L8c10dfba: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10dfbc;
L8c10dfbc: *sr=(*sr&~1u)|(r[0]==0x00000058u); /* cmp/eq #0x58,r0 */
 goto L8c10dfbe;
L8c10dfbe: if((*sr&1u))goto L8c10dfcc; /* bt 0x8c10dfcc */
 goto L8c10dfc0;
L8c10dfc0: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10dfc2;
L8c10dfc2: *sr=(*sr&~1u)|(r[0]==0x00000078u); /* cmp/eq #0x78,r0 */
 goto L8c10dfc4;
L8c10dfc4: if((*sr&1u))goto L8c10dfcc; /* bt 0x8c10dfcc */
 goto L8c10dfc6;
L8c10dfc6: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10dfc8;
L8c10dfc8: *sr=(*sr&~1u)|(r[0]==0x0000006fu); /* cmp/eq #0x6f,r0 */
 goto L8c10dfca;
L8c10dfca: if(!(*sr&1u))goto L8c10dffa; /* bf 0x8c10dffa */
 goto L8c10dfcc;
L8c10dfcc: r[0]=sx(m,0x8c10dff8u,2); /* mov.w 0x8c10dff8,r0 */
 goto L8c10dfce;
L8c10dfce: r[5]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r5 */
 goto L8c10dfd0;
L8c10dfd0: r[5]+=4u; /* add #0x4,r5 */
 goto L8c10dfd2;
L8c10dfd2: r[4]=r[5]; /* mov r5,r4 */
 goto L8c10dfd4;
L8c10dfd4: *sr=(*sr&~1u)|!(r[13]&r[4]); /* tst r13,r4 */
 goto L8c10dfd6;
L8c10dfd6: if((*sr&1u))goto L8c10dfde; /* bt 0x8c10dfde */
 goto L8c10dfd8;
L8c10dfd8: r[0]=sx(m,0x8c10dff8u,2); /* mov.w 0x8c10dff8,r0 */
 goto L8c10dfda;
L8c10dfda: r[4]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r4 */
 goto L8c10dfdc;
L8c10dfdc: r[4]+=8u; /* add #0x8,r4 */
 goto L8c10dfde;
L8c10dfde: r[0]=sx(m,0x8c10dff8u,2); /* mov.w 0x8c10dff8,r0 */
 goto L8c10dfe0;
L8c10dfe0: r[6]=r[4]; /* mov r4,r6 */
 goto L8c10dfe2;
L8c10dfe2: r[6]+=4294967292u; /* add #-0x4,r6 */
 goto L8c10dfe4;
L8c10dfe4: wr(m,r[15]+r[0],r[4],4); /* mov.l r4,@(r0,r15) */
 goto L8c10dfe6;
L8c10dfe6: r[4]=r[6]; /* mov r6,r4 */
 goto L8c10dfe8;
L8c10dfe8: *sr=(*sr&~1u)|!(r[13]&r[4]); /* tst r13,r4 */
 goto L8c10dfea;
L8c10dfea: if((*sr&1u))goto L8c10e020; /* bt 0x8c10e020 */
 goto L8c10dfec;
L8c10dfec: r[0]=sx(m,0x8c10dff8u,2); /* mov.w 0x8c10dff8,r0 */
 goto L8c10dfee;
L8c10dfee: r[4]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r4 */
 goto L8c10dff0;
L8c10dff0: r[4]+=4294967288u;goto L8c10e020; /* bra 0x8c10e020 */
 goto L8c10dffa;
L8c10dffa: r[0]=sx(m,0x8c10e100u,2); /* mov.w 0x8c10e100,r0 */
 goto L8c10dffc;
L8c10dffc: r[5]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r5 */
 goto L8c10dffe;
L8c10dffe: r[5]+=4u; /* add #0x4,r5 */
 goto L8c10e000;
L8c10e000: r[4]=r[5]; /* mov r5,r4 */
 goto L8c10e002;
L8c10e002: *sr=(*sr&~1u)|!(r[13]&r[4]); /* tst r13,r4 */
 goto L8c10e004;
L8c10e004: if((*sr&1u))goto L8c10e00c; /* bt 0x8c10e00c */
 goto L8c10e006;
L8c10e006: r[0]=sx(m,0x8c10e100u,2); /* mov.w 0x8c10e100,r0 */
 goto L8c10e008;
L8c10e008: r[4]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r4 */
 goto L8c10e00a;
L8c10e00a: r[4]+=8u; /* add #0x8,r4 */
 goto L8c10e00c;
L8c10e00c: r[0]=sx(m,0x8c10e100u,2); /* mov.w 0x8c10e100,r0 */
 goto L8c10e00e;
L8c10e00e: r[6]=r[4]; /* mov r4,r6 */
 goto L8c10e010;
L8c10e010: r[6]+=4294967292u; /* add #-0x4,r6 */
 goto L8c10e012;
L8c10e012: wr(m,r[15]+r[0],r[4],4); /* mov.l r4,@(r0,r15) */
 goto L8c10e014;
L8c10e014: r[4]=r[6]; /* mov r6,r4 */
 goto L8c10e016;
L8c10e016: *sr=(*sr&~1u)|!(r[13]&r[4]); /* tst r13,r4 */
 goto L8c10e018;
L8c10e018: if((*sr&1u))goto L8c10e020; /* bt 0x8c10e020 */
 goto L8c10e01a;
L8c10e01a: r[0]=sx(m,0x8c10e100u,2); /* mov.w 0x8c10e100,r0 */
 goto L8c10e01c;
L8c10e01c: r[4]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r4 */
 goto L8c10e01e;
L8c10e01e: r[4]+=4294967288u; /* add #-0x8,r4 */
 goto L8c10e020;
L8c10e020: r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c10e022;
L8c10e022: r[0]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r0 */
 goto L8c10e024;
L8c10e024: *sr=(*sr&~1u)|(r[0]==0xffffffffu); /* cmp/eq #0xff,r0 */
 goto L8c10e026;
L8c10e026: take=!(*sr&1u);r[7]=r[14];if(take)goto L8c10e02e;goto L8c10e02a; /* bf/s 0x8c10e02e */
 goto L8c10e02a;
L8c10e02a: r[3]=1u; /* mov #0x1,r3 */
 goto L8c10e02c;
L8c10e02c: wr(m,r[14]+0x8u,r[3],4); /* mov.l r3,@(0x8,r14) */
 goto L8c10e02e;
L8c10e02e: r[6]=sx(m,r[12],1); /* mov.b @r12,r6 */
 goto L8c10e030;
L8c10e030: *pr=0x8c10e034u;r[5]=r[9];status=call(ctx,0x8c10e304u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c10e304 */
 goto L8c10e034;
L8c10e034: r[3]=rd(m,0x8c10e104u,4); /* mov.l 0x8c10e104,r3 */
 goto L8c10e036;
L8c10e036: wr(m,r[15]+0x4u,r[9],4); /* mov.l r9,@(0x4,r15) */
 goto L8c10e038;
L8c10e038: target=r[3];*pr=0x8c10e03cu;r[4]=r[9];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10e03c;
L8c10e03c: goto L8c10e144; /* bra 0x8c10e144 */
 goto L8c10e040;
L8c10e040: r[4]=sx(m,0x8c10e100u,2); /* mov.w 0x8c10e100,r4 */
 goto L8c10e042;
L8c10e042: r[7]=r[14]; /* mov r14,r7 */
 goto L8c10e044;
L8c10e044: r[6]=sx(m,r[12],1); /* mov.b @r12,r6 */
 goto L8c10e046;
L8c10e046: r[5]=r[9]; /* mov r9,r5 */
 goto L8c10e048;
L8c10e048: r[2]=rd(m,0x8c10e108u,4); /* mov.l 0x8c10e108,r2 */
 goto L8c10e04a;
L8c10e04a: target=r[2];*pr=0x8c10e04eu;r[4]+=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c10e04e;
L8c10e04e: r[4]=f->fpscr; /* sts FPSCR,r4 */
 goto L8c10e050;
L8c10e050: r[3]=rd(m,0x8c10e10cu,4); /* mov.l 0x8c10e10c,r3 */
 goto L8c10e052;
L8c10e052: r[2]=rd(m,0x8c10e104u,4); /* mov.l 0x8c10e104,r2 */
 goto L8c10e054;
L8c10e054: r[4]&=r[3]; /* and r3,r4 */
 goto L8c10e056;
L8c10e056: wr(m,r[15]+0x4u,r[9],4); /* mov.l r9,@(0x4,r15) */
 goto L8c10e058;
L8c10e058: f->fpscr=r[4];fr=f->bank[(f->fpscr>>21)&1u]; /* lds r4,FPSCR */
 goto L8c10e05a;
L8c10e05a: target=r[2];*pr=0x8c10e05eu;r[4]=r[9];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c10e05e;
L8c10e05e: goto L8c10e144; /* bra 0x8c10e144 */
 goto L8c10e062;
L8c10e062: r[0]=sx(m,0x8c10e100u,2); /* mov.w 0x8c10e100,r0 */
 goto L8c10e064;
L8c10e064: r[2]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r2 */
 goto L8c10e066;
L8c10e066: r[4]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r4 */
 goto L8c10e068;
L8c10e068: r[4]+=4u; /* add #0x4,r4 */
 goto L8c10e06a;
L8c10e06a: r[3]=r[4]; /* mov r4,r3 */
 goto L8c10e06c;
L8c10e06c: *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c10e06e;
L8c10e06e: if((*sr&1u))goto L8c10e076; /* bt 0x8c10e076 */
 goto L8c10e070;
L8c10e070: r[0]=sx(m,0x8c10e100u,2); /* mov.w 0x8c10e100,r0 */
 goto L8c10e072;
L8c10e072: r[3]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r3 */
 goto L8c10e074;
L8c10e074: r[3]+=8u; /* add #0x8,r3 */
 goto L8c10e076;
L8c10e076: r[0]=sx(m,0x8c10e100u,2); /* mov.w 0x8c10e100,r0 */
 goto L8c10e078;
L8c10e078: r[5]=r[3]; /* mov r3,r5 */
 goto L8c10e07a;
L8c10e07a: r[5]+=4294967292u; /* add #-0x4,r5 */
 goto L8c10e07c;
L8c10e07c: wr(m,r[15]+r[0],r[3],4); /* mov.l r3,@(r0,r15) */
 goto L8c10e07e;
L8c10e07e: r[0]=r[5]; /* mov r5,r0 */
 goto L8c10e080;
L8c10e080: *sr=(*sr&~1u)|!(r[13]&r[0]); /* tst r13,r0 */
 goto L8c10e082;
L8c10e082: if((*sr&1u))goto L8c10e08a; /* bt 0x8c10e08a */
 goto L8c10e084;
L8c10e084: r[1]=r[3]; /* mov r3,r1 */
 goto L8c10e086;
L8c10e086: r[1]+=4294967288u;goto L8c10e08c; /* bra 0x8c10e08c */
 goto L8c10e08a;
L8c10e08a: r[1]=r[5]; /* mov r5,r1 */
 goto L8c10e08c;
L8c10e08c: r[0]=rd(m,r[1],4); /* mov.l @r1,r0 */
 goto L8c10e08e;
L8c10e08e: wr(m,r[2],r[0],1); /* mov.b r0,@r2 */
 goto L8c10e090;
L8c10e090: goto L8c10e182; /* bra 0x8c10e182 */
 goto L8c10e094;
L8c10e094: r[0]=sx(m,0x8c10e100u,2); /* mov.w 0x8c10e100,r0 */
 goto L8c10e096;
L8c10e096: r[4]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r4 */
 goto L8c10e098;
L8c10e098: r[4]+=4u; /* add #0x4,r4 */
 goto L8c10e09a;
L8c10e09a: r[3]=r[4]; /* mov r4,r3 */
 goto L8c10e09c;
L8c10e09c: *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c10e09e;
L8c10e09e: if((*sr&1u))goto L8c10e0a8; /* bt 0x8c10e0a8 */
 goto L8c10e0a0;
L8c10e0a0: r[0]=sx(m,0x8c10e100u,2); /* mov.w 0x8c10e100,r0 */
 goto L8c10e0a2;
L8c10e0a2: r[1]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r1 */
 goto L8c10e0a4;
L8c10e0a4: r[1]+=8u;goto L8c10e0aa; /* bra 0x8c10e0aa */
 goto L8c10e0a8;
L8c10e0a8: r[1]=r[4]; /* mov r4,r1 */
 goto L8c10e0aa;
L8c10e0aa: r[5]=r[1]; /* mov r1,r5 */
 goto L8c10e0ac;
L8c10e0ac: r[5]+=4294967292u; /* add #-0x4,r5 */
 goto L8c10e0ae;
L8c10e0ae: r[2]=r[5]; /* mov r5,r2 */
 goto L8c10e0b0;
L8c10e0b0: r[0]=sx(m,0x8c10e100u,2); /* mov.w 0x8c10e100,r0 */
 goto L8c10e0b2;
L8c10e0b2: *sr=(*sr&~1u)|!(r[13]&r[2]); /* tst r13,r2 */
 goto L8c10e0b4;
L8c10e0b4: take=(*sr&1u);wr(m,r[15]+r[0],r[1],4);if(take)goto L8c10e0be;goto L8c10e0b8; /* bt/s 0x8c10e0be */
 goto L8c10e0b8;
L8c10e0b8: r[3]=r[1]; /* mov r1,r3 */
 goto L8c10e0ba;
L8c10e0ba: r[3]+=4294967288u;goto L8c10e0c0; /* bra 0x8c10e0c0 */
 goto L8c10e0be;
L8c10e0be: r[3]=r[5]; /* mov r5,r3 */
 goto L8c10e0c0;
L8c10e0c0: r[1]=rd(m,r[3],4); /* mov.l @r3,r1 */
 goto L8c10e0c2;
L8c10e0c2: r[3]=rd(m,0x8c10e104u,4); /* mov.l 0x8c10e104,r3 */
 goto L8c10e0c4;
L8c10e0c4: wr(m,r[15]+0x4u,r[1],4); /* mov.l r1,@(0x4,r15) */
 goto L8c10e0c6;
L8c10e0c6: target=r[3];*pr=0x8c10e0cau;r[4]=r[1];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10e0ca;
L8c10e0ca: wr(m,r[15]+0xcu,r[0],4); /* mov.l r0,@(0xc,r15) */
 goto L8c10e0cc;
L8c10e0cc: r[0]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r0 */
 goto L8c10e0ce;
L8c10e0ce: *sr=(*sr&~1u)|(r[0]==0xffffffffu); /* cmp/eq #0xff,r0 */
 goto L8c10e0d0;
L8c10e0d0: take=(*sr&1u);r[4]=r[0];if(take)goto L8c10e0de;goto L8c10e0d4; /* bt/s 0x8c10e0de */
 goto L8c10e0d4;
L8c10e0d4: r[2]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r2 */
 goto L8c10e0d6;
L8c10e0d6: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]); /* cmp/ge r2,r4 */
 goto L8c10e0d8;
L8c10e0d8: if((*sr&1u))goto L8c10e0de; /* bt 0x8c10e0de */
 goto L8c10e0da;
L8c10e0da: r[1]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r1 */
 goto L8c10e0dc;
L8c10e0dc: wr(m,r[15]+0xcu,r[1],4); /* mov.l r1,@(0xc,r15) */
 goto L8c10e0de;
L8c10e0de: wr(m,r[14]+0x14u,r[9],4); /* mov.l r9,@(0x14,r14) */
 goto L8c10e0e0;
L8c10e0e0: wr(m,r[14]+0x20u,r[11],4); /* mov.l r11,@(0x20,r14) */
 goto L8c10e0e2;
L8c10e0e2: r[3]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r3 */
 goto L8c10e0e4;
L8c10e0e4: r[2]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r2 */
 goto L8c10e0e6;
L8c10e0e6: r[2]-=r[3]; /* sub r3,r2 */
 goto L8c10e0e8;
L8c10e0e8: wr(m,r[14]+0x28u,r[2],4);goto L8c10e19e; /* bra 0x8c10e19e */
 goto L8c10e0ec;
L8c10e0ec: r[0]=sx(m,0x8c10e100u,2); /* mov.w 0x8c10e100,r0 */
 goto L8c10e0ee;
L8c10e0ee: r[5]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r5 */
 goto L8c10e0f0;
L8c10e0f0: r[5]+=4u; /* add #0x4,r5 */
 goto L8c10e0f2;
L8c10e0f2: r[3]=r[5]; /* mov r5,r3 */
 goto L8c10e0f4;
L8c10e0f4: *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c10e0f6;
L8c10e0f6: if((*sr&1u))goto L8c10e110; /* bt 0x8c10e110 */
 goto L8c10e0f8;
L8c10e0f8: r[0]=sx(m,0x8c10e100u,2); /* mov.w 0x8c10e100,r0 */
 goto L8c10e0fa;
L8c10e0fa: r[1]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r1 */
 goto L8c10e0fc;
L8c10e0fc: r[1]+=8u;goto L8c10e112; /* bra 0x8c10e112 */
 goto L8c10e110;
L8c10e110: r[1]=r[5]; /* mov r5,r1 */
 goto L8c10e112;
L8c10e112: r[4]=r[1]; /* mov r1,r4 */
 goto L8c10e114;
L8c10e114: r[4]+=4294967292u; /* add #-0x4,r4 */
 goto L8c10e116;
L8c10e116: r[2]=r[4]; /* mov r4,r2 */
 goto L8c10e118;
L8c10e118: r[0]=sx(m,0x8c10e208u,2); /* mov.w 0x8c10e208,r0 */
 goto L8c10e11a;
L8c10e11a: *sr=(*sr&~1u)|!(r[13]&r[2]); /* tst r13,r2 */
 goto L8c10e11c;
L8c10e11c: take=(*sr&1u);wr(m,r[15]+r[0],r[1],4);if(take)goto L8c10e126;goto L8c10e120; /* bt/s 0x8c10e126 */
 goto L8c10e120;
L8c10e120: r[3]=r[1]; /* mov r1,r3 */
 goto L8c10e122;
L8c10e122: r[3]+=4294967288u;goto L8c10e128; /* bra 0x8c10e128 */
 goto L8c10e126;
L8c10e126: r[3]=r[4]; /* mov r4,r3 */
 goto L8c10e128;
L8c10e128: r[0]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r0 */
 goto L8c10e12a;
L8c10e12a: r[4]=rd(m,r[3],4); /* mov.l @r3,r4 */
 goto L8c10e12c;
L8c10e12c: *sr=(*sr&~1u)|(r[0]==0xffffffffu); /* cmp/eq #0xff,r0 */
 goto L8c10e12e;
L8c10e12e: take=!(*sr&1u);r[7]=r[14];if(take)goto L8c10e136;goto L8c10e132; /* bf/s 0x8c10e136 */
 goto L8c10e132;
L8c10e132: r[2]=1u; /* mov #0x1,r2 */
 goto L8c10e134;
L8c10e134: wr(m,r[14]+0x8u,r[2],4); /* mov.l r2,@(0x8,r14) */
 goto L8c10e136;
L8c10e136: r[6]=120u; /* mov #0x78,r6 */
 goto L8c10e138;
L8c10e138: *pr=0x8c10e13cu;r[5]=r[9];status=call(ctx,0x8c10e304u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c10e304 */
 goto L8c10e13c;
L8c10e13c: r[3]=rd(m,0x8c10e20cu,4); /* mov.l 0x8c10e20c,r3 */
 goto L8c10e13e;
L8c10e13e: wr(m,r[15]+0x4u,r[9],4); /* mov.l r9,@(0x4,r15) */
 goto L8c10e140;
L8c10e140: target=r[3];*pr=0x8c10e144u;r[4]=r[9];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10e144;
L8c10e144: wr(m,r[15]+0xcu,r[0],4);goto L8c10e19e; /* bra 0x8c10e19e */
 goto L8c10e148;
L8c10e148: r[0]=sx(m,0x8c10e208u,2); /* mov.w 0x8c10e208,r0 */
 goto L8c10e14a;
L8c10e14a: r[4]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r4 */
 goto L8c10e14c;
L8c10e14c: r[4]+=4u; /* add #0x4,r4 */
 goto L8c10e14e;
L8c10e14e: r[3]=r[4]; /* mov r4,r3 */
 goto L8c10e150;
L8c10e150: *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c10e152;
L8c10e152: if((*sr&1u))goto L8c10e15c; /* bt 0x8c10e15c */
 goto L8c10e154;
L8c10e154: r[0]=sx(m,0x8c10e208u,2); /* mov.w 0x8c10e208,r0 */
 goto L8c10e156;
L8c10e156: r[2]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r2 */
 goto L8c10e158;
L8c10e158: r[2]+=8u;goto L8c10e15e; /* bra 0x8c10e15e */
 goto L8c10e15c;
L8c10e15c: r[2]=r[4]; /* mov r4,r2 */
 goto L8c10e15e;
L8c10e15e: r[5]=r[2]; /* mov r2,r5 */
 goto L8c10e160;
L8c10e160: r[5]+=4294967292u; /* add #-0x4,r5 */
 goto L8c10e162;
L8c10e162: r[1]=r[5]; /* mov r5,r1 */
 goto L8c10e164;
L8c10e164: r[0]=sx(m,0x8c10e208u,2); /* mov.w 0x8c10e208,r0 */
 goto L8c10e166;
L8c10e166: *sr=(*sr&~1u)|!(r[13]&r[1]); /* tst r13,r1 */
 goto L8c10e168;
L8c10e168: take=(*sr&1u);wr(m,r[15]+r[0],r[2],4);if(take)goto L8c10e172;goto L8c10e16c; /* bt/s 0x8c10e172 */
 goto L8c10e16c;
L8c10e16c: r[3]=r[2]; /* mov r2,r3 */
 goto L8c10e16e;
L8c10e16e: r[3]+=4294967288u;goto L8c10e174; /* bra 0x8c10e174 */
 goto L8c10e172;
L8c10e172: r[3]=r[5]; /* mov r5,r3 */
 goto L8c10e174;
L8c10e174: r[1]=rd(m,r[10]+0x8u,4); /* mov.l @(0x8,r10),r1 */
 goto L8c10e176;
L8c10e176: r[4]=rd(m,r[3],4); /* mov.l @r3,r4 */
 goto L8c10e178;
L8c10e178: wr(m,r[4],r[1],4);goto L8c10e19e; /* bra 0x8c10e19e */
 goto L8c10e17c;
L8c10e17c: r[2]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r2 */
 goto L8c10e17e;
L8c10e17e: r[3]=37u; /* mov #0x25,r3 */
 goto L8c10e180;
L8c10e180: wr(m,r[2],r[3],1); /* mov.b r3,@r2 */
 goto L8c10e182;
L8c10e182: r[3]=1u; /* mov #0x1,r3 */
 goto L8c10e184;
L8c10e184: wr(m,r[15]+0x4u,r[9],4); /* mov.l r9,@(0x4,r15) */
 goto L8c10e186;
L8c10e186: wr(m,r[15]+0xcu,r[3],4); /* mov.l r3,@(0xc,r15) */
 goto L8c10e188;
L8c10e188: wr(m,r[14]+0x14u,r[9],4); /* mov.l r9,@(0x14,r14) */
 goto L8c10e18a;
L8c10e18a: wr(m,r[14]+0x20u,r[11],4); /* mov.l r11,@(0x20,r14) */
 goto L8c10e18c;
L8c10e18c: r[3]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r3 */
 goto L8c10e18e;
L8c10e18e: r[3]+=4294967295u; /* add #-0x1,r3 */
 goto L8c10e190;
L8c10e190: wr(m,r[14]+0x28u,r[3],4);goto L8c10e19e; /* bra 0x8c10e19e */
 goto L8c10e194;
L8c10e194: r[1]=rd(m,0x8c10e210u,4); /* mov.l 0x8c10e210,r1 */
 goto L8c10e196;
L8c10e196: wr(m,r[1],r[8],4); /* mov.l r8,@r1 */
 goto L8c10e198;
L8c10e198: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c10e19a;
L8c10e19a: r[0]|=0x1u; /* or #0x1,r0 */
 goto L8c10e19c;
L8c10e19c: wr(m,r[14],r[0],1); /* mov.b r0,@r14 */
 goto L8c10e19e;
L8c10e19e: r[0]=sx(m,r[12],1); /* mov.b @r12,r0 */
 goto L8c10e1a0;
L8c10e1a0: *sr=(*sr&~1u)|(r[0]==0x0000006eu); /* cmp/eq #0x6e,r0 */
 goto L8c10e1a2;
L8c10e1a2: if(!(*sr&1u))goto L8c10e1a8; /* bf 0x8c10e1a8 */
 goto L8c10e1a4;
L8c10e1a4: goto L8c10e2a6; /* bra 0x8c10e2a6 */
 goto L8c10e1a8;
L8c10e1a8: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c10e1aa;
L8c10e1aa: *sr=(*sr&~1u)|!(r[0]&0x1u); /* tst #0x1,r0 */
 goto L8c10e1ac;
L8c10e1ac: if(!(*sr&1u))goto L8c10e2a6; /* bf 0x8c10e2a6 */
 goto L8c10e1ae;
L8c10e1ae: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c10e1b0;
L8c10e1b0: *sr=(*sr&~1u)|!(r[0]&0x2u); /* tst #0x2,r0 */
 goto L8c10e1b2;
L8c10e1b2: if(!(*sr&1u))goto L8c10e1d4; /* bf 0x8c10e1d4 */
 goto L8c10e1b4;
L8c10e1b4: r[2]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r2 */
 goto L8c10e1b6;
L8c10e1b6: *sr=(*sr&~1u)|((int32_t)r[2]>0); /* cmp/pl r2 */
 goto L8c10e1b8;
L8c10e1b8: if(!(*sr&1u))goto L8c10e1d4; /* bf 0x8c10e1d4 */
 goto L8c10e1ba;
L8c10e1ba: goto L8c10e1ce; /* bra 0x8c10e1ce */
 goto L8c10e1be;
L8c10e1be: r[4]=r[14]; /* mov r14,r4 */
 goto L8c10e1c0;
L8c10e1c0: r[5]=1u; /* mov #0x1,r5 */
 goto L8c10e1c2;
L8c10e1c2: r[6]=r[10]; /* mov r10,r6 */
 goto L8c10e1c4;
L8c10e1c4: *pr=0x8c10e1c8u;r[4]+=1u;status=call(ctx,0x8c10e50eu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c10e50e */
 goto L8c10e1c8;
L8c10e1c8: r[2]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r2 */
 goto L8c10e1ca;
L8c10e1ca: r[2]+=4294967295u; /* add #-0x1,r2 */
 goto L8c10e1cc;
L8c10e1cc: wr(m,r[14]+0x28u,r[2],4); /* mov.l r2,@(0x28,r14) */
 goto L8c10e1ce;
L8c10e1ce: r[3]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r3 */
 goto L8c10e1d0;
L8c10e1d0: *sr=(*sr&~1u)|((int32_t)r[3]>0); /* cmp/pl r3 */
 goto L8c10e1d2;
L8c10e1d2: if((*sr&1u))goto L8c10e1be; /* bt 0x8c10e1be */
 goto L8c10e1d4;
L8c10e1d4: r[1]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r1 */
 goto L8c10e1d6;
L8c10e1d6: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c10e1d8;
L8c10e1d8: if(!(*sr&1u))goto L8c10e1f2; /* bf 0x8c10e1f2 */
 goto L8c10e1da;
L8c10e1da: r[2]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r2 */
 goto L8c10e1dc;
L8c10e1dc: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c10e1de;
L8c10e1de: if(!(*sr&1u))goto L8c10e1f2; /* bf 0x8c10e1f2 */
 goto L8c10e1e0;
L8c10e1e0: r[2]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r2 */
 goto L8c10e1e2;
L8c10e1e2: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c10e1e4;
L8c10e1e4: if(!(*sr&1u))goto L8c10e1f2; /* bf 0x8c10e1f2 */
 goto L8c10e1e6;
L8c10e1e6: r[5]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r5 */
 goto L8c10e1e8;
L8c10e1e8: r[6]=r[10]; /* mov r10,r6 */
 goto L8c10e1ea;
L8c10e1ea: *pr=0x8c10e1eeu;r[4]=rd(m,r[15]+0x4u,4);status=call(ctx,0x8c10e50eu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c10e50e */
 goto L8c10e1ee;
L8c10e1ee: goto L8c10e280; /* bra 0x8c10e280 */
 goto L8c10e1f2;
L8c10e1f2: r[5]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r5 */
 goto L8c10e1f4;
L8c10e1f4: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c10e1f6;
L8c10e1f6: r[5]-=r[3]; /* sub r3,r5 */
 goto L8c10e1f8;
L8c10e1f8: *sr=(*sr&~1u)|((int32_t)r[5]>0); /* cmp/pl r5 */
 goto L8c10e1fa;
L8c10e1fa: take=!(*sr&1u);wr(m,r[15],r[5],4);if(take)goto L8c10e224;goto L8c10e1fe; /* bf/s 0x8c10e224 */
 goto L8c10e1fe;
L8c10e1fe: r[6]=r[10]; /* mov r10,r6 */
 goto L8c10e200;
L8c10e200: *pr=0x8c10e204u;r[4]=r[3];status=call(ctx,0x8c10e50eu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c10e50e */
 goto L8c10e204;
L8c10e204: goto L8c10e224; /* bra 0x8c10e224 */
 goto L8c10e214;
L8c10e214: r[4]=r[15]; /* mov r15,r4 */
 goto L8c10e216;
L8c10e216: r[5]=1u; /* mov #0x1,r5 */
 goto L8c10e218;
L8c10e218: r[6]=r[10]; /* mov r10,r6 */
 goto L8c10e21a;
L8c10e21a: *pr=0x8c10e21eu;r[4]+=8u;status=call(ctx,0x8c10e50eu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c10e50e */
 goto L8c10e21e;
L8c10e21e: r[2]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r2 */
 goto L8c10e220;
L8c10e220: r[2]+=4294967295u; /* add #-0x1,r2 */
 goto L8c10e222;
L8c10e222: wr(m,r[14]+0x1cu,r[2],4); /* mov.l r2,@(0x1c,r14) */
 goto L8c10e224;
L8c10e224: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c10e226;
L8c10e226: *sr=(*sr&~1u)|((int32_t)r[3]>0); /* cmp/pl r3 */
 goto L8c10e228;
L8c10e228: if((*sr&1u))goto L8c10e214; /* bt 0x8c10e214 */
 goto L8c10e22a;
L8c10e22a: r[5]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r5 */
 goto L8c10e22c;
L8c10e22c: r[3]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r3 */
 goto L8c10e22e;
L8c10e22e: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c10e230;
L8c10e230: r[5]-=r[3]; /* sub r3,r5 */
 goto L8c10e232;
L8c10e232: *sr=(*sr&~1u)|((int32_t)r[5]>0); /* cmp/pl r5 */
 goto L8c10e234;
L8c10e234: r[2]+=r[5]; /* add r5,r2 */
 goto L8c10e236;
L8c10e236: take=!(*sr&1u);wr(m,r[15],r[2],4);if(take)goto L8c10e254;goto L8c10e23a; /* bf/s 0x8c10e254 */
 goto L8c10e23a;
L8c10e23a: r[6]=r[10]; /* mov r10,r6 */
 goto L8c10e23c;
L8c10e23c: *pr=0x8c10e240u;r[4]=rd(m,r[14]+0x10u,4);status=call(ctx,0x8c10e50eu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c10e50e */
 goto L8c10e240;
L8c10e240: goto L8c10e254; /* bra 0x8c10e254 */
 goto L8c10e244;
L8c10e244: r[4]=r[15]; /* mov r15,r4 */
 goto L8c10e246;
L8c10e246: r[5]=1u; /* mov #0x1,r5 */
 goto L8c10e248;
L8c10e248: r[6]=r[10]; /* mov r10,r6 */
 goto L8c10e24a;
L8c10e24a: *pr=0x8c10e24eu;r[4]+=8u;status=call(ctx,0x8c10e50eu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c10e50e */
 goto L8c10e24e;
L8c10e24e: r[3]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r3 */
 goto L8c10e250;
L8c10e250: r[3]+=4294967295u; /* add #-0x1,r3 */
 goto L8c10e252;
L8c10e252: wr(m,r[14]+0x20u,r[3],4); /* mov.l r3,@(0x20,r14) */
 goto L8c10e254;
L8c10e254: r[2]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r2 */
 goto L8c10e256;
L8c10e256: *sr=(*sr&~1u)|((int32_t)r[2]>0); /* cmp/pl r2 */
 goto L8c10e258;
L8c10e258: if((*sr&1u))goto L8c10e244; /* bt 0x8c10e244 */
 goto L8c10e25a;
L8c10e25a: r[5]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r5 */
 goto L8c10e25c;
L8c10e25c: r[6]=r[10]; /* mov r10,r6 */
 goto L8c10e25e;
L8c10e25e: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c10e260;
L8c10e260: r[5]-=r[3]; /* sub r3,r5 */
 goto L8c10e262;
L8c10e262: *pr=0x8c10e266u;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,0x8c10e50eu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c10e50e */
 goto L8c10e266;
L8c10e266: goto L8c10e27a; /* bra 0x8c10e27a */
 goto L8c10e26a;
L8c10e26a: r[4]=r[15]; /* mov r15,r4 */
 goto L8c10e26c;
L8c10e26c: r[5]=1u; /* mov #0x1,r5 */
 goto L8c10e26e;
L8c10e26e: r[6]=r[10]; /* mov r10,r6 */
 goto L8c10e270;
L8c10e270: *pr=0x8c10e274u;r[4]+=8u;status=call(ctx,0x8c10e50eu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c10e50e */
 goto L8c10e274;
L8c10e274: r[2]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r2 */
 goto L8c10e276;
L8c10e276: r[2]+=4294967295u; /* add #-0x1,r2 */
 goto L8c10e278;
L8c10e278: wr(m,r[14]+0x24u,r[2],4); /* mov.l r2,@(0x24,r14) */
 goto L8c10e27a;
L8c10e27a: r[3]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r3 */
 goto L8c10e27c;
L8c10e27c: *sr=(*sr&~1u)|((int32_t)r[3]>0); /* cmp/pl r3 */
 goto L8c10e27e;
L8c10e27e: if((*sr&1u))goto L8c10e26a; /* bt 0x8c10e26a */
 goto L8c10e280;
L8c10e280: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c10e282;
L8c10e282: *sr=(*sr&~1u)|!(r[0]&0x2u); /* tst #0x2,r0 */
 goto L8c10e284;
L8c10e284: if((*sr&1u))goto L8c10e2a6; /* bt 0x8c10e2a6 */
 goto L8c10e286;
L8c10e286: r[2]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r2 */
 goto L8c10e288;
L8c10e288: *sr=(*sr&~1u)|((int32_t)r[2]>0); /* cmp/pl r2 */
 goto L8c10e28a;
L8c10e28a: if(!(*sr&1u))goto L8c10e2a6; /* bf 0x8c10e2a6 */
 goto L8c10e28c;
L8c10e28c: goto L8c10e2a0; /* bra 0x8c10e2a0 */
 goto L8c10e290;
L8c10e290: r[4]=r[14]; /* mov r14,r4 */
 goto L8c10e292;
L8c10e292: r[5]=1u; /* mov #0x1,r5 */
 goto L8c10e294;
L8c10e294: r[6]=r[10]; /* mov r10,r6 */
 goto L8c10e296;
L8c10e296: *pr=0x8c10e29au;r[4]+=1u;status=call(ctx,0x8c10e50eu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c10e50e */
 goto L8c10e29a;
L8c10e29a: r[2]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r2 */
 goto L8c10e29c;
L8c10e29c: r[2]+=4294967295u; /* add #-0x1,r2 */
 goto L8c10e29e;
L8c10e29e: wr(m,r[14]+0x28u,r[2],4); /* mov.l r2,@(0x28,r14) */
 goto L8c10e2a0;
L8c10e2a0: r[3]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r3 */
 goto L8c10e2a2;
L8c10e2a2: *sr=(*sr&~1u)|((int32_t)r[3]>0); /* cmp/pl r3 */
 goto L8c10e2a4;
L8c10e2a4: if((*sr&1u))goto L8c10e290; /* bt 0x8c10e290 */
 goto L8c10e2a6;
L8c10e2a6: r[12]+=1u;goto L8c10e2ce; /* bra 0x8c10e2ce */
 goto L8c10e2aa;
L8c10e2aa: wr(m,r[15],r[12],4);goto L8c10e2b4; /* bra 0x8c10e2b4 */
 goto L8c10e2ae;
L8c10e2ae: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c10e2b0;
L8c10e2b0: r[2]+=1u; /* add #0x1,r2 */
 goto L8c10e2b2;
L8c10e2b2: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto L8c10e2b4;
L8c10e2b4: r[4]=rd(m,r[15],4); /* mov.l @r15,r4 */
 goto L8c10e2b6;
L8c10e2b6: r[4]=sx(m,r[4],1); /* mov.b @r4,r4 */
 goto L8c10e2b8;
L8c10e2b8: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c10e2ba;
L8c10e2ba: if((*sr&1u))goto L8c10e2c2; /* bt 0x8c10e2c2 */
 goto L8c10e2bc;
L8c10e2bc: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10e2be;
L8c10e2be: *sr=(*sr&~1u)|(r[0]==0x00000025u); /* cmp/eq #0x25,r0 */
 goto L8c10e2c0;
L8c10e2c0: if(!(*sr&1u))goto L8c10e2ae; /* bf 0x8c10e2ae */
 goto L8c10e2c2;
L8c10e2c2: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c10e2c4;
L8c10e2c4: r[6]=r[10]; /* mov r10,r6 */
 goto L8c10e2c6;
L8c10e2c6: r[5]-=r[12]; /* sub r12,r5 */
 goto L8c10e2c8;
L8c10e2c8: *pr=0x8c10e2ccu;r[4]=r[12];status=call(ctx,0x8c10e50eu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c10e50e */
 goto L8c10e2cc;
L8c10e2cc: r[12]=rd(m,r[15],4); /* mov.l @r15,r12 */
 goto L8c10e2ce;
L8c10e2ce: r[3]=sx(m,r[12],1); /* mov.b @r12,r3 */
 goto L8c10e2d0;
L8c10e2d0: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c10e2d2;
L8c10e2d2: if((*sr&1u))goto L8c10e2e6; /* bt 0x8c10e2e6 */
 goto L8c10e2d4;
L8c10e2d4: *pr=0x8c10e2d8u;r[4]=r[10];status=call(ctx,0x8c10e576u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c10e576 */
 goto L8c10e2d8;
L8c10e2d8: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c10e2da;
L8c10e2da: if(!(*sr&1u))goto L8c10e2e6; /* bf 0x8c10e2e6 */
 goto L8c10e2dc;
L8c10e2dc: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c10e2de;
L8c10e2de: *sr=(*sr&~1u)|!(r[0]&0x1u); /* tst #0x1,r0 */
 goto L8c10e2e0;
L8c10e2e0: if(!(*sr&1u))goto L8c10e2e6; /* bf 0x8c10e2e6 */
 goto L8c10e2e2;
L8c10e2e2: goto L8c10dc64; /* bra 0x8c10dc64 */
 goto L8c10e2e6;
L8c10e2e6: *pr=0x8c10e2eau;r[4]=r[10];status=call(ctx,0x8c10e566u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c10e566 */
 goto L8c10e2ea;
L8c10e2ea: r[0]=rd(m,r[10]+0x8u,4); /* mov.l @(0x8,r10),r0 */
 goto L8c10e2ec;
L8c10e2ec: r[1]=sx(m,0x8c10e302u,2); /* mov.w 0x8c10e302,r1 */
 goto L8c10e2ee;
L8c10e2ee: r[15]+=r[1]; /* add r1,r15 */
 goto L8c10e2f0;
L8c10e2f0: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10e2f2;
L8c10e2f2: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c10e2f4;
L8c10e2f4: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c10e2f6;
L8c10e2f6: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c10e2f8;
L8c10e2f8: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c10e2fa;
L8c10e2fa: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c10e2fc;
L8c10e2fc: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c10e2fe;
L8c10e2fe: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
