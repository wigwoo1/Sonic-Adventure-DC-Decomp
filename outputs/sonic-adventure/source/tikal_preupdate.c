#include "fpu_cleanup_transfer.h"
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
/* Static C recovery of retail Tikal pre-update 0cb003a0..0cb00780.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_tikal_preupdate(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;
 goto Lcb003a0;
Lcb003a0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto Lcb003a2;
Lcb003a2: r[14]=r[4]; /* mov r4,r14 */
 goto Lcb003a4;
Lcb003a4: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto Lcb003a6;
Lcb003a6: r[13]=r[6]; /* mov r6,r13 */
 goto Lcb003a8;
Lcb003a8: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto Lcb003aa;
Lcb003aa: r[12]=r[5]; /* mov r5,r12 */
 goto Lcb003ac;
Lcb003ac: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto Lcb003ae;
Lcb003ae: r[11]=7u; /* mov #0x7,r11 */
 goto Lcb003b0;
Lcb003b0: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto Lcb003b2;
Lcb003b2: r[10]=2u; /* mov #0x2,r10 */
 goto Lcb003b4;
Lcb003b4: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto Lcb003b6;
Lcb003b6: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto Lcb003b8;
Lcb003b8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto Lcb003ba;
Lcb003ba: r[9]=rd(m,0x0cb003dcu,4); /* mov.l 0x0cb003dc,r9 */
 goto Lcb003bc;
Lcb003bc: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto Lcb003be;
Lcb003be: r[8]=rd(m,0x0cb003d8u,4); /* mov.l 0x0cb003d8,r8 */
 goto Lcb003c0;
Lcb003c0: r[0]+=4294967295u; /* add #-0x1,r0 */
 goto Lcb003c2;
Lcb003c2: r[1]=11u; /* mov #0xb,r1 */
 goto Lcb003c4;
Lcb003c4: *sr=(*sr&~1u)|(r[0]>=r[1]); /* cmp/hs r1,r0 */
 goto Lcb003c6;
Lcb003c6: if(!(*sr&1u))goto Lcb003cc; /* bf 0x0cb003cc */
 goto Lcb003c8;
Lcb003c8: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb003cc;
Lcb003cc: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto Lcb003ce;
Lcb003ce: r[1]=r[0]; /* mov r0,r1 */
 goto Lcb003d0;
Lcb003d0: r[0]=0x0cb003e0u; /* mova 0x0cb003e0,r0 */
 goto Lcb003d2;
Lcb003d2: r[0]=sx(m,r[1]+r[0],2); /* mov.w @(r0,r1),r0 */
 goto Lcb003d4;
Lcb003d4: switch(0xcb003d8u+r[0]){case 0xcb003f6u:goto Lcb003f6;case 0xcb0043eu:goto Lcb0043e;case 0xcb004b2u:goto Lcb004b2;case 0xcb005a0u:goto Lcb005a0;case 0xcb005ccu:goto Lcb005cc;case 0xcb005f8u:goto Lcb005f8;case 0xcb00680u:goto Lcb00680;case 0xcb00720u:goto Lcb00720;case 0xcb00730u:goto Lcb00730;case 0xcb0076eu:goto Lcb0076e;case 0xcb00750u:goto Lcb00750;default:return -2;} /* braf r0 */
Lcb003f6: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb003f8;
Lcb003f8: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb003fa;
Lcb003fa: *pr=0xcb003feu;r[4]=r[14];status=call(ctx,0xcb00cd2u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00cd2 */
 goto Lcb003fe;
Lcb003fe: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00400;
Lcb00400: if((*sr&1u))goto Lcb00406; /* bt 0x0cb00406 */
 goto Lcb00402;
Lcb00402: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb00406;
Lcb00406: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00408;
Lcb00408: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb0040a;
Lcb0040a: *pr=0xcb0040eu;r[4]=r[14];status=call(ctx,0xcb00e74u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00e74 */
 goto Lcb0040e;
Lcb0040e: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00410;
Lcb00410: if((*sr&1u))goto Lcb00416; /* bt 0x0cb00416 */
 goto Lcb00412;
Lcb00412: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb00416;
Lcb00416: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00418;
Lcb00418: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb0041a;
Lcb0041a: *pr=0xcb0041eu;r[4]=r[14];status=call(ctx,0xcb00b64u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00b64 */
 goto Lcb0041e;
Lcb0041e: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00420;
Lcb00420: if((*sr&1u))goto Lcb00426; /* bt 0x0cb00426 */
 goto Lcb00422;
Lcb00422: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb00426;
Lcb00426: r[2]=rd(m,0x0cb00580u,4); /* mov.l 0x0cb00580,r2 */
 goto Lcb00428;
Lcb00428: r[6]=0u; /* mov #0x0,r6 */
 goto Lcb0042a;
Lcb0042a: r[5]=r[6]; /* mov r6,r5 */
 goto Lcb0042c;
Lcb0042c: target=r[2];*pr=0xcb00430u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb00430;
Lcb00430: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00432;
Lcb00432: if(!(*sr&1u))goto Lcb00438; /* bf 0x0cb00438 */
 goto Lcb00434;
Lcb00434: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb00438;
Lcb00438: wr(m,r[14],r[10],1); /* mov.b r10,@r14 */
 goto Lcb0043a;
Lcb0043a: r[2]=1u;goto Lcb00656; /* bra 0x0cb00656 */
 goto Lcb0043e;
Lcb0043e: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00440;
Lcb00440: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00442;
Lcb00442: *pr=0xcb00446u;r[4]=r[14];status=call(ctx,0xcb00cd2u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00cd2 */
 goto Lcb00446;
Lcb00446: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00448;
Lcb00448: if((*sr&1u))goto Lcb0044e; /* bt 0x0cb0044e */
 goto Lcb0044a;
Lcb0044a: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb0044e;
Lcb0044e: r[10]=4294967264u; /* mov #-0x20,r10 */
 goto Lcb00450;
Lcb00450: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00452;
Lcb00452: r[10]<<=8; /* shll8 r10 */
 goto Lcb00454;
Lcb00454: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00456;
Lcb00456: r[10]+=4294967295u; /* add #-0x1,r10 */
 goto Lcb00458;
Lcb00458: *pr=0xcb0045cu;r[4]=r[14];status=call(ctx,0xcb00e74u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00e74 */
 goto Lcb0045c;
Lcb0045c: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb0045e;
Lcb0045e: if(!(*sr&1u))goto Lcb004aa; /* bf 0x0cb004aa */
 goto Lcb00460;
Lcb00460: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00462;
Lcb00462: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00464;
Lcb00464: *pr=0xcb00468u;r[4]=r[14];status=call(ctx,0xcb00b64u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00b64 */
 goto Lcb00468;
Lcb00468: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb0046a;
Lcb0046a: if(!(*sr&1u))goto Lcb004aa; /* bf 0x0cb004aa */
 goto Lcb0046c;
Lcb0046c: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb0046e;
Lcb0046e: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00470;
Lcb00470: *pr=0xcb00474u;r[4]=r[14];status=call(ctx,0xcb00e90u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00e90 */
 goto Lcb00474;
Lcb00474: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00476;
Lcb00476: if(!(*sr&1u))goto Lcb004aa; /* bf 0x0cb004aa */
 goto Lcb00478;
Lcb00478: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb0047a;
Lcb0047a: target=r[9];*pr=0xcb0047eu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto Lcb0047e;
Lcb0047e: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00480;
Lcb00480: if(!(*sr&1u))goto Lcb00486; /* bf 0x0cb00486 */
 goto Lcb00482;
Lcb00482: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb00486;
Lcb00486: r[0]=4294967232u; /* mov #-0x40,r0 */
 goto Lcb00488;
Lcb00488: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto Lcb0048a;
Lcb0048a: fr[3]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr3 */
 goto Lcb0048c;
Lcb0048c: r[0]=56u; /* mov #0x38,r0 */
 goto Lcb0048e;
Lcb0048e: fr[2]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr2 */
 goto Lcb00490;
Lcb00490: status=gt(f,sr,fr[3],fr[2]);if(status)return status; /* fcmp/gt fr2,fr3 */
 goto Lcb00492;
Lcb00492: if(!(*sr&1u))goto Lcb00498; /* bf 0x0cb00498 */
 goto Lcb00494;
Lcb00494: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb00498;
Lcb00498: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb0049a;
Lcb0049a: wr(m,r[14],r[11],1); /* mov.b r11,@r14 */
 goto Lcb0049c;
Lcb0049c: r[7]=0u; /* mov #0x0,r7 */
 goto Lcb0049e;
Lcb0049e: r[0]<<=2; /* shll2 r0 */
 goto Lcb004a0;
Lcb004a0: r[5]=r[7]; /* mov r7,r5 */
 goto Lcb004a2;
Lcb004a2: r[6]=r[7]; /* mov r7,r6 */
 goto Lcb004a4;
Lcb004a4: wr(m,r[13]+r[0],r[11],2); /* mov.w r11,@(r0,r13) */
 goto Lcb004a6;
Lcb004a6: target=r[8];*pr=0xcb004aau;r[4]=18u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r8 */
 goto Lcb004aa;
Lcb004aa: r[0]=sx(m,r[14]+0x4u,2); /* mov.w @(0x4,r14),r0 */
 goto Lcb004ac;
Lcb004ac: r[0]&=r[10]; /* and r10,r0 */
 goto Lcb004ae;
Lcb004ae: wr(m,r[14]+0x4u,r[0],2);goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb004b2;
Lcb004b2: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb004b4;
Lcb004b4: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb004b6;
Lcb004b6: *pr=0xcb004bau;r[4]=r[14];status=call(ctx,0xcb00cd2u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00cd2 */
 goto Lcb004ba;
Lcb004ba: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb004bc;
Lcb004bc: if((*sr&1u))goto Lcb004c2; /* bt 0x0cb004c2 */
 goto Lcb004be;
Lcb004be: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb004c2;
Lcb004c2: r[0]=sx(m,r[14]+0x4u,2); /* mov.w @(0x4,r14),r0 */
 goto Lcb004c4;
Lcb004c4: *sr=(*sr&~1u)|!(r[0]&0x3u); /* tst #0x3,r0 */
 goto Lcb004c6;
Lcb004c6: if((*sr&1u))goto Lcb0052a; /* bt 0x0cb0052a */
 goto Lcb004c8;
Lcb004c8: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto Lcb004ca;
Lcb004ca: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb004cc;
Lcb004cc: if(!(*sr&1u))goto Lcb004d8; /* bf 0x0cb004d8 */
 goto Lcb004ce;
Lcb004ce: r[7]=0u; /* mov #0x0,r7 */
 goto Lcb004d0;
Lcb004d0: r[5]=r[7]; /* mov r7,r5 */
 goto Lcb004d2;
Lcb004d2: r[6]=r[7]; /* mov r7,r6 */
 goto Lcb004d4;
Lcb004d4: target=r[8];*pr=0xcb004d8u;r[4]=33u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r8 */
 goto Lcb004d8;
Lcb004d8: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb004da;
Lcb004da: target=r[9];*pr=0xcb004deu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto Lcb004de;
Lcb004de: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb004e0;
Lcb004e0: if((*sr&1u))goto Lcb004f0; /* bt 0x0cb004f0 */
 goto Lcb004e2;
Lcb004e2: r[0]=56u; /* mov #0x38,r0 */
 goto Lcb004e4;
Lcb004e4: fr[3]=0; /* fldi0 fr3 */
 goto Lcb004e6;
Lcb004e6: fr[2]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr2 */
 goto Lcb004e8;
Lcb004e8: status=gt(f,sr,fr[2],fr[3]);if(status)return status; /* fcmp/gt fr3,fr2 */
 goto Lcb004ea;
Lcb004ea: if(!(*sr&1u))goto Lcb004f0; /* bf 0x0cb004f0 */
 goto Lcb004ec;
Lcb004ec: goto Lcb00636; /* bra 0x0cb00636 */
 goto Lcb004f0;
Lcb004f0: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb004f2;
Lcb004f2: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb004f4;
Lcb004f4: *pr=0xcb004f8u;r[4]=r[14];status=call(ctx,0xcb00e90u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00e90 */
 goto Lcb004f8;
Lcb004f8: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb004fa;
Lcb004fa: if((*sr&1u))goto Lcb00508; /* bt 0x0cb00508 */
 goto Lcb004fc;
Lcb004fc: r[2]=rd(m,r[12]+0x1cu,4); /* mov.l @(0x1c,r12),r2 */
 goto Lcb004fe;
Lcb004fe: wr(m,r[14]+0x14u,r[2],4); /* mov.l r2,@(0x14,r14) */
 goto Lcb00500;
Lcb00500: r[3]=rd(m,r[12]+0x24u,4); /* mov.l @(0x24,r12),r3 */
 goto Lcb00502;
Lcb00502: wr(m,r[14]+0x1cu,r[3],4); /* mov.l r3,@(0x1c,r14) */
 goto Lcb00504;
Lcb00504: r[2]=6u;goto Lcb00656; /* bra 0x0cb00656 */
 goto Lcb00508;
Lcb00508: r[1]=rd(m,r[12]+0x1cu,4); /* mov.l @(0x1c,r12),r1 */
 goto Lcb0050a;
Lcb0050a: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb0050c;
Lcb0050c: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb0050e;
Lcb0050e: r[4]=r[14]; /* mov r14,r4 */
 goto Lcb00510;
Lcb00510: wr(m,r[14]+0x14u,r[1],4); /* mov.l r1,@(0x14,r14) */
 goto Lcb00512;
Lcb00512: r[3]=rd(m,r[12]+0x24u,4); /* mov.l @(0x24,r12),r3 */
 goto Lcb00514;
Lcb00514: wr(m,r[14]+0x1cu,r[3],4); /* mov.l r3,@(0x1c,r14) */
 goto Lcb00516;
Lcb00516: wr(m,r[14],r[10],1); /* mov.b r10,@r14 */
 goto Lcb00518;
Lcb00518: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto Lcb0051a;
Lcb0051a: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto Lcb0051c;
Lcb0051c: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto Lcb0051e;
Lcb0051e: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto Lcb00520;
Lcb00520: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto Lcb00522;
Lcb00522: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto Lcb00524;
Lcb00524: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto Lcb00526;
Lcb00526: r[14]=rd(m,r[15],4);r[15]+=4;return sa_tikal_transition(m,f,sr,r); /* bra 0x0cb00994 */
 goto Lcb0052a;
Lcb0052a: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb0052c;
Lcb0052c: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb0052e;
Lcb0052e: *pr=0xcb00532u;r[4]=r[14];status=call(ctx,0xcb00c20u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00c20 */
 goto Lcb00532;
Lcb00532: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00534;
Lcb00534: if((*sr&1u))goto Lcb0053a; /* bt 0x0cb0053a */
 goto Lcb00536;
Lcb00536: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb0053a;
Lcb0053a: r[3]=rd(m,0x0cb00584u,4); /* mov.l 0x0cb00584,r3 */
 goto Lcb0053c;
Lcb0053c: r[5]=r[13]; /* mov r13,r5 */
 goto Lcb0053e;
Lcb0053e: target=r[3];*pr=0xcb00542u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00542;
Lcb00542: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00544;
Lcb00544: if(!(*sr&1u))goto Lcb0054a; /* bf 0x0cb0054a */
 goto Lcb00546;
Lcb00546: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb0054a;
Lcb0054a: r[1]=rd(m,0x0cb00588u,4); /* mov.l 0x0cb00588,r1 */
 goto Lcb0054c;
Lcb0054c: r[0]=60u; /* mov #0x3c,r0 */
 goto Lcb0054e;
Lcb0054e: fr[2]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr2 */
 goto Lcb00550;
Lcb00550: *fpul=r[1]; /* lds r1,FPUL */
 goto Lcb00552;
Lcb00552: r[1]=4294967252u; /* mov #-0x2c,r1 */
 goto Lcb00554;
Lcb00554: r[1]=r[1]&255u; /* extu.b r1,r1 */
 goto Lcb00556;
Lcb00556: r[1]+=r[13]; /* add r13,r1 */
 goto Lcb00558;
Lcb00558: fr[3]=rd(m,r[1],4); /* fmov.s @r1,fr3 */
 goto Lcb0055a;
Lcb0055a: fr[0]=*fpul; /* fsts FPUL,fr0 */
 goto Lcb0055c;
Lcb0055c: status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status; /* fmac fr0,fr3,fr2 */
 goto Lcb0055e;
Lcb0055e: wr(m,r[0]+r[13],fr[2],4);goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb005a0;
Lcb005a0: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb005a2;
Lcb005a2: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb005a4;
Lcb005a4: *pr=0xcb005a8u;r[4]=r[14];status=call(ctx,0xcb00cd2u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00cd2 */
 goto Lcb005a8;
Lcb005a8: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb005aa;
Lcb005aa: if((*sr&1u))goto Lcb005b0; /* bt 0x0cb005b0 */
 goto Lcb005ac;
Lcb005ac: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb005b0;
Lcb005b0: r[0]=8u; /* mov #0x8,r0 */
 goto Lcb005b2;
Lcb005b2: fr[3]=0; /* fldi0 fr3 */
 goto Lcb005b4;
Lcb005b4: fr[2]=rd(m,r[0]+r[12],4); /* fmov.s @(r0,r12),fr2 */
 goto Lcb005b6;
Lcb005b6: status=gt(f,sr,fr[2],fr[3]);if(status)return status; /* fcmp/gt fr3,fr2 */
 goto Lcb005b8;
Lcb005b8: if(!(*sr&1u))goto Lcb005be; /* bf 0x0cb005be */
 goto Lcb005ba;
Lcb005ba: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb005be;
Lcb005be: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb005c0;
Lcb005c0: r[2]=6u; /* mov #0x6,r2 */
 goto Lcb005c2;
Lcb005c2: r[0]<<=2; /* shll2 r0 */
 goto Lcb005c4;
Lcb005c4: wr(m,r[14],r[2],1); /* mov.b r2,@r14 */
 goto Lcb005c6;
Lcb005c6: r[3]=4u; /* mov #0x4,r3 */
 goto Lcb005c8;
Lcb005c8: wr(m,r[13]+r[0],r[3],2);goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb005cc;
Lcb005cc: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb005ce;
Lcb005ce: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb005d0;
Lcb005d0: *pr=0xcb005d4u;r[4]=r[14];status=call(ctx,0xcb00cd2u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00cd2 */
 goto Lcb005d4;
Lcb005d4: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb005d6;
Lcb005d6: if((*sr&1u))goto Lcb005dc; /* bt 0x0cb005dc */
 goto Lcb005d8;
Lcb005d8: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb005dc;
Lcb005dc: r[0]=8u; /* mov #0x8,r0 */
 goto Lcb005de;
Lcb005de: fr[3]=0; /* fldi0 fr3 */
 goto Lcb005e0;
Lcb005e0: fr[2]=rd(m,r[0]+r[12],4); /* fmov.s @(r0,r12),fr2 */
 goto Lcb005e2;
Lcb005e2: status=gt(f,sr,fr[2],fr[3]);if(status)return status; /* fcmp/gt fr3,fr2 */
 goto Lcb005e4;
Lcb005e4: if(!(*sr&1u))goto Lcb005ea; /* bf 0x0cb005ea */
 goto Lcb005e6;
Lcb005e6: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb005ea;
Lcb005ea: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb005ec;
Lcb005ec: r[2]=6u; /* mov #0x6,r2 */
 goto Lcb005ee;
Lcb005ee: r[0]<<=2; /* shll2 r0 */
 goto Lcb005f0;
Lcb005f0: wr(m,r[14],r[2],1); /* mov.b r2,@r14 */
 goto Lcb005f2;
Lcb005f2: r[3]=4u; /* mov #0x4,r3 */
 goto Lcb005f4;
Lcb005f4: wr(m,r[13]+r[0],r[3],2);goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb005f8;
Lcb005f8: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb005fa;
Lcb005fa: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb005fc;
Lcb005fc: *pr=0xcb00600u;r[4]=r[14];status=call(ctx,0xcb00cd2u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00cd2 */
 goto Lcb00600;
Lcb00600: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00602;
Lcb00602: if((*sr&1u))goto Lcb00608; /* bt 0x0cb00608 */
 goto Lcb00604;
Lcb00604: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb00608;
Lcb00608: r[0]=sx(m,r[14]+0x4u,2); /* mov.w @(0x4,r14),r0 */
 goto Lcb0060a;
Lcb0060a: *sr=(*sr&~1u)|!(r[0]&0x3u); /* tst #0x3,r0 */
 goto Lcb0060c;
Lcb0060c: if(!(*sr&1u))goto Lcb00612; /* bf 0x0cb00612 */
 goto Lcb0060e;
Lcb0060e: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb00612;
Lcb00612: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto Lcb00614;
Lcb00614: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00616;
Lcb00616: if(!(*sr&1u))goto Lcb00622; /* bf 0x0cb00622 */
 goto Lcb00618;
Lcb00618: r[7]=0u; /* mov #0x0,r7 */
 goto Lcb0061a;
Lcb0061a: r[5]=r[7]; /* mov r7,r5 */
 goto Lcb0061c;
Lcb0061c: r[6]=r[7]; /* mov r7,r6 */
 goto Lcb0061e;
Lcb0061e: target=r[8];*pr=0xcb00622u;r[4]=33u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r8 */
 goto Lcb00622;
Lcb00622: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00624;
Lcb00624: target=r[9];*pr=0xcb00628u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto Lcb00628;
Lcb00628: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb0062a;
Lcb0062a: if((*sr&1u))goto Lcb00640; /* bt 0x0cb00640 */
 goto Lcb0062c;
Lcb0062c: r[0]=56u; /* mov #0x38,r0 */
 goto Lcb0062e;
Lcb0062e: fr[3]=0; /* fldi0 fr3 */
 goto Lcb00630;
Lcb00630: fr[2]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr2 */
 goto Lcb00632;
Lcb00632: status=gt(f,sr,fr[2],fr[3]);if(status)return status; /* fcmp/gt fr3,fr2 */
 goto Lcb00634;
Lcb00634: if(!(*sr&1u))goto Lcb00640; /* bf 0x0cb00640 */
 goto Lcb00636;
Lcb00636: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb00638;
Lcb00638: wr(m,r[14],r[11],1); /* mov.b r11,@r14 */
 goto Lcb0063a;
Lcb0063a: r[0]<<=2; /* shll2 r0 */
 goto Lcb0063c;
Lcb0063c: wr(m,r[13]+r[0],r[11],2);goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb00640;
Lcb00640: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00642;
Lcb00642: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00644;
Lcb00644: *pr=0xcb00648u;r[4]=r[14];status=call(ctx,0xcb00e90u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00e90 */
 goto Lcb00648;
Lcb00648: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb0064a;
Lcb0064a: if((*sr&1u))goto Lcb0065e; /* bt 0x0cb0065e */
 goto Lcb0064c;
Lcb0064c: r[2]=rd(m,r[12]+0x1cu,4); /* mov.l @(0x1c,r12),r2 */
 goto Lcb0064e;
Lcb0064e: wr(m,r[14]+0x14u,r[2],4); /* mov.l r2,@(0x14,r14) */
 goto Lcb00650;
Lcb00650: r[2]=6u; /* mov #0x6,r2 */
 goto Lcb00652;
Lcb00652: r[3]=rd(m,r[12]+0x24u,4); /* mov.l @(0x24,r12),r3 */
 goto Lcb00654;
Lcb00654: wr(m,r[14]+0x1cu,r[3],4); /* mov.l r3,@(0x1c,r14) */
 goto Lcb00656;
Lcb00656: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb00658;
Lcb00658: r[0]<<=2; /* shll2 r0 */
 goto Lcb0065a;
Lcb0065a: wr(m,r[13]+r[0],r[2],2);goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb0065e;
Lcb0065e: r[1]=rd(m,r[12]+0x1cu,4); /* mov.l @(0x1c,r12),r1 */
 goto Lcb00660;
Lcb00660: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00662;
Lcb00662: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00664;
Lcb00664: r[4]=r[14]; /* mov r14,r4 */
 goto Lcb00666;
Lcb00666: wr(m,r[14]+0x14u,r[1],4); /* mov.l r1,@(0x14,r14) */
 goto Lcb00668;
Lcb00668: r[3]=rd(m,r[12]+0x24u,4); /* mov.l @(0x24,r12),r3 */
 goto Lcb0066a;
Lcb0066a: wr(m,r[14]+0x1cu,r[3],4); /* mov.l r3,@(0x1c,r14) */
 goto Lcb0066c;
Lcb0066c: wr(m,r[14],r[10],1); /* mov.b r10,@r14 */
 goto Lcb0066e;
Lcb0066e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto Lcb00670;
Lcb00670: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto Lcb00672;
Lcb00672: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto Lcb00674;
Lcb00674: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto Lcb00676;
Lcb00676: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto Lcb00678;
Lcb00678: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto Lcb0067a;
Lcb0067a: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto Lcb0067c;
Lcb0067c: r[14]=rd(m,r[15],4);r[15]+=4;return sa_tikal_transition(m,f,sr,r); /* bra 0x0cb00994 */
 goto Lcb00680;
Lcb00680: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00682;
Lcb00682: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00684;
Lcb00684: *pr=0xcb00688u;r[4]=r[14];status=call(ctx,0xcb00cd2u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00cd2 */
 goto Lcb00688;
Lcb00688: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb0068a;
Lcb0068a: if((*sr&1u))goto Lcb00690; /* bt 0x0cb00690 */
 goto Lcb0068c;
Lcb0068c: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb00690;
Lcb00690: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00692;
Lcb00692: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00694;
Lcb00694: *pr=0xcb00698u;r[4]=r[14];status=call(ctx,0xcb00e74u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00e74 */
 goto Lcb00698;
Lcb00698: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb0069a;
Lcb0069a: if((*sr&1u))goto Lcb006a0; /* bt 0x0cb006a0 */
 goto Lcb0069c;
Lcb0069c: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb006a0;
Lcb006a0: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb006a2;
Lcb006a2: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb006a4;
Lcb006a4: *pr=0xcb006a8u;r[4]=r[14];status=call(ctx,0xcb00b64u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00b64 */
 goto Lcb006a8;
Lcb006a8: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb006aa;
Lcb006aa: if((*sr&1u))goto Lcb006b0; /* bt 0x0cb006b0 */
 goto Lcb006ac;
Lcb006ac: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb006b0;
Lcb006b0: r[0]=56u; /* mov #0x38,r0 */
 goto Lcb006b2;
Lcb006b2: fr[3]=0; /* fldi0 fr3 */
 goto Lcb006b4;
Lcb006b4: fr[2]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr2 */
 goto Lcb006b6;
Lcb006b6: status=gt(f,sr,fr[2],fr[3]);if(status)return status; /* fcmp/gt fr3,fr2 */
 goto Lcb006b8;
Lcb006b8: if((*sr&1u))goto Lcb006e0; /* bt 0x0cb006e0 */
 goto Lcb006ba;
Lcb006ba: r[2]=rd(m,0x0cb00700u,4); /* mov.l 0x0cb00700,r2 */
 goto Lcb006bc;
Lcb006bc: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb006be;
Lcb006be: target=r[2];*pr=0xcb006c2u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb006c2;
Lcb006c2: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb006c4;
Lcb006c4: if((*sr&1u))goto Lcb006e0; /* bt 0x0cb006e0 */
 goto Lcb006c6;
Lcb006c6: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb006c8;
Lcb006c8: r[3]=rd(m,0x0cb00704u,4); /* mov.l 0x0cb00704,r3 */
 goto Lcb006ca;
Lcb006ca: r[0]<<=2; /* shll2 r0 */
 goto Lcb006cc;
Lcb006cc: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb006ce;
Lcb006ce: r[2]=8u; /* mov #0x8,r2 */
 goto Lcb006d0;
Lcb006d0: wr(m,r[14],r[10],1); /* mov.b r10,@r14 */
 goto Lcb006d2;
Lcb006d2: wr(m,r[13]+r[0],r[2],2); /* mov.w r2,@(r0,r13) */
 goto Lcb006d4;
Lcb006d4: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb006d6;
Lcb006d6: target=r[3];*pr=0xcb006dau;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb006da;
Lcb006da: r[2]=rd(m,r[12]+0x20u,4); /* mov.l @(0x20,r12),r2 */
 goto Lcb006dc;
Lcb006dc: wr(m,r[14]+0x18u,r[2],4);goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb006e0;
Lcb006e0: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb006e2;
Lcb006e2: r[0]<<=2; /* shll2 r0 */
 goto Lcb006e4;
Lcb006e4: r[0]=sx(m,r[13]+r[0],2); /* mov.w @(r0,r13),r0 */
 goto Lcb006e6;
Lcb006e6: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto Lcb006e8;
Lcb006e8: *sr=(*sr&~1u)|(r[0]==0x7u); /* cmp/eq #0x7,r0 */
 goto Lcb006ea;
Lcb006ea: if((*sr&1u))goto Lcb0076e; /* bt 0x0cb0076e */
 goto Lcb006ec;
Lcb006ec: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb006ee;
Lcb006ee: r[2]=1u; /* mov #0x1,r2 */
 goto Lcb006f0;
Lcb006f0: r[0]<<=2; /* shll2 r0 */
 goto Lcb006f2;
Lcb006f2: wr(m,r[14],r[2],1); /* mov.b r2,@r14 */
 goto Lcb006f4;
Lcb006f4: r[3]=0u; /* mov #0x0,r3 */
 goto Lcb006f6;
Lcb006f6: wr(m,r[13]+r[0],r[3],2);goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb00720;
Lcb00720: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00722;
Lcb00722: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00724;
Lcb00724: *pr=0xcb00728u;r[4]=r[14];status=call(ctx,0xcb00cd2u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00cd2 */
 goto Lcb00728;
Lcb00728: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb0072a;
Lcb0072a: if(!(*sr&1u))goto Lcb0076e; /* bf 0x0cb0076e */
 goto Lcb0072c;
Lcb0072c: goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb00730;
Lcb00730: r[3]=rd(m,r[14]+0x3cu,4); /* mov.l @(0x3c,r14),r3 */
 goto Lcb00732;
Lcb00732: r[0]=76u; /* mov #0x4c,r0 */
 goto Lcb00734;
Lcb00734: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto Lcb00736;
Lcb00736: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto Lcb00738;
Lcb00738: if(!(*sr&1u))goto Lcb0076e; /* bf 0x0cb0076e */
 goto Lcb0073a;
Lcb0073a: r[2]=rd(m,r[12]+0x1cu,4); /* mov.l @(0x1c,r12),r2 */
 goto Lcb0073c;
Lcb0073c: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb0073e;
Lcb0073e: r[0]<<=2; /* shll2 r0 */
 goto Lcb00740;
Lcb00740: wr(m,r[14]+0x14u,r[2],4); /* mov.l r2,@(0x14,r14) */
 goto Lcb00742;
Lcb00742: r[2]=1u; /* mov #0x1,r2 */
 goto Lcb00744;
Lcb00744: r[3]=rd(m,r[12]+0x24u,4); /* mov.l @(0x24,r12),r3 */
 goto Lcb00746;
Lcb00746: wr(m,r[14]+0x1cu,r[3],4); /* mov.l r3,@(0x1c,r14) */
 goto Lcb00748;
Lcb00748: r[3]=0u; /* mov #0x0,r3 */
 goto Lcb0074a;
Lcb0074a: wr(m,r[14],r[2],1); /* mov.b r2,@r14 */
 goto Lcb0074c;
Lcb0074c: wr(m,r[13]+r[0],r[3],2);goto Lcb0076e; /* bra 0x0cb0076e */
 goto Lcb00750;
Lcb00750: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto Lcb00752;
Lcb00752: r[3]=52u; /* mov #0x34,r3 */
 goto Lcb00754;
Lcb00754: r[1]=rd(m,0x0cb00860u,4); /* mov.l 0x0cb00860,r1 */
 goto Lcb00756;
Lcb00756: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto Lcb00758;
Lcb00758: *macl=(r[3]&65535u)*(r[0]&65535u); /* mulu.w r3,r0 */
 goto Lcb0075a;
Lcb0075a: r[0]=*macl; /* sts MACL,r0 */
 goto Lcb0075c;
Lcb0075c: r[0]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto Lcb0075e;
Lcb0075e: *sr=(*sr&~1u)|!(r[0]&0x4u); /* tst #0x4,r0 */
 goto Lcb00760;
Lcb00760: if((*sr&1u))goto Lcb0076e; /* bt 0x0cb0076e */
 goto Lcb00762;
Lcb00762: r[2]=73u; /* mov #0x49,r2 */
 goto Lcb00764;
Lcb00764: wr(m,r[14],r[10],1); /* mov.b r10,@r14 */
 goto Lcb00766;
Lcb00766: r[2]<<=2; /* shll2 r2 */
 goto Lcb00768;
Lcb00768: r[2]+=r[13]; /* add r13,r2 */
 goto Lcb0076a;
Lcb0076a: r[0]=1u; /* mov #0x1,r0 */
 goto Lcb0076c;
Lcb0076c: wr(m,r[2],r[0],2); /* mov.w r0,@r2 */
 goto Lcb0076e;
Lcb0076e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto Lcb00770;
Lcb00770: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto Lcb00772;
Lcb00772: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto Lcb00774;
Lcb00774: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto Lcb00776;
Lcb00776: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto Lcb00778;
Lcb00778: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto Lcb0077a;
Lcb0077a: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto Lcb0077c;
Lcb0077c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
