#include "fpu_cleanup_transfer.h"
#include "sh4_fmac.h"
static uint32_t rd(uint8_t *m,uint32_t a){uint8_t *p=m+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
/* Original Tikal 0cb00994..0cb009de. Finite normal/zero FCMP scope,
 * PR=SZ=0; scalar FMAC uses the recovered integer binary32 core. */
SA_STAGE_EXPORT int sa_tikal_transition(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r){
 if(f->fpscr&0x180000u)return -1;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];
 r[0]=56;fr[4]=rd(m,r[6]+r[0]);r[0]=0x0cb00a20;fr[0]=rd(m,r[0]);
 r[0]=56;fr[3]=rd(m,r[5]+r[0]);r[0]=292;
 uint8_t *mode=m+((r[6]+r[0])&0xffffffu);r[0]=mode[0]|((uint32_t)mode[1]<<8);
 unsigned skip=r[0]==8;*sr=(*sr&~1u)|skip;
 int status=sa_sh4_fmac(&fr[0],&fr[3],&fr[4],&f->fpscr,&fr[4]);if(status)return status;
 if(skip)return 0;
 r[0]=192;fr[3]=rd(m,r[6]+r[0]);uint32_t a=fr[4],b=fr[3];
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;int gt;
 if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))gt=0;
 else if((a^b)&0x80000000u)gt=!(a&0x80000000u);
 else gt=(a&0x80000000u)?a<b:a>b;
 *sr=(*sr&~1u)|gt;r[0]=292;if(gt)r[2]=2;else r[1]=1;
 mode[0]=gt?2:1;mode[1]=0;
 r[0]=56;fr[3]=rd(m,r[6]+r[0]);r[0]=0x0cb00a24;fr[0]=rd(m,r[0]);
 r[0]=56;fr[2]=rd(m,r[5]+r[0]);status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;
 uint8_t *p=m+(r[6]&0xffffffu);for(unsigned i=0;i<4;i++)p[i]=(uint8_t)(fr[3]>>(8*i));return 0;
}
