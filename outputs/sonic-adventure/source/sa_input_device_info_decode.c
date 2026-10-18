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
/* Static C recovery of retail Tikal helper 8c66018e..8c6602d2.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_info_decode(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66018e;
L8c66018e: r[6]+=4u; /* add #0x4,r6 */
 goto L8c660190;
L8c660190: r[3]=rd(m,0x8c660234u,4); /* mov.l 0x8c660234,r3 */
 goto L8c660192;
L8c660192: r[5]=r[6]; /* mov r6,r5 */
 goto L8c660194;
L8c660194: r[0]=sx(m,r[5]+0x1u,1); /* mov.b @(0x1,r5),r0 */
 goto L8c660196;
L8c660196: r[2]=rd(m,0x8c660238u,4); /* mov.l 0x8c660238,r2 */
 goto L8c660198;
L8c660198: r[6]=r[4]; /* mov r4,r6 */
 goto L8c66019a;
L8c66019a: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c66019c;
L8c66019c: r[0]<<=16; /* shll16 r0 */
 goto L8c66019e;
L8c66019e: r[0]&=r[3]; /* and r3,r0 */
 goto L8c6601a0;
L8c6601a0: r[1]=r[0]; /* mov r0,r1 */
 goto L8c6601a2;
L8c6601a2: r[0]=sx(m,r[5]+0x2u,1); /* mov.b @(0x2,r5),r0 */
 goto L8c6601a4;
L8c6601a4: r[3]<<=8; /* shll8 r3 */
 goto L8c6601a6;
L8c6601a6: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6601a8;
L8c6601a8: r[0]<<=8; /* shll8 r0 */
 goto L8c6601aa;
L8c6601aa: r[0]&=r[2]; /* and r2,r0 */
 goto L8c6601ac;
L8c6601ac: r[1]|=r[0]; /* or r0,r1 */
 goto L8c6601ae;
L8c6601ae: r[0]=sx(m,r[5],1); /* mov.b @r5,r0 */
 goto L8c6601b0;
L8c6601b0: r[7]=0u; /* mov #0x0,r7 */
 goto L8c6601b2;
L8c6601b2: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6601b4;
L8c6601b4: r[0]<<=16; /* shll16 r0 */
 goto L8c6601b6;
L8c6601b6: r[0]<<=8; /* shll8 r0 */
 goto L8c6601b8;
L8c6601b8: r[0]&=r[3]; /* and r3,r0 */
 goto L8c6601ba;
L8c6601ba: r[1]|=r[0]; /* or r0,r1 */
 goto L8c6601bc;
L8c6601bc: r[0]=sx(m,r[5]+0x3u,1); /* mov.b @(0x3,r5),r0 */
 goto L8c6601be;
L8c6601be: r[5]+=4u; /* add #0x4,r5 */
 goto L8c6601c0;
L8c6601c0: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6601c2;
L8c6601c2: r[1]|=r[0]; /* or r0,r1 */
 goto L8c6601c4;
L8c6601c4: wr(m,r[4],r[1],4); /* mov.l r1,@r4 */
 goto L8c6601c6;
L8c6601c6: r[6]+=4u;goto L8c6601d0; /* bra 0x8c6601d0 */
 goto L8c6601ca;
L8c6601ca: r[3]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r3 */
 goto L8c6601cc;
L8c6601cc: wr(m,r[6],r[3],1); /* mov.b r3,@r6 */
 goto L8c6601ce;
L8c6601ce: r[6]+=1u; /* add #0x1,r6 */
 goto L8c6601d0;
L8c6601d0: r[2]=r[4]; /* mov r4,r2 */
 goto L8c6601d2;
L8c6601d2: r[2]+=8u; /* add #0x8,r2 */
 goto L8c6601d4;
L8c6601d4: *sr=(*sr&~1u)|(r[6]>=r[2]); /* cmp/hs r2,r6 */
 goto L8c6601d6;
L8c6601d6: if(!(*sr&1u))goto L8c6601ca; /* bf 0x8c6601ca */
 goto L8c6601d8;
L8c6601d8: r[6]=r[4]; /* mov r4,r6 */
 goto L8c6601da;
L8c6601da: r[6]+=8u;goto L8c6601e4; /* bra 0x8c6601e4 */
 goto L8c6601de;
L8c6601de: r[3]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r3 */
 goto L8c6601e0;
L8c6601e0: wr(m,r[6],r[3],1); /* mov.b r3,@r6 */
 goto L8c6601e2;
L8c6601e2: r[6]+=1u; /* add #0x1,r6 */
 goto L8c6601e4;
L8c6601e4: r[2]=r[4]; /* mov r4,r2 */
 goto L8c6601e6;
L8c6601e6: r[2]+=12u; /* add #0xc,r2 */
 goto L8c6601e8;
L8c6601e8: *sr=(*sr&~1u)|(r[6]>=r[2]); /* cmp/hs r2,r6 */
 goto L8c6601ea;
L8c6601ea: if(!(*sr&1u))goto L8c6601de; /* bf 0x8c6601de */
 goto L8c6601ec;
L8c6601ec: r[6]=r[4]; /* mov r4,r6 */
 goto L8c6601ee;
L8c6601ee: r[6]+=12u;goto L8c6601f8; /* bra 0x8c6601f8 */
 goto L8c6601f2;
L8c6601f2: r[3]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r3 */
 goto L8c6601f4;
L8c6601f4: wr(m,r[6],r[3],1); /* mov.b r3,@r6 */
 goto L8c6601f6;
L8c6601f6: r[6]+=1u; /* add #0x1,r6 */
 goto L8c6601f8;
L8c6601f8: r[2]=r[4]; /* mov r4,r2 */
 goto L8c6601fa;
L8c6601fa: r[2]+=16u; /* add #0x10,r2 */
 goto L8c6601fc;
L8c6601fc: *sr=(*sr&~1u)|(r[6]>=r[2]); /* cmp/hs r2,r6 */
 goto L8c6601fe;
L8c6601fe: if(!(*sr&1u))goto L8c6601f2; /* bf 0x8c6601f2 */
 goto L8c660200;
L8c660200: r[3]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r3 */
 goto L8c660202;
L8c660202: r[0]=17u; /* mov #0x11,r0 */
 goto L8c660204;
L8c660204: wr(m,r[4]+r[0],r[3],1); /* mov.b r3,@(r0,r4) */
 goto L8c660206;
L8c660206: r[0]=16u; /* mov #0x10,r0 */
 goto L8c660208;
L8c660208: r[2]=sx(m,r[4]+r[0],1); /* mov.b @(r0,r4),r2 */
 goto L8c66020a;
L8c66020a: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c66020c;
L8c66020c: if((*sr&1u))goto L8c66023c; /* bt 0x8c66023c */
 goto L8c66020e;
L8c66020e: r[0]=sx(m,r[5],1); /* mov.b @r5,r0 */
 goto L8c660210;
L8c660210: r[2]=r[4]; /* mov r4,r2 */
 goto L8c660212;
L8c660212: r[2]+=18u; /* add #0x12,r2 */
 goto L8c660214;
L8c660214: r[0]&=0x3u; /* and #0x3,r0 */
 goto L8c660216;
L8c660216: wr(m,r[2],r[0],1); /* mov.b r0,@r2 */
 goto L8c660218;
L8c660218: r[1]=19u; /* mov #0x13,r1 */
 goto L8c66021a;
L8c66021a: r[0]=sx(m,r[5],1); /* mov.b @r5,r0 */
 goto L8c66021c;
L8c66021c: r[1]+=r[4]; /* add r4,r1 */
 goto L8c66021e;
L8c66021e: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c660220;
L8c660220: r[0]>>=2; /* shlr2 r0 */
 goto L8c660222;
L8c660222: r[0]&=0x3u; /* and #0x3,r0 */
 goto L8c660224;
L8c660224: wr(m,r[1],r[0],1);goto L8c660250; /* bra 0x8c660250 */
 goto L8c66023c;
L8c66023c: r[0]=sx(m,r[5],1); /* mov.b @r5,r0 */
 goto L8c66023e;
L8c66023e: r[2]=r[4]; /* mov r4,r2 */
 goto L8c660240;
L8c660240: r[1]=rd(m,0x8c660370u,4); /* mov.l 0x8c660370,r1 */
 goto L8c660242;
L8c660242: r[2]+=18u; /* add #0x12,r2 */
 goto L8c660244;
L8c660244: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c660246;
L8c660246: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c660248;
L8c660248: r[3]=sx(m,r[1]+r[0],1); /* mov.b @(r0,r1),r3 */
 goto L8c66024a;
L8c66024a: r[0]=19u; /* mov #0x13,r0 */
 goto L8c66024c;
L8c66024c: wr(m,r[2],r[3],1); /* mov.b r3,@r2 */
 goto L8c66024e;
L8c66024e: wr(m,r[4]+r[0],r[7],1); /* mov.b r7,@(r0,r4) */
 goto L8c660250;
L8c660250: r[3]=rd(m,0x8c660374u,4); /* mov.l 0x8c660374,r3 */
 goto L8c660252;
L8c660252: r[1]=sx(m,0x8c66036cu,2); /* mov.w 0x8c66036c,r1 */
 goto L8c660254;
L8c660254: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c660256;
L8c660256: r[0]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto L8c660258;
L8c660258: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c66025a;
L8c66025a: if(!(*sr&1u))goto L8c660274; /* bf 0x8c660274 */
 goto L8c66025c;
L8c66025c: r[6]=r[4]; /* mov r4,r6 */
 goto L8c66025e;
L8c66025e: r[6]+=20u;goto L8c660268; /* bra 0x8c660268 */
 goto L8c660262;
L8c660262: r[3]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r3 */
 goto L8c660264;
L8c660264: wr(m,r[6],r[3],1); /* mov.b r3,@r6 */
 goto L8c660266;
L8c660266: r[6]+=1u; /* add #0x1,r6 */
 goto L8c660268;
L8c660268: r[2]=r[4]; /* mov r4,r2 */
 goto L8c66026a;
L8c66026a: r[2]+=51u; /* add #0x33,r2 */
 goto L8c66026c;
L8c66026c: *sr=(*sr&~1u)|(r[6]>=r[2]); /* cmp/hs r2,r6 */
 goto L8c66026e;
L8c66026e: if(!(*sr&1u))goto L8c660262; /* bf 0x8c660262 */
 goto L8c660270;
L8c660270: goto L8c66028e; /* bra 0x8c66028e */
 goto L8c660274;
L8c660274: r[6]=r[4]; /* mov r4,r6 */
 goto L8c660276;
L8c660276: r[5]+=1u; /* add #0x1,r5 */
 goto L8c660278;
L8c660278: r[6]+=20u;goto L8c660282; /* bra 0x8c660282 */
 goto L8c66027c;
L8c66027c: r[3]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r3 */
 goto L8c66027e;
L8c66027e: wr(m,r[6],r[3],1); /* mov.b r3,@r6 */
 goto L8c660280;
L8c660280: r[6]+=1u; /* add #0x1,r6 */
 goto L8c660282;
L8c660282: r[2]=r[4]; /* mov r4,r2 */
 goto L8c660284;
L8c660284: r[2]+=50u; /* add #0x32,r2 */
 goto L8c660286;
L8c660286: *sr=(*sr&~1u)|(r[6]>=r[2]); /* cmp/hs r2,r6 */
 goto L8c660288;
L8c660288: if(!(*sr&1u))goto L8c66027c; /* bf 0x8c66027c */
 goto L8c66028a;
L8c66028a: r[0]=50u; /* mov #0x32,r0 */
 goto L8c66028c;
L8c66028c: wr(m,r[4]+r[0],r[7],1); /* mov.b r7,@(r0,r4) */
 goto L8c66028e;
L8c66028e: r[0]=51u; /* mov #0x33,r0 */
 goto L8c660290;
L8c660290: r[6]=r[4]; /* mov r4,r6 */
 goto L8c660292;
L8c660292: wr(m,r[4]+r[0],r[7],1); /* mov.b r7,@(r0,r4) */
 goto L8c660294;
L8c660294: r[6]+=52u;goto L8c66029e; /* bra 0x8c66029e */
 goto L8c660298;
L8c660298: r[3]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r3 */
 goto L8c66029a;
L8c66029a: wr(m,r[6],r[3],1); /* mov.b r3,@r6 */
 goto L8c66029c;
L8c66029c: r[6]+=1u; /* add #0x1,r6 */
 goto L8c66029e;
L8c66029e: r[2]=r[4]; /* mov r4,r2 */
 goto L8c6602a0;
L8c6602a0: r[2]+=112u; /* add #0x70,r2 */
 goto L8c6602a2;
L8c6602a2: *sr=(*sr&~1u)|(r[6]>=r[2]); /* cmp/hs r2,r6 */
 goto L8c6602a4;
L8c6602a4: if(!(*sr&1u))goto L8c660298; /* bf 0x8c660298 */
 goto L8c6602a6;
L8c6602a6: r[0]=112u; /* mov #0x70,r0 */
 goto L8c6602a8;
L8c6602a8: wr(m,r[4]+r[0],r[7],1); /* mov.b r7,@(r0,r4) */
 goto L8c6602aa;
L8c6602aa: r[1]=116u; /* mov #0x74,r1 */
 goto L8c6602ac;
L8c6602ac: r[0]=sx(m,r[5]+0x1u,1); /* mov.b @(0x1,r5),r0 */
 goto L8c6602ae;
L8c6602ae: r[1]+=r[4]; /* add r4,r1 */
 goto L8c6602b0;
L8c6602b0: r[3]=sx(m,r[5],1); /* mov.b @r5,r3 */
 goto L8c6602b2;
L8c6602b2: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6602b4;
L8c6602b4: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c6602b6;
L8c6602b6: r[0]<<=8; /* shll8 r0 */
 goto L8c6602b8;
L8c6602b8: r[0]|=r[3]; /* or r3,r0 */
 goto L8c6602ba;
L8c6602ba: wr(m,r[1],r[0],2); /* mov.w r0,@r1 */
 goto L8c6602bc;
L8c6602bc: r[0]=sx(m,r[5]+0x3u,1); /* mov.b @(0x3,r5),r0 */
 goto L8c6602be;
L8c6602be: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6602c0;
L8c6602c0: r[0]<<=8; /* shll8 r0 */
 goto L8c6602c2;
L8c6602c2: r[3]=r[0]; /* mov r0,r3 */
 goto L8c6602c4;
L8c6602c4: r[0]=sx(m,r[5]+0x2u,1); /* mov.b @(0x2,r5),r0 */
 goto L8c6602c6;
L8c6602c6: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6602c8;
L8c6602c8: r[3]|=r[0]; /* or r0,r3 */
 goto L8c6602ca;
L8c6602ca: r[0]=118u; /* mov #0x76,r0 */
 goto L8c6602cc;
L8c6602cc: wr(m,r[4]+r[0],r[3],2); /* mov.w r3,@(r0,r4) */
 goto L8c6602ce;
L8c6602ce: r[0]=0u;return 0; /* rts */
}
