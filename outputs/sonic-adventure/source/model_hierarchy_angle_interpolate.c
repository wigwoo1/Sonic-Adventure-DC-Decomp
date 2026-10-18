#include "fpu_cleanup_transfer.h"
#include "sh4_float_i32.h"
uint32_t sa_model_channel_difference_8c055d4e(uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)ram[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
/* d8c..dfa / fe0..804e, before rotation call. Native difference/arithmetic. */
SA_STAGE_EXPORT int sa_model_hierarchy_angle_interpolate(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *r,uint32_t *fpul,unsigned mode){
 if(mode!=2&&mode!=3)return -1;if(f->fpscr&0x180000u)return -1;unsigned b=(f->fpscr>>21)&1u;uint32_t *fr=f->bank[b];int status;
 r[3]=r[5]+4u;wr(ram,r[15],r[3]);r[4]+=4u;r[3]=rd(ram,r[4]);r[4]+=4u;wr(ram,r[15]+8u,r[3]);r[3]=rd(ram,r[4]);r[4]+=4u;wr(ram,r[15]+4u,r[3]);r[2]=rd(ram,r[4]);wr(ram,r[15]+12u,r[2]);
 for(unsigned i=0;i<3;i++){r[5]=rd(ram,r[15]);if(i<2){r[5]+=4u;wr(ram,r[15],r[5]);r[5]-=4u;}r[5]=rd(ram,r[5]);r[4]=rd(ram,r[15]+(i==0?8u:i==1?4u:12u));sa_model_channel_difference_8c055d4e(r);if(i<2)wr(ram,r[15]+(mode==2?(i==0?16u:20u):(i==0?20u:16u)),r[0]);}
 r[3]=rd(ram,r[15]+(mode==2?16u:20u));r[4]=r[0];r[2]=rd(ram,r[15]+(mode==2?20u:16u));*fpul=r[3];r[1]=rd(ram,r[15]+12u);r[3]=rd(ram,r[15]+8u);
 status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status;status=sa_sh4_multiply(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;status=sa_sh4_truncate_i32(fr[3],&f->fpscr,fpul);if(status)return status;r[5]=*fpul;*fpul=r[2];r[2]=rd(ram,r[15]+4u);r[5]+=r[3];
 status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status;status=sa_sh4_multiply(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;status=sa_sh4_truncate_i32(fr[3],&f->fpscr,fpul);if(status)return status;r[6]=*fpul;*fpul=r[4];r[6]+=r[2];status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status;fr[2]=fr[3];status=sa_sh4_multiply(&fr[2],&fr[15],&f->fpscr,&fr[2]);if(status)return status;status=sa_sh4_truncate_i32(fr[2],&f->fpscr,fpul);if(status)return status;r[7]=*fpul;r[7]+=r[1];return 0;
}
