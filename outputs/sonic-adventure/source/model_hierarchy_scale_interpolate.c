#include "fpu_cleanup_transfer.h"
#include "sh4_fmac.h"
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
/* Original 8056..8086, ending before scale call. */
SA_STAGE_EXPORT int sa_model_hierarchy_scale_interpolate(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *r){
 if(f->fpscr&0x180000u)return -1;unsigned b=(f->fpscr>>21)&1u;uint32_t *fr=f->bank[b];int status;
 r[6]=r[4];r[4]=r[5]+4u;fr[0]=fr[15];r[6]+=4u;fr[3]=rd(ram,r[4]);r[4]+=4u;fr[4]=rd(ram,r[6]);r[6]+=4u;r[3]=0x8c63a69cu;
 status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;fr[2]=fr[4];fr[5]=rd(ram,r[6]);r[6]+=4u;fr[6]=rd(ram,r[6]);
 status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;fr[3]=rd(ram,r[4]);r[4]+=4u;status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;fr[4]=fr[2];fr[2]=fr[5];
 status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;fr[3]=rd(ram,r[4]);status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;fr[5]=fr[2];fr[2]=fr[6];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;fr[6]=fr[2];return 0;
}

int sa_model_matrix_scale_registers(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
SA_STAGE_EXPORT int sa_model_hierarchy_scale_native(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r){int status=sa_model_hierarchy_scale_interpolate(ram,f,r);if(status)return status;r[4]=0;return sa_model_matrix_scale_registers(ram,f,sr,r);}
