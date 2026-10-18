#include "owner_helper_prepare.h"
#include "owner_helper_state0.h"
#include "owner_helper_state1.h"
#include "owner_helper_terminal.h"
int sa_owner_helper_state2_with_tail(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,void *,sa_owner_state0_service,uint32_t *);
int sa_owner_helper_state8(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,void *,sa_owner_state0_service,uint32_t *);
int sa_owner_helper_repeat(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t *);
int sa_owner_helper_advance(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t *);
int sa_owner_helper_clamp(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t *);
/* All original states are dispatched here. Nested services remain explicit. */
SA_STAGE_EXPORT int sa_owner_helper_dispatch_8c079476(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,void *context,sa_owner_state0_service service,uint32_t *r0){
 struct sa_owner_helper_selection selected;sa_owner_helper_prepare(ram,sr,config,&selected);
 uint32_t index=selected.descriptor_offset>>4,state=selected.state;
 switch(state){
 case 0:*r0=sa_owner_helper_state0(ram,sr,config,index,context,service);return 0;
 case 1:return sa_owner_helper_state1(ram,sr,fpscr,fpul,config,index,context,service,r0);
 case 2:return sa_owner_helper_state2_with_tail(ram,sr,fpscr,fpul,config,index,context,service,r0);
 case 3:case 9:case 10:case 12:return sa_owner_helper_repeat(ram,sr,fpscr,fpul,config,index,state,r0);
 case 4:case 6:case 11:return sa_owner_helper_advance(ram,sr,fpscr,fpul,config,index,state,r0);
 case 5:case 7:return sa_owner_helper_clamp(ram,sr,fpscr,fpul,config,index,state,r0);
 case 8:return sa_owner_helper_state8(ram,sr,fpscr,fpul,config,index,context,service,r0);
 default:*r0=sa_owner_helper_terminal(ram,sr,config,state<15u?1526u:state,context,service);return 0;
 }
}
