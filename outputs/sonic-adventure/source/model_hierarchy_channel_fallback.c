#include "fpu_cleanup_transfer.h"
typedef int (*sa_channel_transform)(void *,uint32_t,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)ram[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
SA_STAGE_EXPORT int sa_model_hierarchy_channel_dispatch(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *next){
 if(f->fpscr&0x180000u)return -1;r[0]=0x8c057cacu;r[12]=0x8c055d4eu;f->bank[(f->fpscr>>21)&1u][14]=0x4f000000u;r[0]=r[9];r[11]=0x8c639cf4u;r[10]=2;
 *sr=(*sr&~1u)|(r[0]==2);if(r[0]==2){r[8]=rd(ram,r[14]);r[9]=0;*sr&=~1u;*next=0x8c057c36u;}
 else{*sr=(*sr&~1u)|(r[0]==3);if(r[0]==3){r[8]=rd(ram,r[14]);r[9]=0;r[3]=3;*sr&=~1u;*next=0x8c057e22u;}else *next=0x8c05809cu;}return 0;
}
/* Null channel entry c44/e30 through the common increment boundary.
 * Transform callees remain explicit services, not recovered by this fragment. */
SA_STAGE_EXPORT int sa_model_hierarchy_channel_fallback(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,unsigned mode,void *ctx,sa_channel_transform call,uint32_t *next){
 if(mode!=2&&mode!=3)return -1;uint32_t target=0,pr=0;r[0]=r[9];*sr=(*sr&~1u)|(r[0]==0);
 if(r[0]==0){if(mode==3){r[0]=rd(ram,r[13]);*sr=(*sr&~1u)|((r[0]&1u)==0);if(r[0]&1u)goto end;r[2]=0x8c63a804u;pr=0x8c057e94u;}else{r[3]=0x8c63a804u;pr=0x8c057c5cu;}r[5]=r[13]+8u;target=0x8c63a804u;}
 else{*sr=(*sr&~1u)|(r[0]==1);if(r[0]==1){uint32_t flags=rd(ram,r[13]);if(mode==2)r[3]=flags;else r[2]=flags;*sr=(*sr&~1u)|((flags&r[10])==0);if(flags&r[10])goto end;r[7]=r[13]+20u;wr(ram,r[15],r[7]);r[3]=rd(ram,r[15]);r[7]=rd(ram,r[7]+8u);r[5]=rd(ram,r[3]);r[6]=rd(ram,r[3]+4u);target=r[11];pr=mode==2?0x8c057c7cu:0x8c057eb4u;}
 else if(mode==3){*sr=(*sr&~1u)|(r[0]==2);if(r[0]==2){r[0]=rd(ram,r[13]);*sr=(*sr&~1u)|((r[0]&4u)==0);if(r[0]&4u)goto end;r[3]=0x8c63a5ecu;r[5]=r[13]+32u;target=r[3];pr=0x8c057eccu;}}}
 if(target){r[4]=0;if(!call)return -1;int status=call(ctx,target,pr,ram,f,sr,r);if(status)return status;}
 end:*next=mode==2?0x8c057dfeu:0x8c05808au;return 0;
}
