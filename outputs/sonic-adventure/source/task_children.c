/* Original 0x8c098e60..0x8c098e97. Child head is reloaded after every disposal. */
#include "task_children.h"
SA_STAGE_EXPORT void sa_task_children_8c098e60(struct sa_created_task *task,
 const struct sa_task_children_services *services) {
 if((uintptr_t)task->slots[4]==0x8c098e60u) {
  services->process_8c098e98(services->context,task);
  if(!task->slots[3])task->slots[4]=(void *)(uintptr_t)0x8c09865cu;
 } else {
  while(task->slots[3])services->dispose_8c09865c(services->context,task->slots[3]);
 }
}
/* Original 0x8c098e98..0x8c098eb9. Save each sibling before invoking callback. */
SA_STAGE_EXPORT void sa_task_process_children_8c098e98(struct sa_created_task *parent,
 void *context,void (*invoke)(void *,uintptr_t,struct sa_created_task *)) {
 struct sa_created_task *child=parent->slots[3];
 while(child) {
  uintptr_t callback=(uintptr_t)child->slots[4];
  struct sa_created_task *next=child->slots[0];
  invoke(context,callback,child);
  child=next;
 }
}
