#include "fpu_cleanup_transfer.h"
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
#include "interrupt_io.h"
/* Retail interrupt callback removal, 8c645c54..8c645dbc.
 * Hardware accesses use explicit callbacks; VBR is supplied by the host. */
static uint32_t rd(uint8_t *m,struct sa_interrupt_io *io,uint32_t a,unsigned n){
 uint32_t physical=a&0x1fffffffu,v=0;
 if(io->status)return 0;
 if(physical>=0x0c000000u && physical<=0x0d000000u-n){
  for(unsigned i=0;i<n;i++)v|=(uint32_t)m[physical-0x0c000000u+i]<<(i*8);
 }else if(n==4 && io->read32)io->status=io->read32(io->context,a,&v);
 else io->status=-3;
 return v;
}
static uint32_t sx(uint8_t *m,struct sa_interrupt_io *io,uint32_t a,unsigned n){
 uint32_t v=rd(m,io,a,n);
 return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;
}
static void wr(uint8_t *m,struct sa_interrupt_io *io,uint32_t a,uint32_t v,unsigned n){
 uint32_t physical=a&0x1fffffffu;
 if(io->status)return;
 if(physical>=0x0c000000u && physical<=0x0d000000u-n){
  for(unsigned i=0;i<n;i++)m[physical-0x0c000000u+i]=(uint8_t)(v>>(i*8));
 }else if(n==4 && io->write32)io->status=io->write32(io->context,a,v);
 else io->status=-3;
}
SA_STAGE_EXPORT int sa_interrupt_callback_remove(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,struct sa_interrupt_io *io,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 if(!io)return -3;io->status=0;uint32_t vbr=io->vbr;
 goto L8c645c54;
L8c645c54: if(io->status)return io->status; r[15]-=4;wr(m,io,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c645c56;
L8c645c56: if(io->status)return io->status; r[15]-=4;wr(m,io,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c645c58;
L8c645c58: if(io->status)return io->status; r[15]-=4;wr(m,io,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c645c5a;
L8c645c5a: if(io->status)return io->status; r[15]-=4;wr(m,io,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c645c5c;
L8c645c5c: if(io->status)return io->status; r[15]-=4;wr(m,io,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c645c5e;
L8c645c5e: if(io->status)return io->status; r[15]-=4;wr(m,io,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c645c60;
L8c645c60: if(io->status)return io->status; r[15]-=4;wr(m,io,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c645c62;
L8c645c62: if(io->status)return io->status; r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c645c64;
L8c645c64: if(io->status)return io->status; r[0]=*sr; /* stc SR,r0 */
 goto L8c645c66;
L8c645c66: if(io->status)return io->status; r[3]=sx(m,io,0x8c645d60u,2); /* mov.w 0x8c645d60,r3 */
 goto L8c645c68;
L8c645c68: if(io->status)return io->status; r[0]>>=2; /* shlr2 r0 */
 goto L8c645c6a;
L8c645c6a: if(io->status)return io->status; r[0]>>=2; /* shlr2 r0 */
 goto L8c645c6c;
L8c645c6c: if(io->status)return io->status; r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c645c6e;
L8c645c6e: if(io->status)return io->status; wr(m,io,r[15],r[0],2); /* mov.w r0,@r15 */
 goto L8c645c70;
L8c645c70: if(io->status)return io->status; r[0]=*sr; /* stc SR,r0 */
 goto L8c645c72;
L8c645c72: if(io->status)return io->status; r[0]&=r[3]; /* and r3,r0 */
 goto L8c645c74;
L8c645c74: if(io->status)return io->status; r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c645c76;
L8c645c76: if(io->status)return io->status; *sr=r[0]; /* ldc r0,SR */
 goto L8c645c78;
L8c645c78: if(io->status)return io->status; r[12]=r[4]; /* mov r4,r12 */
 goto L8c645c7a;
L8c645c7a: if(io->status)return io->status; r[12]<<=2; /* shll2 r12 */
 goto L8c645c7c;
L8c645c7c: if(io->status)return io->status; r[7]=rd(m,io,0x8c645d70u,4); /* mov.l 0x8c645d70,r7 */
 goto L8c645c7e;
L8c645c7e: if(io->status)return io->status; r[12]<<=2; /* shll2 r12 */
 goto L8c645c80;
L8c645c80: if(io->status)return io->status; r[13]=r[12]; /* mov r12,r13 */
 goto L8c645c82;
L8c645c82: if(io->status)return io->status; r[4]=sx(m,io,0x8c645d62u,2); /* mov.w 0x8c645d62,r4 */
 goto L8c645c84;
L8c645c84: if(io->status)return io->status; r[5]=1u; /* mov #0x1,r5 */
 goto L8c645c86;
L8c645c86: if(io->status)return io->status; r[6]=r[5]; /* mov r5,r6 */
 goto L8c645c88;
L8c645c88: if(io->status)return io->status; r[13]+=r[7]; /* add r7,r13 */
 goto L8c645c8a;
L8c645c8a: if(io->status)return io->status; r[14]=r[6]; /* mov r6,r14 */
 goto L8c645c8c;
L8c645c8c: if(io->status)return io->status; r[14]<<=2; /* shll2 r14 */
 goto L8c645c8e;
L8c645c8e: if(io->status)return io->status; r[14]<<=2; /* shll2 r14 */
 goto L8c645c90;
L8c645c90: if(io->status)return io->status; r[14]+=r[7]; /* add r7,r14 */
 goto L8c645c92;
L8c645c92: if(io->status)return io->status; r[3]=rd(m,io,r[14]+0x4u,4); /* mov.l @(0x4,r14),r3 */
 goto L8c645c94;
L8c645c94: if(io->status)return io->status; *sr=(*sr&~1u)|(r[13]==r[3]); /* cmp/eq r13,r3 */
 goto L8c645c96;
L8c645c96: if(io->status)return io->status; if(!(*sr&1u))goto L8c645ca0; /* bf 0x8c645ca0 */
 goto L8c645c98;
L8c645c98: if(io->status)return io->status; r[2]=rd(m,io,r[14]+0xcu,4); /* mov.l @(0xc,r14),r2 */
 goto L8c645c9a;
L8c645c9a: if(io->status)return io->status; r[3]=rd(m,io,0x8c645d74u,4); /* mov.l 0x8c645d74,r3 */
 goto L8c645c9c;
L8c645c9c: if(io->status)return io->status; *sr=(*sr&~1u)|!(r[3]&r[2]); /* tst r3,r2 */
 goto L8c645c9e;
L8c645c9e: if(io->status)return io->status; if(!(*sr&1u))goto L8c645ca6; /* bf 0x8c645ca6 */
 goto L8c645ca0;
L8c645ca0: if(io->status)return io->status; r[6]+=1u; /* add #0x1,r6 */
 goto L8c645ca2;
L8c645ca2: if(io->status)return io->status; *sr=(*sr&~1u)|(r[6]>=r[4]); /* cmp/hs r4,r6 */
 goto L8c645ca4;
L8c645ca4: if(io->status)return io->status; if(!(*sr&1u))goto L8c645c8a; /* bf 0x8c645c8a */
 goto L8c645ca6;
L8c645ca6: if(io->status)return io->status; *sr=(*sr&~1u)|(r[4]==r[6]); /* cmp/eq r4,r6 */
 goto L8c645ca8;
L8c645ca8: if(io->status)return io->status; take=(*sr&1u);r[1]=0u;if(take)goto L8c645cbe;goto L8c645cac; /* bt/s 0x8c645cbe */
 goto L8c645cac;
L8c645cac: if(io->status)return io->status; r[4]=r[12]; /* mov r12,r4 */
 goto L8c645cae;
L8c645cae: if(io->status)return io->status; r[6]<<=2; /* shll2 r6 */
 goto L8c645cb0;
L8c645cb0: if(io->status)return io->status; r[4]+=r[7]; /* add r7,r4 */
 goto L8c645cb2;
L8c645cb2: if(io->status)return io->status; r[3]=rd(m,io,r[4]+0x4u,4); /* mov.l @(0x4,r4),r3 */
 goto L8c645cb4;
L8c645cb4: if(io->status)return io->status; r[6]<<=2; /* shll2 r6 */
 goto L8c645cb6;
L8c645cb6: if(io->status)return io->status; r[6]+=r[7]; /* add r7,r6 */
 goto L8c645cb8;
L8c645cb8: if(io->status)return io->status; wr(m,io,r[6]+0x4u,r[3],4); /* mov.l r3,@(0x4,r6) */
 goto L8c645cba;
L8c645cba: if(io->status)return io->status; wr(m,io,r[4]+0xcu,r[1],4);goto L8c645d98; /* bra 0x8c645d98 */
 goto L8c645cbe;
L8c645cbe: if(io->status)return io->status; r[13]=r[12]; /* mov r12,r13 */
 goto L8c645cc0;
L8c645cc0: if(io->status)return io->status; r[13]+=r[7]; /* add r7,r13 */
 goto L8c645cc2;
L8c645cc2: if(io->status)return io->status; r[7]=sx(m,io,0x8c645d68u,2); /* mov.w 0x8c645d68,r7 */
 goto L8c645cc4;
L8c645cc4: if(io->status)return io->status; r[14]=r[5]; /* mov r5,r14 */
 goto L8c645cc6;
L8c645cc6: if(io->status)return io->status; r[10]=rd(m,io,0x8c645d7cu,4); /* mov.l 0x8c645d7c,r10 */
 goto L8c645cc8;
L8c645cc8: if(io->status)return io->status; r[4]=rd(m,io,0x8c645d80u,4); /* mov.l 0x8c645d80,r4 */
 goto L8c645cca;
L8c645cca: if(io->status)return io->status; r[9]=rd(m,io,0x8c645d78u,4); /* mov.l 0x8c645d78,r9 */
 goto L8c645ccc;
L8c645ccc: if(io->status)return io->status; r[12]=sx(m,io,0x8c645d64u,2); /* mov.w 0x8c645d64,r12 */
 goto L8c645cce;
L8c645cce: if(io->status)return io->status; r[8]=sx(m,io,0x8c645d66u,2); /* mov.w 0x8c645d66,r8 */
 goto L8c645cd0;
L8c645cd0: if(io->status)return io->status; r[0]=r[14]; /* mov r14,r0 */
 goto L8c645cd2;
L8c645cd2: if(io->status)return io->status; r[0]<<=2; /* shll2 r0 */
 goto L8c645cd4;
L8c645cd4: if(io->status)return io->status; r[3]=rd(m,io,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c645cd6;
L8c645cd6: if(io->status)return io->status; *sr=(*sr&~1u)|(r[13]==r[3]); /* cmp/eq r13,r3 */
 goto L8c645cd8;
L8c645cd8: if(io->status)return io->status; if(!(*sr&1u))goto L8c645d90; /* bf 0x8c645d90 */
 goto L8c645cda;
L8c645cda: if(io->status)return io->status; r[6]=r[14]; /* mov r14,r6 */
 goto L8c645cdc;
L8c645cdc: if(io->status)return io->status; r[6]<<=2; /* shll2 r6 */
 goto L8c645cde;
L8c645cde: if(io->status)return io->status; r[6]+=r[4]; /* add r4,r6 */
 goto L8c645ce0;
L8c645ce0: if(io->status)return io->status; r[3]=rd(m,io,r[13]+0x4u,4); /* mov.l @(0x4,r13),r3 */
 goto L8c645ce2;
L8c645ce2: if(io->status)return io->status; wr(m,io,r[6],r[3],4); /* mov.l r3,@r6 */
 goto L8c645ce4;
L8c645ce4: if(io->status)return io->status; wr(m,io,r[13]+0xcu,r[1],4); /* mov.l r1,@(0xc,r13) */
 goto L8c645ce6;
L8c645ce6: if(io->status)return io->status; r[2]=rd(m,io,r[6],4); /* mov.l @r6,r2 */
 goto L8c645ce8;
L8c645ce8: if(io->status)return io->status; *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c645cea;
L8c645cea: if(io->status)return io->status; if(!(*sr&1u))goto L8c645d96; /* bf 0x8c645d96 */
 goto L8c645cec;
L8c645cec: if(io->status)return io->status; *sr=(*sr&~1u)|(r[7]==r[14]); /* cmp/eq r7,r14 */
 goto L8c645cee;
L8c645cee: if(io->status)return io->status; if(!(*sr&1u))goto L8c645cfc; /* bf 0x8c645cfc */
 goto L8c645cf0;
L8c645cf0: if(io->status)return io->status; r[2]=rd(m,io,0x8c645d84u,4); /* mov.l 0x8c645d84,r2 */
 goto L8c645cf2;
L8c645cf2: if(io->status)return io->status; r[5]=0u; /* mov #0x0,r5 */
 goto L8c645cf4;
L8c645cf4: if(io->status)return io->status; target=r[2];*pr=0x8c645cf8u;r[4]=r[5];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c645cf8;
L8c645cf8: if(io->status)return io->status; goto L8c645d96; /* bra 0x8c645d96 */
 goto L8c645cfc;
L8c645cfc: if(io->status)return io->status; r[2]=sx(m,io,0x8c645d6au,2); /* mov.w 0x8c645d6a,r2 */
 goto L8c645cfe;
L8c645cfe: if(io->status)return io->status; *sr=(*sr&~1u)|(r[2]==r[14]); /* cmp/eq r2,r14 */
 goto L8c645d00;
L8c645d00: if(io->status)return io->status; if(!(*sr&1u))goto L8c645d0e; /* bf 0x8c645d0e */
 goto L8c645d02;
L8c645d02: if(io->status)return io->status; r[1]=rd(m,io,0x8c645d88u,4); /* mov.l 0x8c645d88,r1 */
 goto L8c645d04;
L8c645d04: if(io->status)return io->status; r[5]=0u; /* mov #0x0,r5 */
 goto L8c645d06;
L8c645d06: if(io->status)return io->status; target=r[1];*pr=0x8c645d0au;r[4]=r[5];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c645d0a;
L8c645d0a: if(io->status)return io->status; goto L8c645d96; /* bra 0x8c645d96 */
 goto L8c645d0e;
L8c645d0e: if(io->status)return io->status; r[13]=r[14]; /* mov r14,r13 */
 goto L8c645d10;
L8c645d10: if(io->status)return io->status; r[13]<<=2; /* shll2 r13 */
 goto L8c645d12;
L8c645d12: if(io->status)return io->status; r[13]<<=2; /* shll2 r13 */
 goto L8c645d14;
L8c645d14: if(io->status)return io->status; r[4]=r[13]; /* mov r13,r4 */
 goto L8c645d16;
L8c645d16: if(io->status)return io->status; r[7]=r[4]&255u; /* extu.b r4,r7 */
 goto L8c645d18;
L8c645d18: if(io->status)return io->status; r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c645d1a;
L8c645d1a: if(io->status)return io->status; r[6]=(uint32_t)(int32_t)(int16_t)r[4]; /* exts.w r4,r6 */
 goto L8c645d1c;
L8c645d1c: if(io->status)return io->status; r[7]>>=2; /* shlr2 r7 */
 goto L8c645d1e;
L8c645d1e: if(io->status)return io->status; {uint32_t shift=r[3];if(!(shift&0x80000000u))r[6]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[6]=n?(uint32_t)((int32_t)r[6]>>n):((r[6]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r6 */
 goto L8c645d20;
L8c645d20: if(io->status)return io->status; r[0]=r[6]; /* mov r6,r0 */
 goto L8c645d22;
L8c645d22: if(io->status)return io->status; r[7]>>=2; /* shlr2 r7 */
 goto L8c645d24;
L8c645d24: if(io->status)return io->status; *sr=(*sr&~1u)|(r[7]&1u);r[7]>>=1; /* shlr r7 */
 goto L8c645d26;
L8c645d26: if(io->status)return io->status; *sr=(*sr&~1u)|(r[0]==0xau); /* cmp/eq #0xa,r0 */
 goto L8c645d28;
L8c645d28: if(io->status)return io->status; if(!(*sr&1u))goto L8c645d3a; /* bf 0x8c645d3a */
 goto L8c645d2a;
L8c645d2a: if(io->status)return io->status; r[3]=r[5]; /* mov r5,r3 */
 goto L8c645d2c;
L8c645d2c: if(io->status)return io->status; r[7]+=12u; /* add #0xc,r7 */
 goto L8c645d2e;
L8c645d2e: if(io->status)return io->status; r[1]=rd(m,io,r[10],4); /* mov.l @r10,r1 */
 goto L8c645d30;
L8c645d30: if(io->status)return io->status; {uint32_t shift=r[7];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r7,r3 */
 goto L8c645d32;
L8c645d32: if(io->status)return io->status; r[3]=~r[3]; /* not r3,r3 */
 goto L8c645d34;
L8c645d34: if(io->status)return io->status; r[1]&=r[3]; /* and r3,r1 */
 goto L8c645d36;
L8c645d36: if(io->status)return io->status; wr(m,io,r[10],r[1],4);goto L8c645d4a; /* bra 0x8c645d4a */
 goto L8c645d3a;
L8c645d3a: if(io->status)return io->status; *sr=(*sr&~1u)|(r[0]==0xbu); /* cmp/eq #0xb,r0 */
 goto L8c645d3c;
L8c645d3c: if(io->status)return io->status; if(!(*sr&1u))goto L8c645d4a; /* bf 0x8c645d4a */
 goto L8c645d3e;
L8c645d3e: if(io->status)return io->status; r[3]=r[5]; /* mov r5,r3 */
 goto L8c645d40;
L8c645d40: if(io->status)return io->status; r[1]=rd(m,io,r[9],4); /* mov.l @r9,r1 */
 goto L8c645d42;
L8c645d42: if(io->status)return io->status; {uint32_t shift=r[7];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r7,r3 */
 goto L8c645d44;
L8c645d44: if(io->status)return io->status; r[3]=~r[3]; /* not r3,r3 */
 goto L8c645d46;
L8c645d46: if(io->status)return io->status; r[1]&=r[3]; /* and r3,r1 */
 goto L8c645d48;
L8c645d48: if(io->status)return io->status; wr(m,io,r[9],r[1],4); /* mov.l r1,@r9 */
 goto L8c645d4a;
L8c645d4a: if(io->status)return io->status; r[2]=sx(m,io,0x8c645d6cu,2); /* mov.w 0x8c645d6c,r2 */
 goto L8c645d4c;
L8c645d4c: if(io->status)return io->status; r[3]=vbr; /* stc VBR,r3 */
 goto L8c645d4e;
L8c645d4e: if(io->status)return io->status; r[13]+=r[2]; /* add r2,r13 */
 goto L8c645d50;
L8c645d50: if(io->status)return io->status; r[0]=rd(m,io,0x8c645d8cu,4); /* mov.l 0x8c645d8c,r0 */
 goto L8c645d52;
L8c645d52: if(io->status)return io->status; r[13]>>=2; /* shlr2 r13 */
 goto L8c645d54;
L8c645d54: if(io->status)return io->status; *sr=(*sr&~1u)|(r[13]&1u);r[13]>>=1; /* shlr r13 */
 goto L8c645d56;
L8c645d56: if(io->status)return io->status; r[3]+=r[13]; /* add r13,r3 */
 goto L8c645d58;
L8c645d58: if(io->status)return io->status; r[3]+=r[8]; /* add r8,r3 */
 goto L8c645d5a;
L8c645d5a: if(io->status)return io->status; wr(m,io,r[3],r[0],4); /* mov.l r0,@r3 */
 goto L8c645d5c;
L8c645d5c: if(io->status)return io->status; goto L8c645d96; /* bra 0x8c645d96 */
 goto L8c645d90;
L8c645d90: if(io->status)return io->status; r[14]+=1u; /* add #0x1,r14 */
 goto L8c645d92;
L8c645d92: if(io->status)return io->status; *sr=(*sr&~1u)|(r[14]>=r[12]); /* cmp/hs r12,r14 */
 goto L8c645d94;
L8c645d94: if(io->status)return io->status; if(!(*sr&1u))goto L8c645cd0; /* bf 0x8c645cd0 */
 goto L8c645d96;
L8c645d96: if(io->status)return io->status; *sr=(*sr&~1u)|(r[12]==r[14]); /* cmp/eq r12,r14 */
 goto L8c645d98;
L8c645d98: if(io->status)return io->status; r[0]=sx(m,io,r[15],2); /* mov.w @r15,r0 */
 goto L8c645d9a;
L8c645d9a: if(io->status)return io->status; r[3]=*sr; /* stc SR,r3 */
 goto L8c645d9c;
L8c645d9c: if(io->status)return io->status; r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c645d9e;
L8c645d9e: if(io->status)return io->status; r[2]=sx(m,io,0x8c645dbeu,2); /* mov.w 0x8c645dbe,r2 */
 goto L8c645da0;
L8c645da0: if(io->status)return io->status; r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c645da2;
L8c645da2: if(io->status)return io->status; r[0]<<=2; /* shll2 r0 */
 goto L8c645da4;
L8c645da4: if(io->status)return io->status; r[3]&=r[2]; /* and r2,r3 */
 goto L8c645da6;
L8c645da6: if(io->status)return io->status; r[0]<<=2; /* shll2 r0 */
 goto L8c645da8;
L8c645da8: if(io->status)return io->status; r[0]|=r[3]; /* or r3,r0 */
 goto L8c645daa;
L8c645daa: if(io->status)return io->status; *sr=r[0]; /* ldc r0,SR */
 goto L8c645dac;
L8c645dac: if(io->status)return io->status; r[15]+=4u; /* add #0x4,r15 */
 goto L8c645dae;
L8c645dae: if(io->status)return io->status; *pr=rd(m,io,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c645db0;
L8c645db0: if(io->status)return io->status; r[8]=rd(m,io,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c645db2;
L8c645db2: if(io->status)return io->status; r[9]=rd(m,io,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c645db4;
L8c645db4: if(io->status)return io->status; r[10]=rd(m,io,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c645db6;
L8c645db6: if(io->status)return io->status; r[12]=rd(m,io,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c645db8;
L8c645db8: if(io->status)return io->status; r[13]=rd(m,io,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c645dba;
L8c645dba: if(io->status)return io->status; r[14]=rd(m,io,r[15],4);r[15]+=4;return io->status; /* rts */
}
