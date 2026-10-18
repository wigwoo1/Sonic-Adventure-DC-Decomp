#include "owner_helper_state0.h"
void sa_owner_model_change_initialize(uint8_t *,uint32_t *,uint32_t,uint32_t,uint32_t *);
void sa_owner_model_change_advance(uint8_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *);
uint32_t sa_owner_model_change_finish(uint8_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t);
int sa_owner_model_first_channel(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,void *,sa_owner_state0_service,uint32_t *);
int sa_owner_model_angle_channel(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,void *,sa_owner_state0_service,uint32_t *);
int sa_owner_model_third_channel(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,void *,sa_owner_state0_service,uint32_t *);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+((a&0x1fffffffu)-0x0c000000u);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
/* Worker body 8c079c90..8c07a484. Caller owns original allocated frame.
 * Allocator callback is the remaining external dependency. */
SA_STAGE_EXPORT int sa_owner_model_change_worker(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t frame,void *context,sa_owner_state0_service service,uint32_t *r0){
 uint32_t init[6],out[3],advance[5];sa_owner_model_change_initialize(ram,sr,config,frame,init);
 uint32_t current=init[1],previous=init[2],destination=init[3];*r0=init[4];
 while(init[5]!=0x8c07a456u){
  if(sa_owner_model_first_channel(ram,sr,fpscr,fpul,config,frame,current,previous,destination,*r0,context,service,out)!=0)return -1;
  if(sa_owner_model_angle_channel(ram,sr,fpscr,fpul,config,frame,current,previous,destination,out[0],out[1],context,service,out)!=0)return -1;
  if(sa_owner_model_third_channel(ram,sr,fpscr,fpul,config,frame,current,previous,destination,out[0],out[1],context,service,out)!=0)return -1;
  sa_owner_model_change_advance(ram,sr,config,frame,current,previous,destination,advance);
  current=advance[0];previous=advance[1];destination=advance[2];*r0=advance[3];init[5]=advance[4];
 }
 *r0=sa_owner_model_change_finish(ram,sr,config,rd(ram,frame+24u),rd(ram,frame+20u),init[0]);return 0;
}
