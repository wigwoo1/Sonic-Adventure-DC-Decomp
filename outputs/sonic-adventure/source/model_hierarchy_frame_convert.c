#include "fpu_cleanup_transfer.h"
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
/* c80..cc4 / ed0..ee8. Normal/zero comparison scope; original correction. */
SA_STAGE_EXPORT int sa_model_hierarchy_frame_convert(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,unsigned mode){
 if((mode!=2&&mode!=3)||(f->fpscr&0x180000u))return -1;unsigned bank=(f->fpscr>>21)&1u;uint32_t *fr=f->bank[bank];r[0]=16;fr[4]=rd(ram,r[14]+r[0]);uint32_t a=fr[4],b=fr[14];for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;int gt;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))gt=0;else if((a^b)&0x80000000u)gt=!(a&0x80000000u);else gt=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|gt;
 if(gt){r[0]=mode==2?0x8c057cbcu:0x8c057fc8u;fr[2]=fr[4];fr[3]=0xcf800000u;int status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;fr[3]=fr[2];}else fr[3]=fr[4];return sa_sh4_truncate_i32(fr[3],&f->fpscr,fpul);
}
