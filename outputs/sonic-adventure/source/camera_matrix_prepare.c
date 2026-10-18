#include "fpu_cleanup_transfer.h"
static uint32_t rd(uint8_t *m,uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
/* Retail 8c057b40..8c057bac, including old-XF save and paired transfers. */
SA_STAGE_EXPORT int sa_camera_matrix_prepare(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r){
 r[1]=0x8c88f698u;r[6]=0x8c88f69cu;r[3]=rd(m,r[1]);r[2]=rd(m,r[6]);
 *sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]);
 if(*sr&1u){r[0]=0;return 0;}
 if(f->fpscr&0x180000u)return -1;
 r[2]++;r[1]=0x8c88f5f8u;wr(m,r[6],r[2]);r[5]=rd(m,r[1])+64u;wr(m,r[1],r[5]);r[4]=r[5];
 unsigned b=(f->fpscr>>21)&1u;uint32_t *fr=f->bank[b],*xf=f->bank[b^1u];
 fr[0]=0x3f800000u;fr[1]=0;fr[2]=0;fr[3]=0x3f800000u;fr[4]=0;fr[5]=0;
 f->fpscr^=0x100000u;
 for(unsigned i=16;i;i-=2){r[5]-=8;wr(m,r[5],xf[i-2]);wr(m,r[5]+4,xf[i-1]);}
 r[4]+=64;
 xf[0]=fr[0];xf[1]=fr[1];xf[2]=fr[4];xf[3]=fr[5];xf[4]=fr[2];xf[5]=fr[3];xf[6]=fr[4];xf[7]=fr[5];
 xf[8]=fr[4];xf[9]=fr[5];xf[10]=fr[0];xf[11]=fr[1];xf[12]=fr[4];xf[13]=fr[5];xf[14]=fr[2];xf[15]=fr[3];
 for(unsigned i=16;i;i-=2){r[4]-=8;wr(m,r[4],xf[i-2]);wr(m,r[4]+4,xf[i-1]);}
 f->fpscr^=0x100000u;r[0]=1;return 0;
}
