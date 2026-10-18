/* Recovered 0x8c09865c..0x8c098785, using native projected pointer slots.
 * Child disposal, work cleanup, destructor dispatch and heap free use services.
 */
#include "task_dispose.h"
SA_STAGE_EXPORT void sa_task_dispose_8c09865c(struct sa_task_lists *lists,
 struct sa_created_task *task,const struct sa_task_dispose_services *services) {
 if(task->slots[6])services->invoke_destructor(services->context,(uintptr_t)task->slots[6],task);
 task->slots[4]=0;task->slots[5]=0;task->slots[6]=0;
 struct sa_created_task *next=task->slots[0],*previous=task->slots[1];
 if(task->slots[3])services->release_children_8c098e60(services->context,task);
 if(next && previous) {
  next->slots[1]=previous;previous->slots[0]=next;
 } else if(!next && previous)previous->slots[0]=0;
 else {
  struct sa_created_task *parent=task->slots[2];
  if(next)next->slots[1]=0;
  if(parent)parent->slots[3]=next;
  else for(unsigned i=0;i<8;i++)if(lists->level_heads[i]==task) {
   lists->level_heads[i]=next;break;
  }
 }
 if(task->slots[8])services->release_work_8c098998(services->context,task);
 static const unsigned slots[3]={10,9,11};
 for(unsigned i=0;i<3;i++)if(task->slots[slots[i]]) {
  services->free_8c098b42(services->context,task->slots[slots[i]]);
  task->slots[slots[i]]=0;
 }
 if(task->slots[7]) {
  uint8_t *attachment=task->slots[7];
  attachment[2]&=254u;
  task->slots[7]=0;
 }
 struct sa_created_task *free=lists->free_head;
 if(free)free->slots[1]=task;
 task->slots[0]=free;task->slots[1]=0;lists->free_head=task;
 task->slots[3]=0;task->slots[2]=0;
}
