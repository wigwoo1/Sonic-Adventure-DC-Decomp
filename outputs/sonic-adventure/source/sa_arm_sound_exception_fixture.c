/* Native original exception/input fixture; caller has already entered exception. */
#include <stdint.h>
#include "arm_sound_exception.h"
struct sa_arm_state {uint32_t r[16],cpsr;};
typedef int (*sa_arm_read)(void *,uint32_t,unsigned,uint32_t *);
typedef int (*sa_arm_write)(void *,uint32_t,uint32_t,unsigned);
int sa_arm_sound_exception_step(uint8_t *,struct sa_arm_state *,struct sa_arm_exception_banks *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
int sa_arm_sound_input_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
__declspec(dllexport) int sa_arm_sound_exception_native(uint8_t *ram,struct sa_arm_state *state,struct sa_arm_exception_banks *banks,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,uint32_t stop_pc,uint32_t budget){
 if(!ram||!state||!banks||!read||!write||driver>1||banks->returned||(banks->spsr&0x0100003fu)!=0x13u||(state->cpsr&0x01000020u))return -2;
 uint32_t mode=state->cpsr&31u;
 if(mode!=0x11u&&mode!=0x12u&&mode!=0x13u&&mode!=0x17u&&mode!=0x1bu)return -2;
 int started=0;
 for(;;){
  if(started&&state->r[15]==stop_pc)return 0;
  if(banks->returned)return -8;
  if(!budget)return -3;
  --budget;started=1;int handled=0;
  int rc=sa_arm_sound_exception_step(ram,state,banks,driver,user,read,write,&handled);
  if(rc)return rc;
  if(handled)continue;
  rc=sa_arm_sound_input_step(ram,state,driver,user,read,write,&handled);
  if(rc)return rc;
  if(!handled)return -8;
 }
}
