#include "stage00_task.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void sh(uint8_t *ram,uint32_t a,uint32_t v){at(ram,a)[0]=(uint8_t)v;at(ram,a)[1]=(uint8_t)(v>>8);}
/* Shared worker finalization 8c07a456..8c07a484; caller restores its frame. */
SA_STAGE_EXPORT uint32_t sa_owner_model_change_finish(uint8_t *ram,uint32_t *sr,uint32_t config,uint32_t current_mode,uint32_t previous_mode,uint32_t cached_mode){
 int triple=current_mode==3u || previous_mode==3u;*sr=(*sr&~1u)|(uint32_t)triple;
 uint32_t header=rd(ram,rd(ram,config+40u)+4u);sh(ram,header+8u,triple?7u:3u);
 uint32_t result=triple?3u:cached_mode;header=rd(ram,rd(ram,config+40u)+4u);sh(ram,header+10u,result);return result;
}
