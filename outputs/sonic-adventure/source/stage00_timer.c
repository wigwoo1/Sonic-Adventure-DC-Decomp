/* Original integer instructions 0x0c900d76..0x0c900db1.
 * Work offset 6 is a wrapping signed 16-bit countdown in this callback.
 * The original keeps its initial work pointer even if a service changes task->work.
 */
#include "stage00_timer.h"
SA_STAGE_EXPORT void sa_stage00_timer_0c900d76(struct sa_stage00_task *task,
 const struct sa_stage00_timer_services *services) {
 struct sa_stage00_work *work=task->work;
 work->cached_variant=(uint16_t)(work->cached_variant-1u);
 if(work->cached_variant & 0x8000u) {
  services->call_8c09852e(services->context,0,1,0x8c107864u);
  int32_t state=work->state;
  if(state>=128)state-=256;
  services->call_8c107ac6(services->context,state);
  services->call_8c09865c(services->context,task);
 }
}
