#include "task_scheduler.h"
/* Original instructions 8c09878c..8c0987ce. Empty lists leave the
 * diagnostic callback global untouched. Capture next before each callback. */
void sa_task_dispatch_8c09878c(struct sa_task_scheduler *scheduler,uint32_t level)
{
 if(level==8) level=7;
 struct sa_created_task *task=scheduler->lists->level_heads[level];
 while(task) {
  uintptr_t callback=(uintptr_t)task->slots[4];
  struct sa_created_task *next=task->slots[0];
  scheduler->active_callback=callback;
  if(callback) scheduler->invoke(scheduler->context,callback,task);
  task=next;
  scheduler->active_callback=0;
 }
}
/* Saved sibling is captured before invocation; child head is read afterwards,
 * so callbacks can change which descendants are visited. */
void sa_task_secondary_children_8c098842(struct sa_task_scheduler *scheduler,struct sa_created_task *parent)
{
 struct sa_created_task *task=parent->slots[3];
 while(task) {
  uintptr_t callback=(uintptr_t)task->slots[5];
  struct sa_created_task *next=task->slots[0];
  if(callback) scheduler->invoke(scheduler->context,callback,task);
  if(task->slots[3]) sa_task_secondary_children_8c098842(scheduler,task);
  task=next;
 }
}
void sa_task_secondary_dispatch_8c098800(struct sa_task_scheduler *scheduler,uint32_t level)
{
 if(level==8) level=7;
 struct sa_created_task *task=scheduler->lists->level_heads[level];
 while(task) {
  uintptr_t callback=(uintptr_t)task->slots[5];
  struct sa_created_task *next=task->slots[0];
  if(callback) scheduler->invoke(scheduler->context,callback,task);
  if(task->slots[3]) sa_task_secondary_children_8c098842(scheduler,task);
  task=next;
 }
}
