#include "fpu_cleanup_transfer.h"
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
#include "interrupt_io.h"
/* Retail interrupt callback constructor, 8c64594c..8c645af8.
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
SA_STAGE_EXPORT int sa_interrupt_callback_create(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,struct sa_interrupt_io *io,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 if(!io)return -3;io->status=0;uint32_t vbr=io->vbr;
 goto L8c64594c;
L8c64594c: if(io->status)return io->status; r[15]-=4;wr(m,io,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c64594e;
L8c64594e: if(io->status)return io->status; r[15]-=4;wr(m,io,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c645950;
L8c645950: if(io->status)return io->status; r[15]-=4;wr(m,io,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c645952;
L8c645952: if(io->status)return io->status; r[15]-=4;wr(m,io,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c645954;
L8c645954: if(io->status)return io->status; r[15]-=4;wr(m,io,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c645956;
L8c645956: if(io->status)return io->status; r[15]-=4;wr(m,io,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c645958;
L8c645958: if(io->status)return io->status; r[15]-=4;wr(m,io,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c64595a;
L8c64595a: if(io->status)return io->status; r[15]-=4;wr(m,io,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c64595c;
L8c64595c: if(io->status)return io->status; r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c64595e;
L8c64595e: if(io->status)return io->status; r[0]=r[4]; /* mov r4,r0 */
 goto L8c645960;
L8c645960: if(io->status)return io->status; wr(m,io,r[15]+0x4u,r[0],2); /* mov.w r0,@(0x4,r15) */
 goto L8c645962;
L8c645962: if(io->status)return io->status; r[0]=*sr; /* stc SR,r0 */
 goto L8c645964;
L8c645964: if(io->status)return io->status; r[3]=sx(m,io,0x8c6459fcu,2); /* mov.w 0x8c6459fc,r3 */
 goto L8c645966;
L8c645966: if(io->status)return io->status; r[8]=r[7]; /* mov r7,r8 */
 goto L8c645968;
L8c645968: if(io->status)return io->status; wr(m,io,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c64596a;
L8c64596a: if(io->status)return io->status; r[0]>>=2; /* shlr2 r0 */
 goto L8c64596c;
L8c64596c: if(io->status)return io->status; r[0]>>=2; /* shlr2 r0 */
 goto L8c64596e;
L8c64596e: if(io->status)return io->status; r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c645970;
L8c645970: if(io->status)return io->status; wr(m,io,r[15]+0x8u,r[0],2); /* mov.w r0,@(0x8,r15) */
 goto L8c645972;
L8c645972: if(io->status)return io->status; r[0]=*sr; /* stc SR,r0 */
 goto L8c645974;
L8c645974: if(io->status)return io->status; r[0]&=r[3]; /* and r3,r0 */
 goto L8c645976;
L8c645976: if(io->status)return io->status; r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c645978;
L8c645978: if(io->status)return io->status; *sr=r[0]; /* ldc r0,SR */
 goto L8c64597a;
L8c64597a: if(io->status)return io->status; r[0]=sx(m,io,r[15]+0x4u,2); /* mov.w @(0x4,r15),r0 */
 goto L8c64597c;
L8c64597c: if(io->status)return io->status; r[3]=4294967292u; /* mov #-0x4,r3 */
 goto L8c64597e;
L8c64597e: if(io->status)return io->status; r[9]=rd(m,io,0x8c645a10u,4); /* mov.l 0x8c645a10,r9 */
 goto L8c645980;
L8c645980: if(io->status)return io->status; r[11]=r[6]; /* mov r6,r11 */
 goto L8c645982;
L8c645982: if(io->status)return io->status; r[13]=r[0]; /* mov r0,r13 */
 goto L8c645984;
L8c645984: if(io->status)return io->status; r[0]=r[13]; /* mov r13,r0 */
 goto L8c645986;
L8c645986: if(io->status)return io->status; {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c645988;
L8c645988: if(io->status)return io->status; r[7]=rd(m,io,0x8c645a14u,4); /* mov.l 0x8c645a14,r7 */
 goto L8c64598a;
L8c64598a: if(io->status)return io->status; r[0]<<=2; /* shll2 r0 */
 goto L8c64598c;
L8c64598c: if(io->status)return io->status; r[4]=rd(m,io,r[9]+r[0],4); /* mov.l @(r0,r9),r4 */
 goto L8c64598e;
L8c64598e: if(io->status)return io->status; r[5]=r[4]; /* mov r4,r5 */
 goto L8c645990;
L8c645990: if(io->status)return io->status; r[11]|=r[7];goto L8c645998; /* bra 0x8c645998 */
 goto L8c645994;
L8c645994: if(io->status)return io->status; r[5]=r[4]; /* mov r4,r5 */
 goto L8c645996;
L8c645996: if(io->status)return io->status; r[4]=rd(m,io,r[4]+0x4u,4); /* mov.l @(0x4,r4),r4 */
 goto L8c645998;
L8c645998: if(io->status)return io->status; *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c64599a;
L8c64599a: if(io->status)return io->status; if((*sr&1u))goto L8c6459a2; /* bt 0x8c6459a2 */
 goto L8c64599c;
L8c64599c: if(io->status)return io->status; r[2]=rd(m,io,r[4]+0xcu,4); /* mov.l @(0xc,r4),r2 */
 goto L8c64599e;
L8c64599e: if(io->status)return io->status; *sr=(*sr&~1u)|(r[2]>=r[11]); /* cmp/hs r11,r2 */
 goto L8c6459a0;
L8c6459a0: if(io->status)return io->status; if(!(*sr&1u))goto L8c645994; /* bf 0x8c645994 */
 goto L8c6459a2;
L8c6459a2: if(io->status)return io->status; r[10]=rd(m,io,0x8c645a0cu,4); /* mov.l 0x8c645a0c,r10 */
 goto L8c6459a4;
L8c6459a4: if(io->status)return io->status; r[12]=1u; /* mov #0x1,r12 */
 goto L8c6459a6;
L8c6459a6: if(io->status)return io->status; r[6]=sx(m,io,0x8c645a02u,2); /* mov.w 0x8c645a02,r6 */
 goto L8c6459a8;
L8c6459a8: if(io->status)return io->status; r[14]=r[12]; /* mov r12,r14 */
 goto L8c6459aa;
L8c6459aa: if(io->status)return io->status; r[2]=r[14]; /* mov r14,r2 */
 goto L8c6459ac;
L8c6459ac: if(io->status)return io->status; r[2]<<=2; /* shll2 r2 */
 goto L8c6459ae;
L8c6459ae: if(io->status)return io->status; r[2]<<=2; /* shll2 r2 */
 goto L8c6459b0;
L8c6459b0: if(io->status)return io->status; r[2]+=r[10]; /* add r10,r2 */
 goto L8c6459b2;
L8c6459b2: if(io->status)return io->status; r[3]=rd(m,io,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c6459b4;
L8c6459b4: if(io->status)return io->status; *sr=(*sr&~1u)|!(r[7]&r[3]); /* tst r7,r3 */
 goto L8c6459b6;
L8c6459b6: if(io->status)return io->status; if((*sr&1u))goto L8c6459be; /* bt 0x8c6459be */
 goto L8c6459b8;
L8c6459b8: if(io->status)return io->status; r[14]+=1u; /* add #0x1,r14 */
 goto L8c6459ba;
L8c6459ba: if(io->status)return io->status; *sr=(*sr&~1u)|(r[14]>=r[6]); /* cmp/hs r6,r14 */
 goto L8c6459bc;
L8c6459bc: if(io->status)return io->status; if(!(*sr&1u))goto L8c6459aa; /* bf 0x8c6459aa */
 goto L8c6459be;
L8c6459be: if(io->status)return io->status; *sr=(*sr&~1u)|(r[14]>=r[6]); /* cmp/hs r6,r14 */
 goto L8c6459c0;
L8c6459c0: if(io->status)return io->status; if(!(*sr&1u))goto L8c6459c6; /* bf 0x8c6459c6 */
 goto L8c6459c2;
L8c6459c2: if(io->status)return io->status; goto L8c645ace; /* bra 0x8c645ace */
 goto L8c6459c6;
L8c6459c6: if(io->status)return io->status; *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c6459c8;
L8c6459c8: if(io->status)return io->status; if(!(*sr&1u))goto L8c645a9a; /* bf 0x8c645a9a */
 goto L8c6459ca;
L8c6459ca: if(io->status)return io->status; *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c6459cc;
L8c6459cc: if(io->status)return io->status; if(!(*sr&1u))goto L8c645a9a; /* bf 0x8c645a9a */
 goto L8c6459ce;
L8c6459ce: if(io->status)return io->status; r[4]=sx(m,io,0x8c645a04u,2); /* mov.w 0x8c645a04,r4 */
 goto L8c6459d0;
L8c6459d0: if(io->status)return io->status; *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[4]); /* cmp/ge r4,r13 */
 goto L8c6459d2;
L8c6459d2: if(io->status)return io->status; if((*sr&1u))goto L8c6459ec; /* bt 0x8c6459ec */
 goto L8c6459d4;
L8c6459d4: if(io->status)return io->status; r[3]=sx(m,io,0x8c645a06u,2); /* mov.w 0x8c645a06,r3 */
 goto L8c6459d6;
L8c6459d6: if(io->status)return io->status; r[2]=vbr; /* stc VBR,r2 */
 goto L8c6459d8;
L8c6459d8: if(io->status)return io->status; r[3]+=r[13]; /* add r13,r3 */
 goto L8c6459da;
L8c6459da: if(io->status)return io->status; r[0]=sx(m,io,0x8c645a08u,2); /* mov.w 0x8c645a08,r0 */
 goto L8c6459dc;
L8c6459dc: if(io->status)return io->status; *sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u); /* shar r3 */
 goto L8c6459de;
L8c6459de: if(io->status)return io->status; *sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u); /* shar r3 */
 goto L8c6459e0;
L8c6459e0: if(io->status)return io->status; *sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u); /* shar r3 */
 goto L8c6459e2;
L8c6459e2: if(io->status)return io->status; r[2]+=r[3]; /* add r3,r2 */
 goto L8c6459e4;
L8c6459e4: if(io->status)return io->status; r[3]=rd(m,io,0x8c645a18u,4); /* mov.l 0x8c645a18,r3 */
 goto L8c6459e6;
L8c6459e6: if(io->status)return io->status; wr(m,io,r[2]+r[0],r[3],4); /* mov.l r3,@(r0,r2) */
 goto L8c6459e8;
L8c6459e8: if(io->status)return io->status; goto L8c645a3e; /* bra 0x8c645a3e */
 goto L8c6459ec;
L8c6459ec: if(io->status)return io->status; *sr=(*sr&~1u)|(r[4]==r[13]); /* cmp/eq r4,r13 */
 goto L8c6459ee;
L8c6459ee: if(io->status)return io->status; if(!(*sr&1u))goto L8c645a24; /* bf 0x8c645a24 */
 goto L8c6459f0;
L8c6459f0: if(io->status)return io->status; r[2]=rd(m,io,0x8c645a20u,4); /* mov.l 0x8c645a20,r2 */
 goto L8c6459f2;
L8c6459f2: if(io->status)return io->status; r[4]=rd(m,io,0x8c645a1cu,4); /* mov.l 0x8c645a1c,r4 */
 goto L8c6459f4;
L8c6459f4: if(io->status)return io->status; target=r[2];*pr=0x8c6459f8u;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6459f8;
L8c6459f8: if(io->status)return io->status; goto L8c645a3e; /* bra 0x8c645a3e */
 goto L8c645a24;
L8c645a24: if(io->status)return io->status; r[2]=sx(m,io,0x8c645b44u,2); /* mov.w 0x8c645b44,r2 */
 goto L8c645a26;
L8c645a26: if(io->status)return io->status; *sr=(*sr&~1u)|(r[2]==r[13]); /* cmp/eq r2,r13 */
 goto L8c645a28;
L8c645a28: if(io->status)return io->status; if(!(*sr&1u))goto L8c645a36; /* bf 0x8c645a36 */
 goto L8c645a2a;
L8c645a2a: if(io->status)return io->status; r[1]=rd(m,io,0x8c645b50u,4); /* mov.l 0x8c645b50,r1 */
 goto L8c645a2c;
L8c645a2c: if(io->status)return io->status; r[4]=rd(m,io,0x8c645b4cu,4); /* mov.l 0x8c645b4c,r4 */
 goto L8c645a2e;
L8c645a2e: if(io->status)return io->status; target=r[1];*pr=0x8c645a32u;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c645a32;
L8c645a32: if(io->status)return io->status; goto L8c645a3e; /* bra 0x8c645a3e */
 goto L8c645a36;
L8c645a36: if(io->status)return io->status; r[1]=vbr; /* stc VBR,r1 */
 goto L8c645a38;
L8c645a38: if(io->status)return io->status; r[0]=sx(m,io,0x8c645b46u,2); /* mov.w 0x8c645b46,r0 */
 goto L8c645a3a;
L8c645a3a: if(io->status)return io->status; r[3]=rd(m,io,0x8c645b54u,4); /* mov.l 0x8c645b54,r3 */
 goto L8c645a3c;
L8c645a3c: if(io->status)return io->status; wr(m,io,r[1]+r[0],r[3],4); /* mov.l r3,@(r0,r1) */
 goto L8c645a3e;
L8c645a3e: if(io->status)return io->status; r[4]=r[14]; /* mov r14,r4 */
 goto L8c645a40;
L8c645a40: if(io->status)return io->status; r[4]<<=2; /* shll2 r4 */
 goto L8c645a42;
L8c645a42: if(io->status)return io->status; r[0]=r[13]; /* mov r13,r0 */
 goto L8c645a44;
L8c645a44: if(io->status)return io->status; r[3]=4294967292u; /* mov #-0x4,r3 */
 goto L8c645a46;
L8c645a46: if(io->status)return io->status; r[4]<<=2; /* shll2 r4 */
 goto L8c645a48;
L8c645a48: if(io->status)return io->status; {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c645a4a;
L8c645a4a: if(io->status)return io->status; r[4]+=r[10]; /* add r10,r4 */
 goto L8c645a4c;
L8c645a4c: if(io->status)return io->status; r[0]<<=2; /* shll2 r0 */
 goto L8c645a4e;
L8c645a4e: if(io->status)return io->status; wr(m,io,r[9]+r[0],r[4],4); /* mov.l r4,@(r0,r9) */
 goto L8c645a50;
L8c645a50: if(io->status)return io->status; r[3]=0u; /* mov #0x0,r3 */
 goto L8c645a52;
L8c645a52: if(io->status)return io->status; wr(m,io,r[4]+0x4u,r[3],4); /* mov.l r3,@(0x4,r4) */
 goto L8c645a54;
L8c645a54: if(io->status)return io->status; r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c645a56;
L8c645a56: if(io->status)return io->status; r[2]=rd(m,io,r[15],4); /* mov.l @r15,r2 */
 goto L8c645a58;
L8c645a58: if(io->status)return io->status; wr(m,io,r[4],r[2],4); /* mov.l r2,@r4 */
 goto L8c645a5a;
L8c645a5a: if(io->status)return io->status; wr(m,io,r[4]+0x8u,r[8],4); /* mov.l r8,@(0x8,r4) */
 goto L8c645a5c;
L8c645a5c: if(io->status)return io->status; wr(m,io,r[4]+0xcu,r[11],4); /* mov.l r11,@(0xc,r4) */
 goto L8c645a5e;
L8c645a5e: if(io->status)return io->status; r[0]=sx(m,io,r[15]+0x4u,2); /* mov.w @(0x4,r15),r0 */
 goto L8c645a60;
L8c645a60: if(io->status)return io->status; r[4]=r[0]; /* mov r0,r4 */
 goto L8c645a62;
L8c645a62: if(io->status)return io->status; r[4]=r[4]&255u; /* extu.b r4,r4 */
 goto L8c645a64;
L8c645a64: if(io->status)return io->status; r[0]=r[13]; /* mov r13,r0 */
 goto L8c645a66;
L8c645a66: if(io->status)return io->status; r[4]>>=2; /* shlr2 r4 */
 goto L8c645a68;
L8c645a68: if(io->status)return io->status; {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c645a6a;
L8c645a6a: if(io->status)return io->status; r[4]>>=2; /* shlr2 r4 */
 goto L8c645a6c;
L8c645a6c: if(io->status)return io->status; *sr=(*sr&~1u)|(r[4]&1u);r[4]>>=1; /* shlr r4 */
 goto L8c645a6e;
L8c645a6e: if(io->status)return io->status; *sr=(*sr&~1u)|(r[0]==0xau); /* cmp/eq #0xa,r0 */
 goto L8c645a70;
L8c645a70: if(io->status)return io->status; if(!(*sr&1u))goto L8c645a82; /* bf 0x8c645a82 */
 goto L8c645a72;
L8c645a72: if(io->status)return io->status; r[3]=rd(m,io,0x8c645b58u,4); /* mov.l 0x8c645b58,r3 */
 goto L8c645a74;
L8c645a74: if(io->status)return io->status; r[4]+=12u; /* add #0xc,r4 */
 goto L8c645a76;
L8c645a76: if(io->status)return io->status; {uint32_t shift=r[4];if(!(shift&0x80000000u))r[12]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[12]=n?(uint32_t)((int32_t)r[12]>>n):((r[12]&0x80000000u)?0xffffffffu:0u);}} /* shad r4,r12 */
 goto L8c645a78;
L8c645a78: if(io->status)return io->status; r[2]=rd(m,io,r[3],4); /* mov.l @r3,r2 */
 goto L8c645a7a;
L8c645a7a: if(io->status)return io->status; r[2]|=r[12]; /* or r12,r2 */
 goto L8c645a7c;
L8c645a7c: if(io->status)return io->status; wr(m,io,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c645a7e;
L8c645a7e: if(io->status)return io->status; goto L8c645aca; /* bra 0x8c645aca */
 goto L8c645a82;
L8c645a82: if(io->status)return io->status; r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c645a84;
L8c645a84: if(io->status)return io->status; r[0]=r[13]; /* mov r13,r0 */
 goto L8c645a86;
L8c645a86: if(io->status)return io->status; {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c645a88;
L8c645a88: if(io->status)return io->status; *sr=(*sr&~1u)|(r[0]==0xbu); /* cmp/eq #0xb,r0 */
 goto L8c645a8a;
L8c645a8a: if(io->status)return io->status; if(!(*sr&1u))goto L8c645aca; /* bf 0x8c645aca */
 goto L8c645a8c;
L8c645a8c: if(io->status)return io->status; r[3]=rd(m,io,0x8c645b5cu,4); /* mov.l 0x8c645b5c,r3 */
 goto L8c645a8e;
L8c645a8e: if(io->status)return io->status; {uint32_t shift=r[4];if(!(shift&0x80000000u))r[12]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[12]=n?(uint32_t)((int32_t)r[12]>>n):((r[12]&0x80000000u)?0xffffffffu:0u);}} /* shad r4,r12 */
 goto L8c645a90;
L8c645a90: if(io->status)return io->status; r[2]=rd(m,io,r[3],4); /* mov.l @r3,r2 */
 goto L8c645a92;
L8c645a92: if(io->status)return io->status; r[2]|=r[12]; /* or r12,r2 */
 goto L8c645a94;
L8c645a94: if(io->status)return io->status; wr(m,io,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c645a96;
L8c645a96: if(io->status)return io->status; goto L8c645aca; /* bra 0x8c645aca */
 goto L8c645a9a;
L8c645a9a: if(io->status)return io->status; r[0]=r[13]; /* mov r13,r0 */
 goto L8c645a9c;
L8c645a9c: if(io->status)return io->status; r[3]=4294967292u; /* mov #-0x4,r3 */
 goto L8c645a9e;
L8c645a9e: if(io->status)return io->status; {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c645aa0;
L8c645aa0: if(io->status)return io->status; r[12]=r[14]; /* mov r14,r12 */
 goto L8c645aa2;
L8c645aa2: if(io->status)return io->status; r[0]<<=2; /* shll2 r0 */
 goto L8c645aa4;
L8c645aa4: if(io->status)return io->status; r[2]=rd(m,io,r[9]+r[0],4); /* mov.l @(r0,r9),r2 */
 goto L8c645aa6;
L8c645aa6: if(io->status)return io->status; r[12]<<=2; /* shll2 r12 */
 goto L8c645aa8;
L8c645aa8: if(io->status)return io->status; r[12]<<=2; /* shll2 r12 */
 goto L8c645aaa;
L8c645aaa: if(io->status)return io->status; *sr=(*sr&~1u)|(r[2]==r[4]); /* cmp/eq r2,r4 */
 goto L8c645aac;
L8c645aac: if(io->status)return io->status; take=!(*sr&1u);r[12]+=r[10];if(take)goto L8c645abe;goto L8c645ab0; /* bf/s 0x8c645abe */
 goto L8c645ab0;
L8c645ab0: if(io->status)return io->status; r[3]=4294967292u; /* mov #-0x4,r3 */
 goto L8c645ab2;
L8c645ab2: if(io->status)return io->status; r[0]=r[13]; /* mov r13,r0 */
 goto L8c645ab4;
L8c645ab4: if(io->status)return io->status; {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c645ab6;
L8c645ab6: if(io->status)return io->status; wr(m,io,r[12]+0x4u,r[4],4); /* mov.l r4,@(0x4,r12) */
 goto L8c645ab8;
L8c645ab8: if(io->status)return io->status; r[0]<<=2; /* shll2 r0 */
 goto L8c645aba;
L8c645aba: if(io->status)return io->status; wr(m,io,r[9]+r[0],r[12],4);goto L8c645ac2; /* bra 0x8c645ac2 */
 goto L8c645abe;
L8c645abe: if(io->status)return io->status; wr(m,io,r[5]+0x4u,r[12],4); /* mov.l r12,@(0x4,r5) */
 goto L8c645ac0;
L8c645ac0: if(io->status)return io->status; wr(m,io,r[12]+0x4u,r[4],4); /* mov.l r4,@(0x4,r12) */
 goto L8c645ac2;
L8c645ac2: if(io->status)return io->status; r[2]=rd(m,io,r[15],4); /* mov.l @r15,r2 */
 goto L8c645ac4;
L8c645ac4: if(io->status)return io->status; wr(m,io,r[12],r[2],4); /* mov.l r2,@r12 */
 goto L8c645ac6;
L8c645ac6: if(io->status)return io->status; wr(m,io,r[12]+0x8u,r[8],4); /* mov.l r8,@(0x8,r12) */
 goto L8c645ac8;
L8c645ac8: if(io->status)return io->status; wr(m,io,r[12]+0xcu,r[11],4); /* mov.l r11,@(0xc,r12) */
 goto L8c645aca;
L8c645aca: if(io->status)return io->status; r[4]=r[14];goto L8c645ad0; /* bra 0x8c645ad0 */
 goto L8c645ace;
L8c645ace: if(io->status)return io->status; r[4]=0u; /* mov #0x0,r4 */
 goto L8c645ad0;
L8c645ad0: if(io->status)return io->status; r[0]=sx(m,io,r[15]+0x8u,2); /* mov.w @(0x8,r15),r0 */
 goto L8c645ad2;
L8c645ad2: if(io->status)return io->status; r[3]=*sr; /* stc SR,r3 */
 goto L8c645ad4;
L8c645ad4: if(io->status)return io->status; r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c645ad6;
L8c645ad6: if(io->status)return io->status; r[2]=sx(m,io,0x8c645b48u,2); /* mov.w 0x8c645b48,r2 */
 goto L8c645ad8;
L8c645ad8: if(io->status)return io->status; r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c645ada;
L8c645ada: if(io->status)return io->status; r[0]<<=2; /* shll2 r0 */
 goto L8c645adc;
L8c645adc: if(io->status)return io->status; r[3]&=r[2]; /* and r2,r3 */
 goto L8c645ade;
L8c645ade: if(io->status)return io->status; r[0]<<=2; /* shll2 r0 */
 goto L8c645ae0;
L8c645ae0: if(io->status)return io->status; r[0]|=r[3]; /* or r3,r0 */
 goto L8c645ae2;
L8c645ae2: if(io->status)return io->status; *sr=r[0]; /* ldc r0,SR */
 goto L8c645ae4;
L8c645ae4: if(io->status)return io->status; r[0]=r[4]; /* mov r4,r0 */
 goto L8c645ae6;
L8c645ae6: if(io->status)return io->status; r[15]+=12u; /* add #0xc,r15 */
 goto L8c645ae8;
L8c645ae8: if(io->status)return io->status; *pr=rd(m,io,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c645aea;
L8c645aea: if(io->status)return io->status; r[8]=rd(m,io,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c645aec;
L8c645aec: if(io->status)return io->status; r[9]=rd(m,io,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c645aee;
L8c645aee: if(io->status)return io->status; r[10]=rd(m,io,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c645af0;
L8c645af0: if(io->status)return io->status; r[11]=rd(m,io,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c645af2;
L8c645af2: if(io->status)return io->status; r[12]=rd(m,io,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c645af4;
L8c645af4: if(io->status)return io->status; r[13]=rd(m,io,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c645af6;
L8c645af6: if(io->status)return io->status; r[14]=rd(m,io,r[15],4);r[15]+=4;return io->status; /* rts */
}
