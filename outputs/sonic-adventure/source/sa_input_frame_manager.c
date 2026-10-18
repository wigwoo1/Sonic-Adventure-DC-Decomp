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
/* Static C recovery of retail Tikal helper 8c668e60..8c668f2e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
static int sa_input_frame_manager_core(uint32_t entry,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 switch(entry){case 0:goto L8c668e60;case 0x8c668e92u:goto L8c668e92;case 0x8c668edcu:goto L8c668edc;case 0x8c668ef2u:goto L8c668ef2;default:return -4;}
L8c668e60: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c668e62;
L8c668e62: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c668e64;
L8c668e64: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c668e66;
L8c668e66: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c668e68;
L8c668e68: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c668e6a;
L8c668e6a: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c668e6c;
L8c668e6c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c668e6e;
L8c668e6e: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c668e70;
L8c668e70: r[9]=rd(m,0x8c668ea4u,4); /* mov.l 0x8c668ea4,r9 */
 goto L8c668e72;
L8c668e72: r[13]=r[4]; /* mov r4,r13 */
 goto L8c668e74;
L8c668e74: r[2]=rd(m,0x8c668eb4u,4); /* mov.l 0x8c668eb4,r2 */
 goto L8c668e76;
L8c668e76: r[4]=r[13]; /* mov r13,r4 */
 goto L8c668e78;
L8c668e78: r[3]=rd(m,r[9],4); /* mov.l @r9,r3 */
 goto L8c668e7a;
L8c668e7a: r[12]=0u; /* mov #0x0,r12 */
 goto L8c668e7c;
L8c668e7c: r[1]=rd(m,0x8c668eb8u,4); /* mov.l 0x8c668eb8,r1 */
 goto L8c668e7e;
L8c668e7e: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c668e80;
L8c668e80: r[3]=f->fpscr; /* sts FPSCR,r3 */
 goto L8c668e82;
L8c668e82: f->fpscr=r[1];fr=f->bank[(f->fpscr>>21)&1u]; /* lds r1,fpscr */
 goto L8c668e84;
L8c668e84: r[14]=sx(m,0x8c668e9au,2); /* mov.w 0x8c668e9a,r14 */
 goto L8c668e86;
L8c668e86: wr(m,r[9],r[13],4); /* mov.l r13,@r9 */
 goto L8c668e88;
L8c668e88: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c668e8a;
L8c668e8a: r[14]+=r[13]; /* add r13,r14 */
 goto L8c668e8c;
L8c668e8c: r[3]=rd(m,0x8c668eacu,4); /* mov.l 0x8c668eac,r3 */
 goto L8c668e8e;
L8c668e8e: target=r[3];*pr=0x8c668e92u;r[4]+=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668e92;
L8c668e92: r[10]=rd(m,0x8c668eb0u,4); /* mov.l 0x8c668eb0,r10 */
 goto L8c668e94;
L8c668e94: r[11]=3u;goto L8c668f02; /* bra 0x8c668f02 */
 goto L8c668ebc;
L8c668ebc: r[0]=sx(m,0x8c668f2eu,2); /* mov.w 0x8c668f2e,r0 */
 goto L8c668ebe;
L8c668ebe: wr(m,r[13]+r[0],r[14],4); /* mov.l r14,@(r0,r13) */
 goto L8c668ec0;
L8c668ec0: r[0]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r0 */
 goto L8c668ec2;
L8c668ec2: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c668ec4;
L8c668ec4: if((*sr&1u))goto L8c668efc; /* bt 0x8c668efc */
 goto L8c668ec6;
L8c668ec6: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c668ec8;
L8c668ec8: if((*sr&1u))goto L8c668ed6; /* bt 0x8c668ed6 */
 goto L8c668eca;
L8c668eca: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c668ecc;
L8c668ecc: if((*sr&1u))goto L8c668ee0; /* bt 0x8c668ee0 */
 goto L8c668ece;
L8c668ece: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c668ed0;
L8c668ed0: if((*sr&1u))goto L8c668ef6; /* bt 0x8c668ef6 */
 goto L8c668ed2;
L8c668ed2: goto L8c668efc; /* bra 0x8c668efc */
 goto L8c668ed6;
L8c668ed6: r[5]=r[14]; /* mov r14,r5 */
 goto L8c668ed8;
L8c668ed8: *pr=0x8c668edcu;r[4]=r[13];status=call(ctx,0x8c668ccau,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c668cca */
 goto L8c668edc;
L8c668edc: goto L8c668efc; /* bra 0x8c668efc */
 goto L8c668ee0;
L8c668ee0: r[0]=sx(m,0x8c668f30u,2); /* mov.w 0x8c668f30,r0 */
 goto L8c668ee2;
L8c668ee2: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto L8c668ee4;
L8c668ee4: r[2]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r2 */
 goto L8c668ee6;
L8c668ee6: *sr=(*sr&~1u)|(r[3]>r[2]); /* cmp/hi r2,r3 */
 goto L8c668ee8;
L8c668ee8: if((*sr&1u))goto L8c668efc; /* bt 0x8c668efc */
 goto L8c668eea;
L8c668eea: r[4]=r[14]; /* mov r14,r4 */
 goto L8c668eec;
L8c668eec: r[5]=1u; /* mov #0x1,r5 */
 goto L8c668eee;
L8c668eee: target=r[10];*pr=0x8c668ef2u;r[4]+=24u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r10 */
 goto L8c668ef2;
L8c668ef2: goto L8c668efc; /* bra 0x8c668efc */
 goto L8c668ef6;
L8c668ef6: r[3]=rd(m,0x8c668f38u,4); /* mov.l 0x8c668f38,r3 */
 goto L8c668ef8;
L8c668ef8: wr(m,r[14]+0xcu,r[11],4); /* mov.l r11,@(0xc,r14) */
 goto L8c668efa;
L8c668efa: wr(m,r[14]+0x20u,r[3],4); /* mov.l r3,@(0x20,r14) */
 goto L8c668efc;
L8c668efc: r[3]=sx(m,0x8c668f32u,2); /* mov.w 0x8c668f32,r3 */
 goto L8c668efe;
L8c668efe: r[12]+=1u; /* add #0x1,r12 */
 goto L8c668f00;
L8c668f00: r[14]+=r[3]; /* add r3,r14 */
 goto L8c668f02;
L8c668f02: r[0]=sx(m,0x8c668f34u,2); /* mov.w 0x8c668f34,r0 */
 goto L8c668f04;
L8c668f04: r[2]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r2 */
 goto L8c668f06;
L8c668f06: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[2]); /* cmp/ge r2,r12 */
 goto L8c668f08;
L8c668f08: if(!(*sr&1u))goto L8c668ebc; /* bf 0x8c668ebc */
 goto L8c668f0a;
L8c668f0a: r[0]=sx(m,0x8c668f30u,2); /* mov.w 0x8c668f30,r0 */
 goto L8c668f0c;
L8c668f0c: r[1]=rd(m,0x8c668f3cu,4); /* mov.l 0x8c668f3c,r1 */
 goto L8c668f0e;
L8c668f0e: r[2]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r2 */
 goto L8c668f10;
L8c668f10: r[2]+=1u; /* add #0x1,r2 */
 goto L8c668f12;
L8c668f12: wr(m,r[13]+r[0],r[2],4); /* mov.l r2,@(r0,r13) */
 goto L8c668f14;
L8c668f14: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c668f16;
L8c668f16: wr(m,r[9],r[3],4); /* mov.l r3,@r9 */
 goto L8c668f18;
L8c668f18: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c668f1a;
L8c668f1a: f->fpscr=r[2];fr=f->bank[(f->fpscr>>21)&1u]; /* lds r2,fpscr */
 goto L8c668f1c;
L8c668f1c: r[15]+=4u; /* add #0x4,r15 */
 goto L8c668f1e;
L8c668f1e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c668f20;
L8c668f20: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c668f22;
L8c668f22: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c668f24;
L8c668f24: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c668f26;
L8c668f26: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c668f28;
L8c668f28: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c668f2a;
L8c668f2a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_input_frame_manager(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){return sa_input_frame_manager_core(0,m,f,sr,r,pr,fpul,macl,ctx,call);}
SA_STAGE_EXPORT int sa_input_frame_manager_resume(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){return sa_input_frame_manager_core(*pr,m,f,sr,r,pr,fpul,macl,ctx,call);}
