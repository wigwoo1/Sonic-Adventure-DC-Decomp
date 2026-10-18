/* Recovered task allocation wrapper, 0x8c09852e..0x8c0985eb.
 * Allocation, base-task creation and disposal bodies remain service dependencies.
 */
#include "task_create.h"
SA_STAGE_EXPORT struct sa_created_task *sa_task_create_8c09852e(
 uint32_t flags,uint32_t level,uintptr_t callback,const struct sa_task_create_services *services) {
 struct sa_created_task *task=services->create_8c0985ec(services->context,level,callback);
 if(!task)return 0;
 static const uint16_t bits[4]={2,4,1,8};
 static const uint8_t kinds[4]={1,2,1,1},sizes[4]={64,28,64,16},slots[4]={8,10,9,11};
 for(unsigned i=0;i<4;i++) {
  if((uint16_t)flags & bits[i]) {
   void *allocation=services->allocate_8c098b0c(services->context,kinds[i],sizes[i]);
   if(!allocation) {
    services->dispose_8c09865c(services->context,task);
    return 0;
   }
   task->slots[slots[i]]=allocation;
  }
 }
 return task;
}
