/* Bounded native ramp fixture, with no external ARM services. */
#include <stdint.h>
struct sa_arm_state {uint32_t r[16],cpsr;};
typedef int (*sa_arm_read)(void *,uint32_t,unsigned,uint32_t *);
typedef int (*sa_arm_write)(void *,uint32_t,uint32_t,unsigned);
int sa_arm_sound_ramp_step(uint8_t *,struct sa_arm_state *,uint32_t,void *,sa_arm_read,sa_arm_write,int *);
__declspec(dllexport) int sa_arm_sound_ramp_native(uint8_t *ram,struct sa_arm_state *state,uint32_t driver,
 void *user,sa_arm_read read,sa_arm_write write,uint32_t stop_pc,uint32_t budget){
 if(!ram||!state||!read||!write||driver>1||(state->cpsr&0x3fu)!=0x13u)return -2;
 int started=0;
 for(;;){
  if(started&&state->r[15]==stop_pc)return 0;
  if(!budget)return -3;
  --budget;started=1;int handled=0;
  int rc=sa_arm_sound_ramp_step(ram,state,driver,user,read,write,&handled);
  if(rc)return rc;
  if(!handled)return -8;
 }
}
