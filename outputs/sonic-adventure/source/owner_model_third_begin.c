#include "owner_helper_state0.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static uint32_t half(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static void flag(uint32_t *sr,int v){*sr=(*sr&~1u)|(uint32_t)v;}
/* 8c07a1c4..8c07a23e mode-dependent third-channel allocation setup. */
SA_STAGE_EXPORT void sa_owner_model_third_begin(uint8_t *ram,uint32_t *sr,uint32_t frame,uint32_t current,uint32_t previous,uint32_t destination,void *context,sa_owner_state0_service service,uint32_t *out){
 uint32_t r0=rd(ram,frame+20u);flag(sr,r0==3u);
 if(!(*sr&1u)){r0=half(ram,frame+48u);flag(sr,r0==3u);if(!(*sr&1u)){out[0]=r0;out[1]=0x8c07a414u;return;}}
 wr(ram,frame+36u,current);wr(ram,frame+4u,previous);wr(ram,frame+16u,destination);
 wr(ram,destination+16u,rd(ram,destination+12u));wr(ram,rd(ram,frame+16u)+12u,rd(ram,destination+8u));
 r0=rd(ram,frame+24u);flag(sr,r0==3u);int allocate=0;
 if(*sr&1u){flag(sr,rd(ram,rd(ram,frame+36u)+8u)==0u);allocate=(*sr&1u)==0u;}
 if(!allocate){r0=rd(ram,frame+20u);flag(sr,r0==3u);if(*sr&1u){flag(sr,rd(ram,rd(ram,frame+4u)+8u)==0u);allocate=(*sr&1u)==0u;}}
 if(!allocate){out[0]=r0;out[1]=0x8c07a40cu;return;}
 r0=service(context,0x8c098a80u,1u,32u,0u,r0,sr);flag(sr,r0==0u);wr(ram,frame+8u,r0);
 if(r0==0u){out[0]=r0;out[1]=0x8c07a40cu;return;}
 wr(ram,rd(ram,frame+16u)+8u,r0);wr(ram,r0,0u);r0=rd(ram,frame+20u);flag(sr,r0==3u);
 if(!(*sr&1u)){out[0]=r0;out[1]=0x8c07a34eu;return;}
 flag(sr,rd(ram,rd(ram,frame+4u)+8u)==0u);out[0]=r0;out[1]=(*sr&1u)?0x8c07a34eu:0x8c07a23eu;
}
