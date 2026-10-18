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
/* Static C recovery of the USA Rev A main executable. External calls use the supplied service dispatcher. */
SA_STAGE_EXPORT int sa_engine_frame_services(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c668f40;
L8c668f40: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c668f42;
L8c668f42: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c668f44;
L8c668f44: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto L8c668f46;
L8c668f46: r[0]=*sr; /* stc SR,r0 */
 goto L8c668f48;
L8c668f48: r[3]=sx(m,0x8c668fdau,2); /* mov.w 0x8c668fda,r3 */
 goto L8c668f4a;
L8c668f4a: r[0]>>=2; /* shlr2 r0 */
 goto L8c668f4c;
L8c668f4c: r[0]>>=2; /* shlr2 r0 */
 goto L8c668f4e;
L8c668f4e: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c668f50;
L8c668f50: wr(m,r[15]+0xcu,r[0],4); /* mov.l r0,@(0xc,r15) */
 goto L8c668f52;
L8c668f52: r[0]=*sr; /* stc SR,r0 */
 goto L8c668f54;
L8c668f54: r[0]&=r[3]; /* and r3,r0 */
 goto L8c668f56;
L8c668f56: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c668f58;
L8c668f58: *sr=r[0]; /* ldc r0,SR */
 goto L8c668f5a;
L8c668f5a: r[14]=rd(m,0x8c668fe4u,4); /* mov.l 0x8c668fe4,r14 */
 goto L8c668f5c;
L8c668f5c: r[2]=1u; /* mov #0x1,r2 */
 goto L8c668f5e;
L8c668f5e: r[0]=sx(m,0x8c668fdcu,2); /* mov.w 0x8c668fdc,r0 */
 goto L8c668f60;
L8c668f60: wr(m,r[4]+0x4u,r[2],4); /* mov.l r2,@(0x4,r4) */
 goto L8c668f62;
L8c668f62: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c668f64;
L8c668f64: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c668f66;
L8c668f66: r[2]+=1u; /* add #0x1,r2 */
 goto L8c668f68;
L8c668f68: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c668f6a;
L8c668f6a: r[3]=rd(m,0x8c668fe8u,4); /* mov.l 0x8c668fe8,r3 */
 goto L8c668f6c;
L8c668f6c: target=r[3];*pr=0x8c668f70u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668f70;
L8c668f70: r[0]=sx(m,0x8c668fdeu,2); /* mov.w 0x8c668fde,r0 */
 goto L8c668f72;
L8c668f72: r[3]=0u; /* mov #0x0,r3 */
 goto L8c668f74;
L8c668f74: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c668f76;
L8c668f76: r[4]=r[15]; /* mov r15,r4 */
 goto L8c668f78;
L8c668f78: wr(m,r[2]+r[0],r[3],4); /* mov.l r3,@(r0,r2) */
 goto L8c668f7a;
L8c668f7a: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c668f7c;
L8c668f7c: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c668f7e;
L8c668f7e: r[3]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto L8c668f80;
L8c668f80: r[3]+=1u; /* add #0x1,r3 */
 goto L8c668f82;
L8c668f82: wr(m,r[2]+r[0],r[3],4); /* mov.l r3,@(r0,r2) */
 goto L8c668f84;
L8c668f84: r[2]=rd(m,0x8c668fecu,4); /* mov.l 0x8c668fec,r2 */
 goto L8c668f86;
L8c668f86: target=r[2];*pr=0x8c668f8au;r[4]+=8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c668f8a;
L8c668f8a: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c668f8c;
L8c668f8c: *sr=(*sr&~1u)|((int32_t)r[3]>0); /* cmp/pl r3 */
 goto L8c668f8e;
L8c668f8e: if(!(*sr&1u))goto L8c668fc0; /* bf 0x8c668fc0 */
 goto L8c668f90;
L8c668f90: r[3]=rd(m,0x8c668ff0u,4); /* mov.l 0x8c668ff0,r3 */
 goto L8c668f92;
L8c668f92: r[5]=r[15]; /* mov r15,r5 */
 goto L8c668f94;
L8c668f94: r[5]+=4u; /* add #0x4,r5 */
 goto L8c668f96;
L8c668f96: target=r[3];*pr=0x8c668f9au;r[4]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668f9a;
L8c668f9a: r[1]=sx(m,0x8c668fe0u,2); /* mov.w 0x8c668fe0,r1 */
 goto L8c668f9c;
L8c668f9c: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c668f9e;
L8c668f9e: r[0]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto L8c668fa0;
L8c668fa0: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c668fa2;
L8c668fa2: if((*sr&1u))goto L8c668fac; /* bt 0x8c668fac */
 goto L8c668fa4;
L8c668fa4: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c668fa6;
L8c668fa6: if((*sr&1u))goto L8c668fb8; /* bt 0x8c668fb8 */
 goto L8c668fa8;
L8c668fa8: goto L8c668fc0; /* bra 0x8c668fc0 */
 goto L8c668fac;
L8c668fac: r[3]=rd(m,0x8c668ff4u,4); /* mov.l 0x8c668ff4,r3 */
 goto L8c668fae;
L8c668fae: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c668fb0;
L8c668fb0: target=r[3];*pr=0x8c668fb4u;r[4]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668fb4;
L8c668fb4: goto L8c668fc0; /* bra 0x8c668fc0 */
 goto L8c668fb8;
L8c668fb8: r[3]=rd(m,0x8c668ff8u,4); /* mov.l 0x8c668ff8,r3 */
 goto L8c668fba;
L8c668fba: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c668fbc;
L8c668fbc: target=r[3];*pr=0x8c668fc0u;r[4]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668fc0;
L8c668fc0: r[0]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r0 */
 goto L8c668fc2;
L8c668fc2: r[3]=*sr; /* stc SR,r3 */
 goto L8c668fc4;
L8c668fc4: r[2]=sx(m,0x8c668fdau,2); /* mov.w 0x8c668fda,r2 */
 goto L8c668fc6;
L8c668fc6: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c668fc8;
L8c668fc8: r[0]<<=2; /* shll2 r0 */
 goto L8c668fca;
L8c668fca: r[3]&=r[2]; /* and r2,r3 */
 goto L8c668fcc;
L8c668fcc: r[0]<<=2; /* shll2 r0 */
 goto L8c668fce;
L8c668fce: r[0]|=r[3]; /* or r3,r0 */
 goto L8c668fd0;
L8c668fd0: *sr=r[0]; /* ldc r0,SR */
 goto L8c668fd2;
L8c668fd2: r[15]+=16u; /* add #0x10,r15 */
 goto L8c668fd4;
L8c668fd4: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c668fd6;
L8c668fd6: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
