/* Authored native host support, not recovered game code.
 * Executes an already captured transfer using caller-provided mapped buses.
 * No register timing, physical cache, exception entry or IRQ delivery is modeled.
 */
#include "g2_transfer_host.h"
#include "fpu_cleanup_transfer.h"
SA_STAGE_EXPORT int sa_g2_transfer_execute(const struct sa_g2_transfer *job,const struct sa_g2_bus *bus,uint32_t *completed,uint32_t *event){
 if(!completed||!event)return -1;
 *completed=0;*event=0;
 if(!job||!bus||!bus->read||!bus->write||job->channel>=4||job->direction>1)return -1;
 uint32_t length=job->length&0x7fffffffu;
 uint32_t source=job->direction?job->g2_address:job->root_address;
 uint32_t destination=job->direction?job->root_address:job->g2_address;
 if((length&31u)||length>0xffffffffu-source||length>0xffffffffu-destination)return -2;
 uint8_t chunk[32];
 while(*completed<length){
  int result=bus->read(bus->user,source+*completed,chunk,32);
  if(result)return result;
  result=bus->write(bus->user,destination+*completed,chunk,32);
  if(result)return result;
  *completed+=32;
 }
 *event=0xa60u+job->channel*0x20u;
 return 0;
}

SA_STAGE_EXPORT int sa_g2_transfer_capture(uint32_t channel,const uint32_t *registers,struct sa_g2_transfer *job){
 if(!registers||!job||channel>=4)return -1;
 if(!(registers[5]&1u)||!(registers[6]&1u))return 1;
 if(registers[3]>1)return -2;
 *job=(struct sa_g2_transfer){channel,registers[0],registers[1],registers[2],registers[3]};
 return 0;
}
