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
/* Static C recovery of retail Tikal helper 8c10db1c..8c10dbc4.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_string_compare(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10db1c;
L8c10db1c: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10db1e;
L8c10db1e: r[0]|=r[5]; /* or r5,r0 */
 goto L8c10db20;
L8c10db20: *sr=(*sr&~1u)|!(r[0]&0x3u); /* tst #0x3,r0 */
 goto L8c10db22;
L8c10db22: if(!(*sr&1u))goto L8c10db90; /* bf 0x8c10db90 */
 goto L8c10db24;
L8c10db24: r[3]=rd(m,r[5],4);r[5]+=4; /* mov.l @r5+,r3 */
 goto L8c10db26;
L8c10db26: r[2]=0u; /* mov #0x0,r2 */
 goto L8c10db28;
L8c10db28: r[0]=rd(m,r[4],4);r[4]+=4; /* mov.l @r4+,r0 */
 goto L8c10db2a;
L8c10db2a: {uint32_t x=r[2]^r[3];*sr=(*sr&~1u)|(!(x&0xffu)||!(x&0xff00u)||!(x&0xff0000u)||!(x&0xff000000u));} /* cmp/str r2,r3 */
 goto L8c10db2c;
L8c10db2c: if((*sr&1u))goto L8c10db5a; /* bt 0x8c10db5a */
 goto L8c10db2e;
L8c10db2e: *sr=(*sr&~1u)|(r[0]==r[3]); /* cmp/eq r0,r3 */
 goto L8c10db30;
L8c10db30: r[1]=rd(m,r[5],4);r[5]+=4; /* mov.l @r5+,r1 */
 goto L8c10db32;
L8c10db32: if(!(*sr&1u))goto L8c10db58; /* bf 0x8c10db58 */
 goto L8c10db34;
L8c10db34: r[0]=rd(m,r[4],4);r[4]+=4; /* mov.l @r4+,r0 */
 goto L8c10db36;
L8c10db36: {uint32_t x=r[2]^r[1];*sr=(*sr&~1u)|(!(x&0xffu)||!(x&0xff00u)||!(x&0xff0000u)||!(x&0xff000000u));} /* cmp/str r2,r1 */
 goto L8c10db38;
L8c10db38: if((*sr&1u))goto L8c10db5a; /* bt 0x8c10db5a */
 goto L8c10db3a;
L8c10db3a: *sr=(*sr&~1u)|(r[0]==r[1]); /* cmp/eq r0,r1 */
 goto L8c10db3c;
L8c10db3c: r[3]=rd(m,r[5],4);r[5]+=4; /* mov.l @r5+,r3 */
 goto L8c10db3e;
L8c10db3e: if(!(*sr&1u))goto L8c10db58; /* bf 0x8c10db58 */
 goto L8c10db40;
L8c10db40: r[0]=rd(m,r[4],4);r[4]+=4; /* mov.l @r4+,r0 */
 goto L8c10db42;
L8c10db42: {uint32_t x=r[2]^r[3];*sr=(*sr&~1u)|(!(x&0xffu)||!(x&0xff00u)||!(x&0xff0000u)||!(x&0xff000000u));} /* cmp/str r2,r3 */
 goto L8c10db44;
L8c10db44: if((*sr&1u))goto L8c10db5a; /* bt 0x8c10db5a */
 goto L8c10db46;
L8c10db46: *sr=(*sr&~1u)|(r[0]==r[3]); /* cmp/eq r0,r3 */
 goto L8c10db48;
L8c10db48: r[1]=rd(m,r[5],4);r[5]+=4; /* mov.l @r5+,r1 */
 goto L8c10db4a;
L8c10db4a: if(!(*sr&1u))goto L8c10db58; /* bf 0x8c10db58 */
 goto L8c10db4c;
L8c10db4c: r[0]=rd(m,r[4],4);r[4]+=4; /* mov.l @r4+,r0 */
 goto L8c10db4e;
L8c10db4e: {uint32_t x=r[2]^r[1];*sr=(*sr&~1u)|(!(x&0xffu)||!(x&0xff00u)||!(x&0xff0000u)||!(x&0xff000000u));} /* cmp/str r2,r1 */
 goto L8c10db50;
L8c10db50: if((*sr&1u))goto L8c10db5a; /* bt 0x8c10db5a */
 goto L8c10db52;
L8c10db52: *sr=(*sr&~1u)|(r[0]==r[1]); /* cmp/eq r0,r1 */
 goto L8c10db54;
L8c10db54: r[3]=rd(m,r[5],4);r[5]+=4; /* mov.l @r5+,r3 */
 goto L8c10db56;
L8c10db56: if((*sr&1u))goto L8c10db28; /* bt 0x8c10db28 */
 goto L8c10db58;
L8c10db58: r[5]+=4294967292u; /* add #-0x4,r5 */
 goto L8c10db5a;
L8c10db5a: r[5]+=4294967292u; /* add #-0x4,r5 */
 goto L8c10db5c;
L8c10db5c: r[1]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r1 */
 goto L8c10db5e;
L8c10db5e: r[4]+=4294967292u; /* add #-0x4,r4 */
 goto L8c10db60;
L8c10db60: r[0]=sx(m,r[4],1);r[4]+=1; /* mov.b @r4+,r0 */
 goto L8c10db62;
L8c10db62: *sr=(*sr&~1u)|(r[2]==r[1]); /* cmp/eq r2,r1 */
 goto L8c10db64;
L8c10db64: if((*sr&1u))goto L8c10db86; /* bt 0x8c10db86 */
 goto L8c10db66;
L8c10db66: *sr=(*sr&~1u)|(r[0]==r[1]); /* cmp/eq r0,r1 */
 goto L8c10db68;
L8c10db68: if(!(*sr&1u))goto L8c10db86; /* bf 0x8c10db86 */
 goto L8c10db6a;
L8c10db6a: r[1]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r1 */
 goto L8c10db6c;
L8c10db6c: r[0]=sx(m,r[4],1);r[4]+=1; /* mov.b @r4+,r0 */
 goto L8c10db6e;
L8c10db6e: *sr=(*sr&~1u)|(r[2]==r[1]); /* cmp/eq r2,r1 */
 goto L8c10db70;
L8c10db70: if((*sr&1u))goto L8c10db86; /* bt 0x8c10db86 */
 goto L8c10db72;
L8c10db72: *sr=(*sr&~1u)|(r[0]==r[1]); /* cmp/eq r0,r1 */
 goto L8c10db74;
L8c10db74: if(!(*sr&1u))goto L8c10db86; /* bf 0x8c10db86 */
 goto L8c10db76;
L8c10db76: r[1]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r1 */
 goto L8c10db78;
L8c10db78: r[0]=sx(m,r[4],1);r[4]+=1; /* mov.b @r4+,r0 */
 goto L8c10db7a;
L8c10db7a: *sr=(*sr&~1u)|(r[2]==r[1]); /* cmp/eq r2,r1 */
 goto L8c10db7c;
L8c10db7c: if((*sr&1u))goto L8c10db86; /* bt 0x8c10db86 */
 goto L8c10db7e;
L8c10db7e: *sr=(*sr&~1u)|(r[0]==r[1]); /* cmp/eq r0,r1 */
 goto L8c10db80;
L8c10db80: if(!(*sr&1u))goto L8c10db86; /* bf 0x8c10db86 */
 goto L8c10db82;
L8c10db82: r[0]=sx(m,r[4],1);r[4]+=1; /* mov.b @r4+,r0 */
 goto L8c10db84;
L8c10db84: r[1]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r1 */
 goto L8c10db86;
L8c10db86: r[0]-=r[1];return 0; /* rts */
L8c10db90: r[0]=sx(m,r[4],1);r[4]+=1; /* mov.b @r4+,r0 */
 goto L8c10db92;
L8c10db92: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c10db94;
L8c10db94: r[3]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r3 */
 goto L8c10db96;
L8c10db96: if((*sr&1u))goto L8c10dbc0; /* bt 0x8c10dbc0 */
 goto L8c10db98;
L8c10db98: *sr=(*sr&~1u)|(r[0]==r[3]); /* cmp/eq r0,r3 */
 goto L8c10db9a;
L8c10db9a: if(!(*sr&1u))goto L8c10dbc0; /* bf 0x8c10dbc0 */
 goto L8c10db9c;
L8c10db9c: r[0]=sx(m,r[4],1);r[4]+=1; /* mov.b @r4+,r0 */
 goto L8c10db9e;
L8c10db9e: r[3]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r3 */
 goto L8c10dba0;
L8c10dba0: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c10dba2;
L8c10dba2: if((*sr&1u))goto L8c10dbc0; /* bt 0x8c10dbc0 */
 goto L8c10dba4;
L8c10dba4: *sr=(*sr&~1u)|(r[0]==r[3]); /* cmp/eq r0,r3 */
 goto L8c10dba6;
L8c10dba6: if(!(*sr&1u))goto L8c10dbc0; /* bf 0x8c10dbc0 */
 goto L8c10dba8;
L8c10dba8: r[0]=sx(m,r[4],1);r[4]+=1; /* mov.b @r4+,r0 */
 goto L8c10dbaa;
L8c10dbaa: r[3]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r3 */
 goto L8c10dbac;
L8c10dbac: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c10dbae;
L8c10dbae: if((*sr&1u))goto L8c10dbc0; /* bt 0x8c10dbc0 */
 goto L8c10dbb0;
L8c10dbb0: *sr=(*sr&~1u)|(r[0]==r[3]); /* cmp/eq r0,r3 */
 goto L8c10dbb2;
L8c10dbb2: if(!(*sr&1u))goto L8c10dbc0; /* bf 0x8c10dbc0 */
 goto L8c10dbb4;
L8c10dbb4: r[0]=sx(m,r[4],1);r[4]+=1; /* mov.b @r4+,r0 */
 goto L8c10dbb6;
L8c10dbb6: r[3]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r3 */
 goto L8c10dbb8;
L8c10dbb8: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c10dbba;
L8c10dbba: if((*sr&1u))goto L8c10dbc0; /* bt 0x8c10dbc0 */
 goto L8c10dbbc;
L8c10dbbc: *sr=(*sr&~1u)|(r[0]==r[3]); /* cmp/eq r0,r3 */
 goto L8c10dbbe;
L8c10dbbe: if((*sr&1u))goto L8c10db90; /* bt 0x8c10db90 */
 goto L8c10dbc0;
L8c10dbc0: r[0]-=r[3];return 0; /* rts */
}
