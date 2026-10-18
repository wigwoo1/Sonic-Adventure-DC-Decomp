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
/* Retail magic8000 audio metadata65ae44..65af42.
 * Signed size>=16,BE signature/offset/rate/length,raw metadata bytes.
 * Final field uses native signed((stride-2)*8)/width calculation. */
SA_STAGE_EXPORT int sa_sound_magic_header_metadata(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65ae44;
L8c65ae44: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65ae46;
L8c65ae46: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65ae48;
L8c65ae48: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c65ae4a;
L8c65ae4a: r[3]=16u; /* mov #0x10,r3 */
 goto L8c65ae4c;
L8c65ae4c: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[3]); /* cmp/ge r3,r5 */
 goto L8c65ae4e;
L8c65ae4e: take=(*sr&1u);r[14]=r[4];if(take)goto L8c65ae5c;goto L8c65ae52; /* bt/s 0x8c65ae5c */
 goto L8c65ae52;
L8c65ae52: r[0]=4294967295u; /* mov #-0x1,r0 */
 goto L8c65ae54;
L8c65ae54: r[15]+=4u; /* add #0x4,r15 */
 goto L8c65ae56;
L8c65ae56: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65ae58;
L8c65ae58: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c65ae5c: r[3]=sx(m,r[4],2); /* mov.w @r4,r3 */
 goto L8c65ae5e;
L8c65ae5e: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c65ae60;
L8c65ae60: r[5]=rd(m,0x8c65af54u,4); /* mov.l 0x8c65af54,r5 */
 goto L8c65ae62;
L8c65ae62: r[3]=r[3]&65535u; /* extu.w r3,r3 */
 goto L8c65ae64;
L8c65ae64: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c65ae66;
L8c65ae66: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r3 */
 goto L8c65ae68;
L8c65ae68: r[1]=rd(m,r[15],4); /* mov.l @r15,r1 */
 goto L8c65ae6a;
L8c65ae6a: r[4]=r[3]&255u; /* extu.b r3,r4 */
 goto L8c65ae6c;
L8c65ae6c: r[3]=rd(m,0x8c65af58u,4); /* mov.l 0x8c65af58,r3 */
 goto L8c65ae6e;
L8c65ae6e: r[1]<<=8; /* shll8 r1 */
 goto L8c65ae70;
L8c65ae70: r[1]&=r[5]; /* and r5,r1 */
 goto L8c65ae72;
L8c65ae72: r[4]|=r[1]; /* or r1,r4 */
 goto L8c65ae74;
L8c65ae74: r[4]=r[4]&65535u; /* extu.w r4,r4 */
 goto L8c65ae76;
L8c65ae76: *sr=(*sr&~1u)|(r[3]==r[4]); /* cmp/eq r3,r4 */
 goto L8c65ae78;
L8c65ae78: if((*sr&1u))goto L8c65ae84; /* bt 0x8c65ae84 */
 goto L8c65ae7a;
L8c65ae7a: r[0]=4294967294u; /* mov #-0x2,r0 */
 goto L8c65ae7c;
L8c65ae7c: r[15]+=4u; /* add #0x4,r15 */
 goto L8c65ae7e;
L8c65ae7e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65ae80;
L8c65ae80: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c65ae84: r[0]=sx(m,r[14]+0x2u,2); /* mov.w @(0x2,r14),r0 */
 goto L8c65ae86;
L8c65ae86: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c65ae88;
L8c65ae88: r[3]=r[0]; /* mov r0,r3 */
 goto L8c65ae8a;
L8c65ae8a: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r3 */
 goto L8c65ae8c;
L8c65ae8c: r[1]=r[0]; /* mov r0,r1 */
 goto L8c65ae8e;
L8c65ae8e: r[2]=sx(m,0x8c65af42u,2); /* mov.w 0x8c65af42,r2 */
 goto L8c65ae90;
L8c65ae90: r[1]<<=8; /* shll8 r1 */
 goto L8c65ae92;
L8c65ae92: r[4]=r[0]; /* mov r0,r4 */
 goto L8c65ae94;
L8c65ae94: r[1]&=r[2]; /* and r2,r1 */
 goto L8c65ae96;
L8c65ae96: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c65ae98;
L8c65ae98: r[3]|=r[1]; /* or r1,r3 */
 goto L8c65ae9a;
L8c65ae9a: r[4]=4u; /* mov #0x4,r4 */
 goto L8c65ae9c;
L8c65ae9c: r[3]+=4u; /* add #0x4,r3 */
 goto L8c65ae9e;
L8c65ae9e: r[0]=r[4]; /* mov r4,r0 */
 goto L8c65aea0;
L8c65aea0: wr(m,r[6],r[3],2); /* mov.w r3,@r6 */
 goto L8c65aea2;
L8c65aea2: r[4]+=1u; /* add #0x1,r4 */
 goto L8c65aea4;
L8c65aea4: r[3]=sx(m,r[14]+r[0],1); /* mov.b @(r0,r14),r3 */
 goto L8c65aea6;
L8c65aea6: r[0]=r[4]; /* mov r4,r0 */
 goto L8c65aea8;
L8c65aea8: r[4]+=1u; /* add #0x1,r4 */
 goto L8c65aeaa;
L8c65aeaa: wr(m,r[7],r[3],1); /* mov.b r3,@r7 */
 goto L8c65aeac;
L8c65aeac: r[3]=sx(m,r[14]+r[0],1); /* mov.b @(r0,r14),r3 */
 goto L8c65aeae;
L8c65aeae: r[0]=r[4]; /* mov r4,r0 */
 goto L8c65aeb0;
L8c65aeb0: r[1]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r1 */
 goto L8c65aeb2;
L8c65aeb2: r[4]+=1u; /* add #0x1,r4 */
 goto L8c65aeb4;
L8c65aeb4: wr(m,r[1],r[3],1); /* mov.b r3,@r1 */
 goto L8c65aeb6;
L8c65aeb6: r[3]=sx(m,r[14]+r[0],1); /* mov.b @(r0,r14),r3 */
 goto L8c65aeb8;
L8c65aeb8: r[0]=r[4]; /* mov r4,r0 */
 goto L8c65aeba;
L8c65aeba: r[1]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r1 */
 goto L8c65aebc;
L8c65aebc: r[4]+=1u; /* add #0x1,r4 */
 goto L8c65aebe;
L8c65aebe: wr(m,r[1],r[3],1); /* mov.b r3,@r1 */
 goto L8c65aec0;
L8c65aec0: r[3]=sx(m,r[14]+r[0],1); /* mov.b @(r0,r14),r3 */
 goto L8c65aec2;
L8c65aec2: r[1]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r1 */
 goto L8c65aec4;
L8c65aec4: r[0]=r[4]; /* mov r4,r0 */
 goto L8c65aec6;
L8c65aec6: wr(m,r[1],r[3],1); /* mov.b r3,@r1 */
 goto L8c65aec8;
L8c65aec8: r[6]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r6 */
 goto L8c65aeca;
L8c65aeca: r[0]=4294967288u; /* mov #-0x8,r0 */
 goto L8c65aecc;
L8c65aecc: r[7]=rd(m,0x8c65af5cu,4); /* mov.l 0x8c65af5c,r7 */
 goto L8c65aece;
L8c65aece: r[4]+=4u; /* add #0x4,r4 */
 goto L8c65aed0;
L8c65aed0: r[1]=r[6]; /* mov r6,r1 */
 goto L8c65aed2;
L8c65aed2: r[1]>>=16; /* shlr16 r1 */
 goto L8c65aed4;
L8c65aed4: r[2]=r[6]; /* mov r6,r2 */
 goto L8c65aed6;
L8c65aed6: r[1]>>=8; /* shlr8 r1 */
 goto L8c65aed8;
L8c65aed8: {uint32_t shift=r[0];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}} /* shad r0,r2 */
 goto L8c65aeda;
L8c65aeda: r[0]=r[4]; /* mov r4,r0 */
 goto L8c65aedc;
L8c65aedc: r[1]=(uint32_t)(int32_t)(int8_t)r[1]; /* exts.b r1,r1 */
 goto L8c65aede;
L8c65aede: r[3]=rd(m,r[15]+0x18u,4); /* mov.l @(0x18,r15),r3 */
 goto L8c65aee0;
L8c65aee0: r[2]&=r[5]; /* and r5,r2 */
 goto L8c65aee2;
L8c65aee2: r[1]=r[1]&255u; /* extu.b r1,r1 */
 goto L8c65aee4;
L8c65aee4: r[1]|=r[2]; /* or r2,r1 */
 goto L8c65aee6;
L8c65aee6: r[2]=r[6]; /* mov r6,r2 */
 goto L8c65aee8;
L8c65aee8: r[2]<<=8; /* shll8 r2 */
 goto L8c65aeea;
L8c65aeea: r[2]&=r[7]; /* and r7,r2 */
 goto L8c65aeec;
L8c65aeec: r[1]|=r[2]; /* or r2,r1 */
 goto L8c65aeee;
L8c65aeee: r[2]=r[6]; /* mov r6,r2 */
 goto L8c65aef0;
L8c65aef0: r[2]<<=16; /* shll16 r2 */
 goto L8c65aef2;
L8c65aef2: r[2]<<=8; /* shll8 r2 */
 goto L8c65aef4;
L8c65aef4: r[1]|=r[2]; /* or r2,r1 */
 goto L8c65aef6;
L8c65aef6: wr(m,r[3],r[1],4); /* mov.l r1,@r3 */
 goto L8c65aef8;
L8c65aef8: r[6]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r6 */
 goto L8c65aefa;
L8c65aefa: r[3]=rd(m,r[15]+0x1cu,4); /* mov.l @(0x1c,r15),r3 */
 goto L8c65aefc;
L8c65aefc: r[2]=r[6]; /* mov r6,r2 */
 goto L8c65aefe;
L8c65aefe: r[2]>>=16; /* shlr16 r2 */
 goto L8c65af00;
L8c65af00: r[2]>>=8; /* shlr8 r2 */
 goto L8c65af02;
L8c65af02: r[2]=(uint32_t)(int32_t)(int8_t)r[2]; /* exts.b r2,r2 */
 goto L8c65af04;
L8c65af04: r[0]=4294967288u; /* mov #-0x8,r0 */
 goto L8c65af06;
L8c65af06: r[1]=r[6]; /* mov r6,r1 */
 goto L8c65af08;
L8c65af08: {uint32_t shift=r[0];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}} /* shad r0,r1 */
 goto L8c65af0a;
L8c65af0a: r[1]&=r[5]; /* and r5,r1 */
 goto L8c65af0c;
L8c65af0c: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c65af0e;
L8c65af0e: r[2]|=r[1]; /* or r1,r2 */
 goto L8c65af10;
L8c65af10: r[1]=r[6]; /* mov r6,r1 */
 goto L8c65af12;
L8c65af12: r[1]<<=8; /* shll8 r1 */
 goto L8c65af14;
L8c65af14: r[1]&=r[7]; /* and r7,r1 */
 goto L8c65af16;
L8c65af16: r[2]|=r[1]; /* or r1,r2 */
 goto L8c65af18;
L8c65af18: r[1]=r[6]; /* mov r6,r1 */
 goto L8c65af1a;
L8c65af1a: r[1]<<=16; /* shll16 r1 */
 goto L8c65af1c;
L8c65af1c: r[1]<<=8; /* shll8 r1 */
 goto L8c65af1e;
L8c65af1e: r[2]|=r[1]; /* or r1,r2 */
 goto L8c65af20;
L8c65af20: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c65af22;
L8c65af22: r[1]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r1 */
 goto L8c65af24;
L8c65af24: r[0]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r0 */
 goto L8c65af26;
L8c65af26: r[1]=sx(m,r[1],1); /* mov.b @r1,r1 */
 goto L8c65af28;
L8c65af28: r[2]=rd(m,0x8c65af60u,4); /* mov.l 0x8c65af60,r2 */
 goto L8c65af2a;
L8c65af2a: r[1]+=4294967294u; /* add #-0x2,r1 */
 goto L8c65af2c;
L8c65af2c: r[3]=rd(m,r[15]+0x20u,4); /* mov.l @(0x20,r15),r3 */
 goto L8c65af2e;
L8c65af2e: r[1]<<=2; /* shll2 r1 */
 goto L8c65af30;
L8c65af30: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c65af32;
L8c65af32: target=r[2];*pr=0x8c65af36u;r[0]=sx(m,r[0],1);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65af36;
L8c65af36: wr(m,r[3],r[0],4); /* mov.l r0,@r3 */
 goto L8c65af38;
L8c65af38: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65af3a;
L8c65af3a: r[15]+=4u; /* add #0x4,r15 */
 goto L8c65af3c;
L8c65af3c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65af3e;
L8c65af3e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
