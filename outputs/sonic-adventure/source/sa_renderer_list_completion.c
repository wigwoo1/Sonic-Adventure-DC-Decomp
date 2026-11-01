/* Original TA list-completion callback651f1e. Exact instruction order. */
#include "fpu_cleanup_transfer.h"
#include "vertex_bus.h"
#include "sh4_integer_state.h"
#include "sh4_vector_base.h"
#include "sh4_add_sub.h"
#include "sh4_vector_numeric.h"
#include "sh4_fmac.h"
#include "sh4_divide.h"
#include "sh4_multiply_extended.h"
#include "sh4_float_i32.h"
#include "sh4_compare_bits.h"
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
typedef int (*sa_shared_service)(void *,uint32_t,uint8_t *,
    struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,uint32_t a,unsigned n){
 if((a&0x1f000000u)!=0x0c000000u){uint32_t value=0;if(!bus||!bus->read){*io_status=-2;return 0;}*io_status=bus->read(bus->context,a,n,&value);return value;}
 uint32_t value=0;for(unsigned i=0;i<n;i++)value|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return value;
}
static uint32_t sx(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,uint32_t a,unsigned n){
 uint32_t value=rd(m,bus,io_status,a,n);return n==1?(uint32_t)(int32_t)(int8_t)value:(uint32_t)(int32_t)(int16_t)value;
}
static void wr(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,uint32_t a,uint32_t value,unsigned n){
 if((a&0x1f000000u)!=0x0c000000u){if(!bus||!bus->write){*io_status=-2;return;}*io_status=bus->write(bus->context,a,value,n);return;}
 for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(value>>(8*i));
}
static void fload(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  f->bank[bank][reg]=rd(m,bus,io_status,a,4);f->bank[bank][reg+1]=rd(m,bus,io_status,a+4u,4);
 }else f->bank[bank][reg]=rd(m,bus,io_status,a,4);
}
static void fstore(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  wr(m,bus,io_status,a,f->bank[bank][reg],4);wr(m,bus,io_status,a+4u,f->bank[bank][reg+1],4);
 }else wr(m,bus,io_status,a,f->bank[bank][reg],4);
}
static void fcopy(struct sa_fpu_banks *f,unsigned n,unsigned s){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){unsigned nb=bank^(n&1u),sb=bank^(s&1u);n&=~1u;s&=~1u;
  uint32_t a=f->bank[sb][s],b=f->bank[sb][s+1];f->bank[nb][n]=a;f->bank[nb][n+1]=b;
 }else f->bank[bank][n]=f->bank[bank][s];
}
static int valid_ram(uint32_t a,unsigned n){
 return (a&0x1f000000u)==0x0c000000u && (a&0xffffffu)<=0x1000000u-n;
}
static int valid_access(uint32_t a,unsigned n){
 if(valid_ram(a,n))return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_renderer_list_completion_8c651f1e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c651f1e;
L8c651f1e: *stop=0x8c651f1eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f98u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651f20; /* mov.l 0x8c651f98,r3 */
L8c651f20: *stop=0x8c651f20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f60u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651f22; /* mov.l 0x8c651f60,r1 */
L8c651f22: *stop=0x8c651f22u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651f24; /* mov.l @r3,r0 */
L8c651f24: *stop=0x8c651f24u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f94u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c651f26; /* mov.l 0x8c651f94,r7 */
L8c651f26: *stop=0x8c651f26u;r[0]<<=2;goto L8c651f28; /* shll2 r0 */
L8c651f28: *stop=0x8c651f28u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651f2a; /* mov.l @(r0,r1),r0 */
L8c651f2a: *stop=0x8c651f2au;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c651f2c; /* cmp/eq #-0x1,r0 */
L8c651f2c: if(!(*sr&1u))goto L8c651f32;goto L8c651f2e; /* bf 0x8c651f32 */
L8c651f2e: *stop=0x8c651f30u;goto L8c65201e; /* bra 0x8c65201e */
L8c651f32: *stop=0x8c651f32u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f9cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651f34; /* mov.l 0x8c651f9c,r2 */
L8c651f34: *stop=0x8c651f34u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651f36; /* mov.l @r2,r0 */
L8c651f36: *stop=0x8c651f36u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c651f38; /* tst r0,r0 */
L8c651f38: if(!(*sr&1u))goto L8c651f4e;goto L8c651f3a; /* bf 0x8c651f4e */
L8c651f3a: *stop=0x8c651f3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651fa0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651f3c; /* mov.l 0x8c651fa0,r3 */
L8c651f3c: *stop=0x8c651f3cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651f3e; /* mov.l @r3,r0 */
L8c651f3e: *stop=0x8c651f3eu;r[0]|=0x20u;goto L8c651f40; /* or #0x20,r0 */
L8c651f40: *stop=0x8c651f40u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c651f42; /* mov.l r0,@r3 */
L8c651f42: *stop=0x8c651f42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f98u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651f44; /* mov.l 0x8c651f98,r0 */
L8c651f44: *stop=0x8c651f44u;r[1]=3u;goto L8c651f46; /* mov #0x3,r1 */
L8c651f46: *stop=0x8c651f46u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651f48; /* mov.l @r0,r3 */
L8c651f48: *stop=0x8c651f48u;r[0]+=4294967272u;goto L8c651f4a; /* add #-0x18,r0 */
L8c651f4a: *stop=0x8c651f4au;r[3]<<=2;goto L8c651f4c; /* shll2 r3 */
L8c651f4c: *stop=0x8c651f4cu;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[1],4);if(io_status)return io_status;goto L8c651f4e; /* mov.l r1,@(r0,r3) */
L8c651f4e: *stop=0x8c651f4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651fa8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651f50; /* mov.l 0x8c651fa8,r2 */
L8c651f50: *stop=0x8c651f50u;r[5]=1u;goto L8c651f52; /* mov #0x1,r5 */
L8c651f52: *stop=0x8c651f52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651fa4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c651f54; /* mov.l 0x8c651fa4,r6 */
L8c651f54: *stop=0x8c651f54u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651f56; /* mov.l @r2,r3 */
L8c651f56: *stop=0x8c651f56u;*sr=(*sr&~1u)|!(r[5]&r[3]);goto L8c651f58; /* tst r5,r3 */
L8c651f58: if((*sr&1u))goto L8c651fc0;goto L8c651f5a; /* bt 0x8c651fc0 */
L8c651f5a: *stop=0x8c651f5cu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[4]=loaded;}goto L8c651fd0; /* bra 0x8c651fd0 */
L8c651fc0: *stop=0x8c651fc0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652064u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651fc2; /* mov.l 0x8c652064,r3 */
L8c651fc2: *stop=0x8c651fc2u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c651fc4; /* mov.l @r3,r4 */
L8c651fc4: *stop=0x8c651fc4u;r[4]&=r[5];goto L8c651fc6; /* and r5,r4 */
L8c651fc6: *stop=0x8c651fc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652068u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651fc8; /* mov.l 0x8c652068,r0 */
L8c651fc8: *stop=0x8c651fc8u;r[4]<<=2;goto L8c651fca; /* shll2 r4 */
L8c651fca: *stop=0x8c651fcau;r[4]<<=2;goto L8c651fcc; /* shll2 r4 */
L8c651fcc: *stop=0x8c651fccu;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c651fce; /* shll r4 */
L8c651fce: *stop=0x8c651fceu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c651fd0; /* mov.l @(r0,r4),r4 */
L8c651fd0: *stop=0x8c651fd0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652064u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651fd2; /* mov.l 0x8c652064,r2 */
L8c651fd2: *stop=0x8c651fd2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651fd4; /* mov.l @r2,r3 */
L8c651fd4: *stop=0x8c651fd4u;r[1]=r[7];goto L8c651fd6; /* mov r7,r1 */
L8c651fd6: *stop=0x8c651fd6u;r[3]&=r[5];goto L8c651fd8; /* and r5,r3 */
L8c651fd8: *stop=0x8c651fd8u;r[1]+=36u;goto L8c651fda; /* add #0x24,r1 */
L8c651fda: *stop=0x8c651fdau;r[3]<<=2;goto L8c651fdc; /* shll2 r3 */
L8c651fdc: *stop=0x8c651fdcu;r[3]+=r[1];goto L8c651fde; /* add r1,r3 */
L8c651fde: *stop=0x8c651fdeu;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651fe0; /* mov.l @r6,r0 */
L8c651fe0: *stop=0x8c651fe0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c651fe2; /* mov.l r0,@r3 */
L8c651fe2: *stop=0x8c651fe2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652070u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651fe4; /* mov.l 0x8c652070,r0 */
L8c651fe4: *stop=0x8c651fe4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65206cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651fe6; /* mov.l 0x8c65206c,r3 */
L8c651fe6: *stop=0x8c651fe6u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c651fe8; /* mov.l @r0,r1 */
L8c651fe8: *stop=0x8c651fe8u;r[0]=r[2];goto L8c651fea; /* mov r2,r0 */
L8c651fea: *stop=0x8c651feau;r[1]&=r[3];goto L8c651fec; /* and r3,r1 */
L8c651fec: *stop=0x8c651fecu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651fee; /* mov.l @r0,r2 */
L8c651fee: *stop=0x8c651feeu;r[2]&=r[5];goto L8c651ff0; /* and r5,r2 */
L8c651ff0: *stop=0x8c651ff0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c652074u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651ff2; /* mov.l 0x8c652074,r0 */
L8c651ff2: *stop=0x8c651ff2u;r[2]<<=2;goto L8c651ff4; /* shll2 r2 */
L8c651ff4: *stop=0x8c651ff4u;r[2]<<=2;goto L8c651ff6; /* shll2 r2 */
L8c651ff6: *stop=0x8c651ff6u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c651ff8; /* shll r2 */
L8c651ff8: *stop=0x8c651ff8u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651ffa; /* mov.l @(r0,r2),r2 */
L8c651ffa: *stop=0x8c651ffau;r[3]=r[4];goto L8c651ffc; /* mov r4,r3 */
L8c651ffc: *stop=0x8c651ffcu;r[3]-=r[2];goto L8c651ffe; /* sub r2,r3 */
L8c651ffe: *stop=0x8c651ffeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c652078u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c652000; /* mov.l 0x8c652078,r2 */
L8c652000: *stop=0x8c652000u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c652002; /* shll r3 */
L8c652002: *stop=0x8c652002u;r[3]&=r[2];goto L8c652004; /* and r2,r3 */
L8c652004: *stop=0x8c652004u;r[1]|=r[3];goto L8c652006; /* or r3,r1 */
L8c652006: *stop=0x8c652006u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65207cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c652008; /* mov.l 0x8c65207c,r3 */
L8c652008: *stop=0x8c652008u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c65200a; /* mov.l r1,@r3 */
L8c65200a: *stop=0x8c65200au;{uint32_t loaded=rd(m,bus,&io_status,0x8c652084u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65200c; /* mov.l 0x8c652084,r2 */
L8c65200c: *stop=0x8c65200cu;r[5]=48u;goto L8c65200e; /* mov #0x30,r5 */
L8c65200e: *stop=0x8c65200eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c652080u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c652010; /* mov.l 0x8c652080,r6 */
L8c652010: *stop=0x8c652010u;r[4]+=r[2];goto L8c652012; /* add r2,r4 */
L8c652012: *stop=0x8c652012u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}r[6]+=4;goto L8c652014; /* mov.l @r6+,r2 */
L8c652014: *stop=0x8c652014u;r[5]+=4294967295u;goto L8c652016; /* add #-0x1,r5 */
L8c652016: *stop=0x8c652016u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c652018; /* tst r5,r5 */
L8c652018: *stop=0x8c652018u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c65201a; /* mov.l r2,@r4 */
L8c65201a: take=!(*sr&1u);*stop=0x8c65201cu;r[4]+=4u;if(take)goto L8c652012;goto L8c65201e; /* bf/s 0x8c652012 */
L8c65201e: *stop=0x8c65201eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c652088u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c652020; /* mov.l 0x8c652088,r3 */
L8c652020: *stop=0x8c652020u;r[2]=0u;goto L8c652022; /* mov #0x0,r2 */
L8c652022: *stop=0x8c652022u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c652024; /* mov.l r2,@r3 */
L8c652024: *stop=0x8c652024u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65208cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c652026; /* mov.l 0x8c65208c,r0 */
L8c652026: *stop=0x8c652026u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c652028; /* mov.l @r0,r2 */
L8c652028: *stop=0x8c652028u;r[0]=72u;goto L8c65202a; /* mov #0x48,r0 */
L8c65202a: *stop=0x8c65202au;if(!valid_access(r[0]+r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65202c; /* mov.l @(r0,r7),r1 */
L8c65202c: *stop=0x8c65202cu;r[0]=76u;goto L8c65202e; /* mov #0x4c,r0 */
L8c65202e: *stop=0x8c65202eu;r[1]-=r[2];goto L8c652030; /* sub r2,r1 */
L8c652030: *stop=0x8c652030u;if(*pr!=entry_pr)return -3;*stop=0x8c652032u;if(!valid_access(r[0]+r[7],4u))return -3;wr(m,bus,&io_status,r[7]+r[0],r[1],4);if(io_status)return io_status;return 0; /* rts */
}
