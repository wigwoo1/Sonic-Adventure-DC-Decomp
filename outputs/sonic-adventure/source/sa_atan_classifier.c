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
#include "sh4_compare_bits.h"
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){return sa_sh4_compare_bits(a,b,&f->fpscr,sr,1);}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){return sa_sh4_compare_bits(a,b,&f->fpscr,sr,0);}
/* Static C recovery of retail Tikal helper 8c10fbb8..8c10fc9c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_atan_classifier(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10fbb8;
L8c10fbb8: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c10fbba;
L8c10fbba: r[15]+=4294967268u; /* add #-0x1c,r15 */
 goto L8c10fbbc;
L8c10fbbc: r[6]=r[15]; /* mov r15,r6 */
 goto L8c10fbbe;
L8c10fbbe: r[6]+=24u; /* add #0x18,r6 */
 goto L8c10fbc0;
L8c10fbc0: r[5]=rd(m,0x8c10fc9cu,4); /* mov.l 0x8c10fc9c,r5 */
 goto L8c10fbc2;
L8c10fbc2: r[14]=r[15]; /* mov r15,r14 */
 goto L8c10fbc4;
L8c10fbc4: r[14]+=20u; /* add #0x14,r14 */
 goto L8c10fbc6;
L8c10fbc6: wr(m,r[6],fr[4],4); /* fmov.s fr4,@r6 */
 goto L8c10fbc8;
L8c10fbc8: wr(m,r[14],fr[5],4); /* fmov.s fr5,@r14 */
 goto L8c10fbca;
L8c10fbca: r[7]=r[15]; /* mov r15,r7 */
 goto L8c10fbcc;
L8c10fbcc: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c10fbce;
L8c10fbce: r[3]&=r[5]; /* and r5,r3 */
 goto L8c10fbd0;
L8c10fbd0: *sr=(*sr&~1u)|(r[5]==r[3]); /* cmp/eq r5,r3 */
 goto L8c10fbd2;
L8c10fbd2: take=(*sr&1u);r[7]+=16u;if(take)goto L8c10fbf0;goto L8c10fbd6; /* bt/s 0x8c10fbf0 */
 goto L8c10fbd6;
L8c10fbd6: r[3]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r3 */
 goto L8c10fbd8;
L8c10fbd8: r[3]&=r[5]; /* and r5,r3 */
 goto L8c10fbda;
L8c10fbda: *sr=(*sr&~1u)|(r[5]==r[3]); /* cmp/eq r5,r3 */
 goto L8c10fbdc;
L8c10fbdc: if(!(*sr&1u))goto L8c10fbe6; /* bf 0x8c10fbe6 */
 goto L8c10fbde;
L8c10fbde: r[2]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r2 */
 goto L8c10fbe0;
L8c10fbe0: r[3]=rd(m,0x8c10fca0u,4); /* mov.l 0x8c10fca0,r3 */
 goto L8c10fbe2;
L8c10fbe2: *sr=(*sr&~1u)|!(r[3]&r[2]); /* tst r3,r2 */
 goto L8c10fbe4;
L8c10fbe4: if(!(*sr&1u))goto L8c10fbf0; /* bf 0x8c10fbf0 */
 goto L8c10fbe6;
L8c10fbe6: r[0]=20u; /* mov #0x14,r0 */
 goto L8c10fbe8;
L8c10fbe8: fr[3]=0; /* fldi0 fr3 */
 goto L8c10fbea;
L8c10fbea: fr[2]=rd(m,r[0]+r[15],4); /* fmov.s @(r0,r15),fr2 */
 goto L8c10fbec;
L8c10fbec: status=eq(f,sr,fr[2],fr[3]);if(status)return status; /* fcmp/eq fr3,fr2 */
 goto L8c10fbee;
L8c10fbee: if(!(*sr&1u))goto L8c10fbfc; /* bf 0x8c10fbfc */
 goto L8c10fbf0;
L8c10fbf0: r[3]=rd(m,0x8c10fca4u,4); /* mov.l 0x8c10fca4,r3 */
 goto L8c10fbf2;
L8c10fbf2: r[2]=0u; /* mov #0x0,r2 */
 goto L8c10fbf4;
L8c10fbf4: wr(m,r[4],r[2],4); /* mov.l r2,@r4 */
 goto L8c10fbf6;
L8c10fbf6: fr[3]=rd(m,r[3],4); /* fmov.s @r3,fr3 */
 goto L8c10fbf8;
L8c10fbf8: wr(m,r[7],fr[3],4);goto L8c10fc94; /* bra 0x8c10fc94 */
 goto L8c10fbfc;
L8c10fbfc: r[1]=rd(m,r[14],4); /* mov.l @r14,r1 */
 goto L8c10fbfe;
L8c10fbfe: r[1]&=r[5]; /* and r5,r1 */
 goto L8c10fc00;
L8c10fc00: *sr=(*sr&~1u)|(r[5]==r[1]); /* cmp/eq r5,r1 */
 goto L8c10fc02;
L8c10fc02: if(!(*sr&1u))goto L8c10fc0c; /* bf 0x8c10fc0c */
 goto L8c10fc04;
L8c10fc04: r[2]=0u; /* mov #0x0,r2 */
 goto L8c10fc06;
L8c10fc06: wr(m,r[4],r[2],4); /* mov.l r2,@r4 */
 goto L8c10fc08;
L8c10fc08: wr(m,r[7],fr[4],4);goto L8c10fc94; /* bra 0x8c10fc94 */
 goto L8c10fc0c;
L8c10fc0c: r[2]=rd(m,r[6],4); /* mov.l @r6,r2 */
 goto L8c10fc0e;
L8c10fc0e: r[3]=rd(m,0x8c10fca8u,4); /* mov.l 0x8c10fca8,r3 */
 goto L8c10fc10;
L8c10fc10: r[2]&=r[3]; /* and r3,r2 */
 goto L8c10fc12;
L8c10fc12: wr(m,r[15]+0x8u,r[2],4); /* mov.l r2,@(0x8,r15) */
 goto L8c10fc14;
L8c10fc14: r[1]=rd(m,r[6],4); /* mov.l @r6,r1 */
 goto L8c10fc16;
L8c10fc16: r[2]=rd(m,0x8c10fcacu,4); /* mov.l 0x8c10fcac,r2 */
 goto L8c10fc18;
L8c10fc18: r[1]&=r[2]; /* and r2,r1 */
 goto L8c10fc1a;
L8c10fc1a: wr(m,r[6],r[1],4); /* mov.l r1,@r6 */
 goto L8c10fc1c;
L8c10fc1c: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c10fc1e;
L8c10fc1e: r[0]&=r[2]; /* and r2,r0 */
 goto L8c10fc20;
L8c10fc20: wr(m,r[14],r[0],4); /* mov.l r0,@r14 */
 goto L8c10fc22;
L8c10fc22: r[0]=12u; /* mov #0xc,r0 */
 goto L8c10fc24;
L8c10fc24: fr[3]=rd(m,r[14],4); /* fmov.s @r14,fr3 */
 goto L8c10fc26;
L8c10fc26: fr[4]=rd(m,r[6],4); /* fmov.s @r6,fr4 */
 goto L8c10fc28;
L8c10fc28: status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status; /* fdiv fr3,fr4 */
 goto L8c10fc2a;
L8c10fc2a: wr(m,r[0]+r[15],fr[4],4); /* fmov fr4,@(r0,r15) */
 goto L8c10fc2c;
L8c10fc2c: r[1]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r1 */
 goto L8c10fc2e;
L8c10fc2e: wr(m,r[15],r[1],4); /* mov.l r1,@r15 */
 goto L8c10fc30;
L8c10fc30: r[1]&=r[3]; /* and r3,r1 */
 goto L8c10fc32;
L8c10fc32: wr(m,r[15]+0x4u,r[1],4); /* mov.l r1,@(0x4,r15) */
 goto L8c10fc34;
L8c10fc34: r[1]=rd(m,r[15],4); /* mov.l @r15,r1 */
 goto L8c10fc36;
L8c10fc36: r[3]=rd(m,0x8c10fcb0u,4); /* mov.l 0x8c10fcb0,r3 */
 goto L8c10fc38;
L8c10fc38: r[5]&=r[1]; /* and r1,r5 */
 goto L8c10fc3a;
L8c10fc3a: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[3]); /* cmp/ge r3,r5 */
 goto L8c10fc3c;
L8c10fc3c: if((*sr&1u))goto L8c10fc4a; /* bt 0x8c10fc4a */
 goto L8c10fc3e;
L8c10fc3e: status=sa_sh4_truncate_i32(fr[4],&f->fpscr,fpul);if(status)return status; /* ftrc fr4,FPUL */
 goto L8c10fc40;
L8c10fc40: r[0]=*fpul; /* sts FPUL,r0 */
 goto L8c10fc42;
L8c10fc42: *fpul=r[0]; /* lds r0,FPUL */
 goto L8c10fc44;
L8c10fc44: r[0]=12u; /* mov #0xc,r0 */
 goto L8c10fc46;
L8c10fc46: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto L8c10fc48;
L8c10fc48: wr(m,r[0]+r[15],fr[3],4); /* fmov fr3,@(r0,r15) */
 goto L8c10fc4a;
L8c10fc4a: r[0]=12u; /* mov #0xc,r0 */
 goto L8c10fc4c;
L8c10fc4c: r[2]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r2 */
 goto L8c10fc4e;
L8c10fc4e: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c10fc50;
L8c10fc50: r[2]|=r[3]; /* or r3,r2 */
 goto L8c10fc52;
L8c10fc52: wr(m,r[15]+0xcu,r[2],4); /* mov.l r2,@(0xc,r15) */
 goto L8c10fc54;
L8c10fc54: fr[0]=rd(m,r[0]+r[15],4); /* fmov.s @(r0,r15),fr0 */
 goto L8c10fc56;
L8c10fc56: r[0]=0x8c10fcb4u; /* mova 0x8c10fcb4,r0 */
 goto L8c10fc58;
L8c10fc58: fr[2]=rd(m,r[0],4); /* fmov.s @r0,fr2 */
 goto L8c10fc5a;
L8c10fc5a: status=gt(f,sr,fr[0],fr[2]);if(status)return status; /* fcmp/gt fr2,fr0 */
 goto L8c10fc5c;
L8c10fc5c: if(!(*sr&1u))goto L8c10fc68; /* bf 0x8c10fc68 */
 goto L8c10fc5e;
L8c10fc5e: r[0]=0x8c10fcb8u; /* mova 0x8c10fcb8,r0 */
 goto L8c10fc60;
L8c10fc60: fr[3]=fr[0]; /* fmov fr0,fr3 */
 goto L8c10fc62;
L8c10fc62: fr[1]=rd(m,r[0],4); /* fmov.s @r0,fr1 */
 goto L8c10fc64;
L8c10fc64: status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;goto L8c10fc6a; /* bra 0x8c10fc6a */
 goto L8c10fc68;
L8c10fc68: fr[3]=fr[0]; /* fmov fr0,fr3 */
 goto L8c10fc6a;
L8c10fc6a: status=sa_sh4_truncate_i32(fr[3],&f->fpscr,fpul);if(status)return status; /* ftrc fr3,FPUL */
 goto L8c10fc6c;
L8c10fc6c: r[2]=*fpul; /* sts FPUL,r2 */
 goto L8c10fc6e;
L8c10fc6e: r[3]=r[2]; /* mov r2,r3 */
 goto L8c10fc70;
L8c10fc70: *fpul=r[3]; /* lds r3,FPUL */
 goto L8c10fc72;
L8c10fc72: *sr=(*sr&~1u)|((int32_t)r[3]>=0); /* cmp/pz r3 */
 goto L8c10fc74;
L8c10fc74: wr(m,r[4],r[2],4); /* mov.l r2,@r4 */
 goto L8c10fc76;
L8c10fc76: take=(*sr&1u);status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status;if(take)goto L8c10fc82;goto L8c10fc7a; /* bt/s 0x8c10fc82 */
 goto L8c10fc7a;
L8c10fc7a: r[1]=rd(m,0x8c10fcb0u,4); /* mov.l 0x8c10fcb0,r1 */
 goto L8c10fc7c;
L8c10fc7c: *fpul=r[1]; /* lds r1,FPUL */
 goto L8c10fc7e;
L8c10fc7e: fr[2]=*fpul; /* fsts FPUL,fr2 */
 goto L8c10fc80;
L8c10fc80: status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status; /* fadd fr2,fr3 */
 goto L8c10fc82;
L8c10fc82: fr[2]=rd(m,r[14],4); /* fmov.s @r14,fr2 */
 goto L8c10fc84;
L8c10fc84: fr[1]=rd(m,r[6],4); /* fmov.s @r6,fr1 */
 goto L8c10fc86;
L8c10fc86: status=sa_sh4_multiply(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status; /* fmul fr2,fr3 */
 goto L8c10fc88;
L8c10fc88: status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status; /* fsub fr3,fr1 */
 goto L8c10fc8a;
L8c10fc8a: wr(m,r[7],fr[1],4); /* fmov.s fr1,@r7 */
 goto L8c10fc8c;
L8c10fc8c: r[2]=rd(m,r[7],4); /* mov.l @r7,r2 */
 goto L8c10fc8e;
L8c10fc8e: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c10fc90;
L8c10fc90: r[2]|=r[3]; /* or r3,r2 */
 goto L8c10fc92;
L8c10fc92: wr(m,r[7],r[2],4); /* mov.l r2,@r7 */
 goto L8c10fc94;
L8c10fc94: fr[0]=rd(m,r[7],4); /* fmov.s @r7,fr0 */
 goto L8c10fc96;
L8c10fc96: r[15]+=28u; /* add #0x1c,r15 */
 goto L8c10fc98;
L8c10fc98: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
