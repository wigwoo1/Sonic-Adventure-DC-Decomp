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
/* Static C recovery of retail Tikal helper 8c65fd4e..8c65fdfe.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_port_response_mask(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65fd4e;
L8c65fd4e: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c65fd50;
L8c65fd50: r[6]=rd(m,0x8c65fe0cu,4); /* mov.l 0x8c65fe0c,r6 */
 goto L8c65fd52;
L8c65fd52: r[3]=r[4]; /* mov r4,r3 */
 goto L8c65fd54;
L8c65fd54: r[3]<<=2; /* shll2 r3 */
 goto L8c65fd56;
L8c65fd56: r[2]=sx(m,0x8c65fe02u,2); /* mov.w 0x8c65fe02,r2 */
 goto L8c65fd58;
L8c65fd58: r[5]=rd(m,r[6],4); /* mov.l @r6,r5 */
 goto L8c65fd5a;
L8c65fd5a: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c65fd5c;
L8c65fd5c: r[0]=rd(m,r[6],4); /* mov.l @r6,r0 */
 goto L8c65fd5e;
L8c65fd5e: r[4]<<=2; /* shll2 r4 */
 goto L8c65fd60;
L8c65fd60: r[5]+=4u; /* add #0x4,r5 */
 goto L8c65fd62;
L8c65fd62: r[1]=sx(m,0x8c65fe04u,2); /* mov.w 0x8c65fe04,r1 */
 goto L8c65fd64;
L8c65fd64: r[5]+=r[3]; /* add r3,r5 */
 goto L8c65fd66;
L8c65fd66: r[3]=r[0]; /* mov r0,r3 */
 goto L8c65fd68;
L8c65fd68: r[2]+=r[3]; /* add r3,r2 */
 goto L8c65fd6a;
L8c65fd6a: r[2]=rd(m,r[2],4); /* mov.l @r2,r2 */
 goto L8c65fd6c;
L8c65fd6c: r[0]+=r[1]; /* add r1,r0 */
 goto L8c65fd6e;
L8c65fd6e: r[2]=0u-r[2]; /* neg r2,r2 */
 goto L8c65fd70;
L8c65fd70: r[2]+=1u; /* add #0x1,r2 */
 goto L8c65fd72;
L8c65fd72: r[3]=r[2]; /* mov r2,r3 */
 goto L8c65fd74;
L8c65fd74: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c65fd76;
L8c65fd76: r[2]+=r[3]; /* add r3,r2 */
 goto L8c65fd78;
L8c65fd78: r[2]<<=2; /* shll2 r2 */
 goto L8c65fd7a;
L8c65fd7a: r[2]<<=2; /* shll2 r2 */
 goto L8c65fd7c;
L8c65fd7c: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c65fd7e;
L8c65fd7e: r[0]+=r[2]; /* add r2,r0 */
 goto L8c65fd80;
L8c65fd80: r[2]=r[0]; /* mov r0,r2 */
 goto L8c65fd82;
L8c65fd82: r[0]=rd(m,0x8c65fe10u,4); /* mov.l 0x8c65fe10,r0 */
 goto L8c65fd84;
L8c65fd84: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c65fd86;
L8c65fd86: r[3]<<=2; /* shll2 r3 */
 goto L8c65fd88;
L8c65fd88: r[2]+=r[3]; /* add r3,r2 */
 goto L8c65fd8a;
L8c65fd8a: r[4]=rd(m,r[2],4); /* mov.l @r2,r4 */
 goto L8c65fd8c;
L8c65fd8c: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c65fd8e;
L8c65fd8e: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c65fd90;
L8c65fd90: r[0]=r[3]; /* mov r3,r0 */
 goto L8c65fd92;
L8c65fd92: *sr=(*sr&~1u)|(r[0]==0xffffffffu); /* cmp/eq #-0x1,r0 */
 goto L8c65fd94;
L8c65fd94: take=(*sr&1u);r[7]=0u;if(take)goto L8c65fda2;goto L8c65fd98; /* bt/s 0x8c65fda2 */
 goto L8c65fd98;
L8c65fd98: r[1]=sx(m,0x8c65fe06u,2); /* mov.w 0x8c65fe06,r1 */
 goto L8c65fd9a;
L8c65fd9a: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65fd9c;
L8c65fd9c: if((*sr&1u))goto L8c65fda2; /* bt 0x8c65fda2 */
 goto L8c65fd9e;
L8c65fd9e: goto L8c65fdc0; /* bra 0x8c65fdc0 */
 goto L8c65fda2;
L8c65fda2: r[3]=rd(m,r[5]+0x4u,4); /* mov.l @(0x4,r5),r3 */
 goto L8c65fda4;
L8c65fda4: r[0]=sx(m,0x8c65fe08u,2); /* mov.w 0x8c65fe08,r0 */
 goto L8c65fda6;
L8c65fda6: r[3]+=1u; /* add #0x1,r3 */
 goto L8c65fda8;
L8c65fda8: wr(m,r[5]+0x4u,r[3],4); /* mov.l r3,@(0x4,r5) */
 goto L8c65fdaa;
L8c65fdaa: r[2]=rd(m,r[6],4); /* mov.l @r6,r2 */
 goto L8c65fdac;
L8c65fdac: r[1]=rd(m,r[5]+0x4u,4); /* mov.l @(0x4,r5),r1 */
 goto L8c65fdae;
L8c65fdae: r[3]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto L8c65fdb0;
L8c65fdb0: *sr=(*sr&~1u)|(r[1]>r[3]); /* cmp/hi r3,r1 */
 goto L8c65fdb2;
L8c65fdb2: if(!(*sr&1u))goto L8c65fdba; /* bf 0x8c65fdba */
 goto L8c65fdb4;
L8c65fdb4: r[6]=r[7]; /* mov r7,r6 */
 goto L8c65fdb6;
L8c65fdb6: r[4]=4294967295u;goto L8c65fdda; /* bra 0x8c65fdda */
 goto L8c65fdba;
L8c65fdba: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65fdbc;
L8c65fdbc: r[15]+=4u;return 0; /* rts */
L8c65fdc0: r[1]=sx(m,0x8c65fe0au,2); /* mov.w 0x8c65fe0a,r1 */
 goto L8c65fdc2;
L8c65fdc2: r[0]=rd(m,r[6],4); /* mov.l @r6,r0 */
 goto L8c65fdc4;
L8c65fdc4: r[0]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto L8c65fdc6;
L8c65fdc6: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65fdc8;
L8c65fdc8: if(!(*sr&1u))goto L8c65fdd0; /* bf 0x8c65fdd0 */
 goto L8c65fdca;
L8c65fdca: r[4]=r[15]; /* mov r15,r4 */
 goto L8c65fdcc;
L8c65fdcc: r[4]+=1u;goto L8c65fdd4; /* bra 0x8c65fdd4 */
 goto L8c65fdd0;
L8c65fdd0: r[4]=r[15]; /* mov r15,r4 */
 goto L8c65fdd2;
L8c65fdd2: r[4]+=2u; /* add #0x2,r4 */
 goto L8c65fdd4;
L8c65fdd4: r[6]=sx(m,r[4],1); /* mov.b @r4,r6 */
 goto L8c65fdd6;
L8c65fdd6: r[4]=r[7]; /* mov r7,r4 */
 goto L8c65fdd8;
L8c65fdd8: wr(m,r[5]+0x4u,r[7],4); /* mov.l r7,@(0x4,r5) */
 goto L8c65fdda;
L8c65fdda: r[0]=sx(m,r[5],1); /* mov.b @r5,r0 */
 goto L8c65fddc;
L8c65fddc: wr(m,r[5]+0x1u,r[0],1); /* mov.b r0,@(0x1,r5) */
 goto L8c65fdde;
L8c65fdde: wr(m,r[5],r[6],1); /* mov.b r6,@r5 */
 goto L8c65fde0;
L8c65fde0: r[0]=sx(m,r[5]+0x1u,1); /* mov.b @(0x1,r5),r0 */
 goto L8c65fde2;
L8c65fde2: r[6]=sx(m,r[5],1); /* mov.b @r5,r6 */
 goto L8c65fde4;
L8c65fde4: r[0]^=r[6]; /* xor r6,r0 */
 goto L8c65fde6;
L8c65fde6: r[0]&=r[6]; /* and r6,r0 */
 goto L8c65fde8;
L8c65fde8: wr(m,r[5]+0x2u,r[0],1); /* mov.b r0,@(0x2,r5) */
 goto L8c65fdea;
L8c65fdea: r[6]=sx(m,r[5],1); /* mov.b @r5,r6 */
 goto L8c65fdec;
L8c65fdec: r[0]=~r[6]; /* not r6,r0 */
 goto L8c65fdee;
L8c65fdee: r[3]=r[0]; /* mov r0,r3 */
 goto L8c65fdf0;
L8c65fdf0: r[0]=sx(m,r[5]+0x1u,1); /* mov.b @(0x1,r5),r0 */
 goto L8c65fdf2;
L8c65fdf2: r[0]^=r[6]; /* xor r6,r0 */
 goto L8c65fdf4;
L8c65fdf4: r[0]&=r[3]; /* and r3,r0 */
 goto L8c65fdf6;
L8c65fdf6: wr(m,r[5]+0x3u,r[0],1); /* mov.b r0,@(0x3,r5) */
 goto L8c65fdf8;
L8c65fdf8: r[0]=r[4]; /* mov r4,r0 */
 goto L8c65fdfa;
L8c65fdfa: r[15]+=4u;return 0; /* rts */
}
