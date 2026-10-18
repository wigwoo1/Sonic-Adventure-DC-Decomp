#include "owner_helper_state2.h"
#include "owner_helper_tail.h"
SA_STAGE_EXPORT int sa_owner_helper_state2_with_tail(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t index,void *context,sa_owner_state0_service service,uint32_t *r0){
 int status=sa_owner_helper_state2(ram,sr,fpscr,config,index,context,service,r0);
 if(status==1)return sa_owner_helper_tail(ram,sr,fpscr,fpul,config,r0);
 return status;
}
