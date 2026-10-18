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
/* Static C recovery of retail Tikal helper 8c65eeda..8c65efda.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_commands_prepare(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65eeda;
L8c65eeda: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65eedc;
L8c65eedc: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65eede;
L8c65eede: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65eee0;
L8c65eee0: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65eee2;
L8c65eee2: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65eee4;
L8c65eee4: r[10]=rd(m,0x8c65ef2cu,4); /* mov.l 0x8c65ef2c,r10 */
 goto L8c65eee6;
L8c65eee6: r[0]=sx(m,0x8c65ef1au,2); /* mov.w 0x8c65ef1a,r0 */
 goto L8c65eee8;
L8c65eee8: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c65eeea;
L8c65eeea: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c65eeec;
L8c65eeec: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65eeee;
L8c65eeee: r[14]=rd(m,r[10],4); /* mov.l @r10,r14 */
 goto L8c65eef0;
L8c65eef0: r[11]=sx(m,0x8c65ef18u,2); /* mov.w 0x8c65ef18,r11 */
 goto L8c65eef2;
L8c65eef2: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c65eef4;
L8c65eef4: r[12]=r[14]; /* mov r14,r12 */
 goto L8c65eef6;
L8c65eef6: r[12]+=40u; /* add #0x28,r12 */
 goto L8c65eef8;
L8c65eef8: *sr=(*sr&~1u)|((int32_t)r[3]>0); /* cmp/pl r3 */
 goto L8c65eefa;
L8c65eefa: take=!(*sr&1u);r[11]+=r[14];if(take)goto L8c65ef04;goto L8c65eefe; /* bf/s 0x8c65ef04 */
 goto L8c65eefe;
L8c65eefe: r[3]=rd(m,0x8c65ef30u,4); /* mov.l 0x8c65ef30,r3 */
 goto L8c65ef00;
L8c65ef00: target=r[3];*pr=0x8c65ef04u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65ef04;
L8c65ef04: r[8]=rd(m,0x8c65ef20u,4); /* mov.l 0x8c65ef20,r8 */
 goto L8c65ef06;
L8c65ef06: r[13]=1u; /* mov #0x1,r13 */
 goto L8c65ef08;
L8c65ef08: r[9]=24u; /* mov #0x18,r9 */
 goto L8c65ef0a;
L8c65ef0a: r[14]=0u;goto L8c65efbc; /* bra 0x8c65efbc */
 goto L8c65ef34;
L8c65ef34: r[3]=sx(m,0x8c65f008u,2); /* mov.w 0x8c65f008,r3 */
 goto L8c65ef36;
L8c65ef36: r[0]=rd(m,r[10],4); /* mov.l @r10,r0 */
 goto L8c65ef38;
L8c65ef38: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65ef3a;
L8c65ef3a: r[3]+=r[14]; /* add r14,r3 */
 goto L8c65ef3c;
L8c65ef3c: r[2]=sx(m,r[3],1); /* mov.b @r3,r2 */
 goto L8c65ef3e;
L8c65ef3e: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c65ef40;
L8c65ef40: if((*sr&1u))goto L8c65efb6; /* bt 0x8c65efb6 */
 goto L8c65ef42;
L8c65ef42: r[0]=sx(m,r[12]+0x6u,1); /* mov.b @(0x6,r12),r0 */
 goto L8c65ef44;
L8c65ef44: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c65ef46;
L8c65ef46: if(!(*sr&1u))goto L8c65efb6; /* bf 0x8c65efb6 */
 goto L8c65ef48;
L8c65ef48: r[0]=sx(m,r[12]+0x4u,1); /* mov.b @(0x4,r12),r0 */
 goto L8c65ef4a;
L8c65ef4a: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c65ef4c;
L8c65ef4c: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65ef4e;
L8c65ef4e: if((*sr&1u))goto L8c65ef7e; /* bt 0x8c65ef7e */
 goto L8c65ef50;
L8c65ef50: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c65ef52;
L8c65ef52: if((*sr&1u))goto L8c65ef86; /* bt 0x8c65ef86 */
 goto L8c65ef54;
L8c65ef54: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c65ef56;
L8c65ef56: if((*sr&1u))goto L8c65ef90; /* bt 0x8c65ef90 */
 goto L8c65ef58;
L8c65ef58: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c65ef5a;
L8c65ef5a: if((*sr&1u))goto L8c65ef9a; /* bt 0x8c65ef9a */
 goto L8c65ef5c;
L8c65ef5c: *sr=(*sr&~1u)|(r[0]==0x9u); /* cmp/eq #0x9,r0 */
 goto L8c65ef5e;
L8c65ef5e: if((*sr&1u))goto L8c65efa4; /* bt 0x8c65efa4 */
 goto L8c65ef60;
L8c65ef60: *sr=(*sr&~1u)|(r[0]==0xau); /* cmp/eq #0xa,r0 */
 goto L8c65ef62;
L8c65ef62: if((*sr&1u))goto L8c65efb6; /* bt 0x8c65efb6 */
 goto L8c65ef64;
L8c65ef64: *sr=(*sr&~1u)|(r[0]==0xbu); /* cmp/eq #0xb,r0 */
 goto L8c65ef66;
L8c65ef66: if((*sr&1u))goto L8c65efb6; /* bt 0x8c65efb6 */
 goto L8c65ef68;
L8c65ef68: *sr=(*sr&~1u)|(r[0]==0xcu); /* cmp/eq #0xc,r0 */
 goto L8c65ef6a;
L8c65ef6a: if((*sr&1u))goto L8c65efb6; /* bt 0x8c65efb6 */
 goto L8c65ef6c;
L8c65ef6c: *sr=(*sr&~1u)|(r[0]==0xdu); /* cmp/eq #0xd,r0 */
 goto L8c65ef6e;
L8c65ef6e: if((*sr&1u))goto L8c65efb6; /* bt 0x8c65efb6 */
 goto L8c65ef70;
L8c65ef70: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c65ef72;
L8c65ef72: if((*sr&1u))goto L8c65efb6; /* bt 0x8c65efb6 */
 goto L8c65ef74;
L8c65ef74: r[1]=sx(m,0x8c65f00au,2); /* mov.w 0x8c65f00a,r1 */
 goto L8c65ef76;
L8c65ef76: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65ef78;
L8c65ef78: if((*sr&1u))goto L8c65efb6; /* bt 0x8c65efb6 */
 goto L8c65ef7a;
L8c65ef7a: goto L8c65efb6; /* bra 0x8c65efb6 */
 goto L8c65ef7e;
L8c65ef7e: target=r[8];*pr=0x8c65ef82u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r8 */
 goto L8c65ef82;
L8c65ef82: goto L8c65efb6; /* bra 0x8c65efb6 */
 goto L8c65ef86;
L8c65ef86: r[2]=rd(m,0x8c65f010u,4); /* mov.l 0x8c65f010,r2 */
 goto L8c65ef88;
L8c65ef88: target=r[2];*pr=0x8c65ef8cu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65ef8c;
L8c65ef8c: goto L8c65efb6; /* bra 0x8c65efb6 */
 goto L8c65ef90;
L8c65ef90: r[2]=rd(m,0x8c65f014u,4); /* mov.l 0x8c65f014,r2 */
 goto L8c65ef92;
L8c65ef92: target=r[2];*pr=0x8c65ef96u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65ef96;
L8c65ef96: goto L8c65efb6; /* bra 0x8c65efb6 */
 goto L8c65ef9a;
L8c65ef9a: r[2]=rd(m,0x8c65f018u,4); /* mov.l 0x8c65f018,r2 */
 goto L8c65ef9c;
L8c65ef9c: target=r[2];*pr=0x8c65efa0u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65efa0;
L8c65efa0: goto L8c65efb6; /* bra 0x8c65efb6 */
 goto L8c65efa4;
L8c65efa4: r[5]=rd(m,r[11],4); /* mov.l @r11,r5 */
 goto L8c65efa6;
L8c65efa6: r[2]=r[5]; /* mov r5,r2 */
 goto L8c65efa8;
L8c65efa8: *sr=(*sr&~1u)|!(r[13]&r[2]); /* tst r13,r2 */
 goto L8c65efaa;
L8c65efaa: take=(*sr&1u);r[4]=r[14];if(take)goto L8c65efb0;goto L8c65efae; /* bt/s 0x8c65efb0 */
 goto L8c65efae;
L8c65efae: r[5]=r[13]; /* mov r13,r5 */
 goto L8c65efb0;
L8c65efb0: r[2]=rd(m,0x8c65f01cu,4); /* mov.l 0x8c65f01c,r2 */
 goto L8c65efb2;
L8c65efb2: target=r[2];*pr=0x8c65efb6u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65efb6;
L8c65efb6: r[11]+=120u; /* add #0x78,r11 */
 goto L8c65efb8;
L8c65efb8: r[12]+=44u; /* add #0x2c,r12 */
 goto L8c65efba;
L8c65efba: r[14]+=1u; /* add #0x1,r14 */
 goto L8c65efbc;
L8c65efbc: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[9]); /* cmp/ge r9,r14 */
 goto L8c65efbe;
L8c65efbe: if(!(*sr&1u))goto L8c65ef34; /* bf 0x8c65ef34 */
 goto L8c65efc0;
L8c65efc0: r[3]=rd(m,0x8c65f020u,4); /* mov.l 0x8c65f020,r3 */
 goto L8c65efc2;
L8c65efc2: target=r[3];*pr=0x8c65efc6u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65efc6;
L8c65efc6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65efc8;
L8c65efc8: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65efca;
L8c65efca: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c65efcc;
L8c65efcc: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c65efce;
L8c65efce: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65efd0;
L8c65efd0: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65efd2;
L8c65efd2: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65efd4;
L8c65efd4: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65efd6;
L8c65efd6: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
