/* Native C recovery of STG00 0x0c900d00..0x0c900d71.
 * External calls are labeled by address because their full semantics are not
 * recovered here. The final tail target 0x8c04c680 sets a main-binary global.
 */
#include "stage00_initialize.h"

SA_STAGE_EXPORT void sa_stage00_initialize_0c900d00(
 struct sa_stage00_task *task,const struct sa_stage00_initialize_services *services) {
 struct sa_stage00_work *work=task->work;
 work->cached_variant=(uint16_t)(services->query_variant(services->context)&0xffu);
 services->call_8c07c65e(services->context,0,0x18);
 services->call_8c07c676(services->context,0,0,0,0);
 services->call_8c60526a(services->context,0,0,0);
 struct sa_stage00_task *timer_task=services->call_8c09852e(
  services->context,2,0,0x0c900d76u);
 timer_task->work->state=0x2f;
 timer_task->work->cached_variant=10;
 (void)services->call_8c014242(services->context,0x20,2);
 (void)services->call_8c014308(services->context,0x20,2);
 services->call_8c08a07c(services->context);
 services->call_8c04c680(services->context);
}
