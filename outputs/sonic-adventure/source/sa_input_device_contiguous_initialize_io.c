#include "input_io.h"
#include "fpu_cleanup_transfer.h"
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(const struct sa_input_io *io,uint8_t *m,uint32_t a,unsigned n){uint32_t physical=a&0x1fffffffu;if(physical<0x0c000000u||physical>=0x0d000000u)return io->read(io->user,a,n);uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(const struct sa_input_io *io,uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(io,m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(const struct sa_input_io *io,uint8_t *m,uint32_t a,uint32_t v,unsigned n){uint32_t physical=a&0x1fffffffu;if(physical<0x0c000000u||physical>=0x0d000000u){io->write(io->user,a,v,n);return;}for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|((a==b)||(!(a&0x7fffffffu)&&!(b&0x7fffffffu)));return 0;
}
/* Static C recovery of retail Tikal helper 8c65ebb0..8c65ec9c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_contiguous_initialize_io(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,const struct sa_input_io *io,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65ebb0;
L8c65ebb0: r[15]-=4;wr(io,m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65ebb2;
L8c65ebb2: r[14]=rd(io,m,0x8c65ecacu,4); /* mov.l 0x8c65ecac,r14 */
 goto L8c65ebb4;
L8c65ebb4: r[3]=rd(io,m,0x8c65ecb0u,4); /* mov.l 0x8c65ecb0,r3 */
 goto L8c65ebb6;
L8c65ebb6: r[0]=sx(io,m,0x8c65ec9cu,2); /* mov.w 0x8c65ec9c,r0 */
 goto L8c65ebb8;
L8c65ebb8: r[2]=r[3]; /* mov r3,r2 */
 goto L8c65ebba;
L8c65ebba: r[15]-=4;wr(io,m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65ebbc;
L8c65ebbc: r[15]-=4;wr(io,m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65ebbe;
L8c65ebbe: r[15]-=4;wr(io,m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65ebc0;
L8c65ebc0: r[15]-=4;wr(io,m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65ebc2;
L8c65ebc2: r[15]-=4;wr(io,m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65ebc4;
L8c65ebc4: wr(io,m,r[14],r[3],4); /* mov.l r3,@r14 */
 goto L8c65ebc6;
L8c65ebc6: wr(io,m,r[2]+r[0],r[4],4); /* mov.l r4,@(r0,r2) */
 goto L8c65ebc8;
L8c65ebc8: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65ebca;
L8c65ebca: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ebcc;
L8c65ebcc: wr(io,m,r[3]+r[0],r[6],4); /* mov.l r6,@(r0,r3) */
 goto L8c65ebce;
L8c65ebce: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c65ebd0;
L8c65ebd0: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ebd2;
L8c65ebd2: r[6]=31u; /* mov #0x1f,r6 */
 goto L8c65ebd4;
L8c65ebd4: r[2]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ebd6;
L8c65ebd6: *sr=(*sr&~1u)|!(r[6]&r[2]); /* tst r6,r2 */
 goto L8c65ebd8;
L8c65ebd8: take=(*sr&1u);r[5]=4294967264u;if(take)goto L8c65ebee;goto L8c65ebdc; /* bt/s 0x8c65ebee */
 goto L8c65ebdc;
L8c65ebdc: r[0]=sx(io,m,0x8c65ec9cu,2); /* mov.w 0x8c65ec9c,r0 */
 goto L8c65ebde;
L8c65ebde: r[1]=rd(io,m,r[14],4); /* mov.l @r14,r1 */
 goto L8c65ebe0;
L8c65ebe0: r[3]=rd(io,m,r[1]+r[0],4); /* mov.l @(r0,r1),r3 */
 goto L8c65ebe2;
L8c65ebe2: r[3]+=32u; /* add #0x20,r3 */
 goto L8c65ebe4;
L8c65ebe4: wr(io,m,r[1]+r[0],r[3],4); /* mov.l r3,@(r0,r1) */
 goto L8c65ebe6;
L8c65ebe6: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ebe8;
L8c65ebe8: r[2]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ebea;
L8c65ebea: r[2]&=r[5]; /* and r5,r2 */
 goto L8c65ebec;
L8c65ebec: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65ebee;
L8c65ebee: r[0]=sx(io,m,0x8c65ec9eu,2); /* mov.w 0x8c65ec9e,r0 */
 goto L8c65ebf0;
L8c65ebf0: r[4]=rd(io,m,r[14],4); /* mov.l @r14,r4 */
 goto L8c65ebf2;
L8c65ebf2: r[3]=rd(io,m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c65ebf4;
L8c65ebf4: *sr=(*sr&~1u)|!(r[3]&r[6]); /* tst r3,r6 */
 goto L8c65ebf6;
L8c65ebf6: if((*sr&1u))goto L8c65ec06; /* bt 0x8c65ec06 */
 goto L8c65ebf8;
L8c65ebf8: r[3]=rd(io,m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c65ebfa;
L8c65ebfa: r[3]+=32u; /* add #0x20,r3 */
 goto L8c65ebfc;
L8c65ebfc: wr(io,m,r[4]+r[0],r[3],4); /* mov.l r3,@(r0,r4) */
 goto L8c65ebfe;
L8c65ebfe: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ec00;
L8c65ec00: r[2]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ec02;
L8c65ec02: r[2]&=r[5]; /* and r5,r2 */
 goto L8c65ec04;
L8c65ec04: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65ec06;
L8c65ec06: r[0]=sx(io,m,0x8c65ec9cu,2); /* mov.w 0x8c65ec9c,r0 */
 goto L8c65ec08;
L8c65ec08: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ec0a;
L8c65ec0a: r[4]=rd(io,m,0x8c65ecb4u,4); /* mov.l 0x8c65ecb4,r4 */
 goto L8c65ec0c;
L8c65ec0c: r[2]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ec0e;
L8c65ec0e: r[2]|=r[4]; /* or r4,r2 */
 goto L8c65ec10;
L8c65ec10: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65ec12;
L8c65ec12: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65ec14;
L8c65ec14: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ec16;
L8c65ec16: r[2]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ec18;
L8c65ec18: r[2]|=r[4]; /* or r4,r2 */
 goto L8c65ec1a;
L8c65ec1a: wr(io,m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65ec1c;
L8c65ec1c: r[3]=rd(io,m,0x8c65ecb8u,4); /* mov.l 0x8c65ecb8,r3 */
 goto L8c65ec1e;
L8c65ec1e: target=r[3];*pr=0x8c65ec22u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65ec22;
L8c65ec22: r[4]=rd(io,m,0x8c65ecbcu,4); /* mov.l 0x8c65ecbc,r4 */
 goto L8c65ec24;
L8c65ec24: r[11]=rd(io,m,0x8c65ecc0u,4); /* mov.l 0x8c65ecc0,r11 */
 goto L8c65ec26;
L8c65ec26: r[10]=r[4]; /* mov r4,r10 */
 goto L8c65ec28;
L8c65ec28: r[10]+=16u; /* add #0x10,r10 */
 goto L8c65ec2a;
L8c65ec2a: r[5]=r[4]; /* mov r4,r5 */
 goto L8c65ec2c;
L8c65ec2c: r[12]=r[4]; /* mov r4,r12 */
 goto L8c65ec2e;
L8c65ec2e: r[13]=r[4]; /* mov r4,r13 */
 goto L8c65ec30;
L8c65ec30: r[2]=sx(io,m,0x8c65eca0u,2); /* mov.w 0x8c65eca0,r2 */
 goto L8c65ec32;
L8c65ec32: r[0]=rd(io,m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65ec34;
L8c65ec34: r[3]=rd(io,m,r[13],4); /* mov.l @r13,r3 */
 goto L8c65ec36;
L8c65ec36: r[2]+=r[0]; /* add r0,r2 */
 goto L8c65ec38;
L8c65ec38: r[2]+=r[3]; /* add r3,r2 */
 goto L8c65ec3a;
L8c65ec3a: r[1]=sx(io,m,r[2],1); /* mov.b @r2,r1 */
 goto L8c65ec3c;
L8c65ec3c: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c65ec3e;
L8c65ec3e: if((*sr&1u))goto L8c65ec44; /* bt 0x8c65ec44 */
 goto L8c65ec40;
L8c65ec40: target=r[11];*pr=0x8c65ec44u;r[4]=rd(io,m,r[12],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c65ec44;
L8c65ec44: r[13]+=4u; /* add #0x4,r13 */
 goto L8c65ec46;
L8c65ec46: *sr=(*sr&~1u)|(r[13]>=r[10]); /* cmp/hs r10,r13 */
 goto L8c65ec48;
L8c65ec48: take=!(*sr&1u);r[12]+=4u;if(take)goto L8c65ec30;goto L8c65ec4c; /* bf/s 0x8c65ec30 */
 goto L8c65ec4c;
L8c65ec4c: r[2]=rd(io,m,0x8c65ecc4u,4); /* mov.l 0x8c65ecc4,r2 */
 goto L8c65ec4e;
L8c65ec4e: target=r[2];*pr=0x8c65ec52u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65ec52;
L8c65ec52: r[0]=sx(io,m,0x8c65eca2u,2); /* mov.w 0x8c65eca2,r0 */
 goto L8c65ec54;
L8c65ec54: r[4]=1u; /* mov #0x1,r4 */
 goto L8c65ec56;
L8c65ec56: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ec58;
L8c65ec58: r[1]=rd(io,m,0x8c65ecc8u,4); /* mov.l 0x8c65ecc8,r1 */
 goto L8c65ec5a;
L8c65ec5a: wr(io,m,r[3]+r[0],r[4],4); /* mov.l r4,@(r0,r3) */
 goto L8c65ec5c;
L8c65ec5c: r[0]=sx(io,m,0x8c65eca4u,2); /* mov.w 0x8c65eca4,r0 */
 goto L8c65ec5e;
L8c65ec5e: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ec60;
L8c65ec60: r[2]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ec62;
L8c65ec62: r[0]=sx(io,m,0x8c65eca6u,2); /* mov.w 0x8c65eca6,r0 */
 goto L8c65ec64;
L8c65ec64: wr(io,m,r[2]+r[0],r[1],4); /* mov.l r1,@(r0,r2) */
 goto L8c65ec66;
L8c65ec66: r[0]=sx(io,m,0x8c65eca4u,2); /* mov.w 0x8c65eca4,r0 */
 goto L8c65ec68;
L8c65ec68: r[5]=0u; /* mov #0x0,r5 */
 goto L8c65ec6a;
L8c65ec6a: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ec6c;
L8c65ec6c: r[2]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ec6e;
L8c65ec6e: wr(io,m,r[2]+0x10u,r[5],4); /* mov.l r5,@(0x10,r2) */
 goto L8c65ec70;
L8c65ec70: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ec72;
L8c65ec72: r[2]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ec74;
L8c65ec74: r[0]+=4294967260u; /* add #-0x24,r0 */
 goto L8c65ec76;
L8c65ec76: r[1]=r[3]; /* mov r3,r1 */
 goto L8c65ec78;
L8c65ec78: r[3]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r3 */
 goto L8c65ec7a;
L8c65ec7a: r[0]+=4294967280u; /* add #-0x10,r0 */
 goto L8c65ec7c;
L8c65ec7c: r[1]=rd(io,m,r[1]+r[0],4); /* mov.l @(r0,r1),r1 */
 goto L8c65ec7e;
L8c65ec7e: r[0]=sx(io,m,0x8c65eca8u,2); /* mov.w 0x8c65eca8,r0 */
 goto L8c65ec80;
L8c65ec80: r[1]|=r[3]; /* or r3,r1 */
 goto L8c65ec82;
L8c65ec82: wr(io,m,r[2]+r[0],r[1],4); /* mov.l r1,@(r0,r2) */
 goto L8c65ec84;
L8c65ec84: r[0]=sx(io,m,0x8c65eca4u,2); /* mov.w 0x8c65eca4,r0 */
 goto L8c65ec86;
L8c65ec86: r[3]=rd(io,m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ec88;
L8c65ec88: r[2]=rd(io,m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ec8a;
L8c65ec8a: wr(io,m,r[2]+0x14u,r[4],4); /* mov.l r4,@(0x14,r2) */
 goto L8c65ec8c;
L8c65ec8c: *pr=rd(io,m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65ec8e;
L8c65ec8e: r[0]=r[5]; /* mov r5,r0 */
 goto L8c65ec90;
L8c65ec90: r[10]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65ec92;
L8c65ec92: r[11]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65ec94;
L8c65ec94: r[12]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65ec96;
L8c65ec96: r[13]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65ec98;
L8c65ec98: r[14]=rd(io,m,r[15],4);r[15]+=4;return 0; /* rts */
}
