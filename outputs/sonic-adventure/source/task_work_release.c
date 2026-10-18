/* Original 0x8c098998..0x8c0989c7. Re-read task work after each callback:
 * resource release is permitted to replace the task's current work pointer. */
#include "task_work_release.h"
SA_STAGE_EXPORT void sa_task_work_release_8c098998(struct sa_created_task *task,
 const struct sa_task_work_release_services *services) {
 if(((uint32_t *)task->slots[8])[14])
  services->release_8c0344a0(services->context,task->slots[8]);
 if(((uint32_t *)task->slots[8])[15])
  services->release_8c04b468(services->context,task->slots[8]);
 services->free_8c098b42(services->context,task->slots[8]);
 task->slots[8]=0;
}
