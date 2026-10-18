/* Recovered 0x8c0985ec..0x8c09865b; native pointers replace SH-4 task addresses. */
#include "task_lists.h"
SA_STAGE_EXPORT struct sa_created_task *sa_base_task_create_8c0985ec(
 struct sa_task_lists *lists,uint32_t level,uintptr_t callback) {
 struct sa_created_task *task=lists->free_head;
 if(!task)return 0;
 struct sa_created_task *next=task->slots[0];
 if(next)next->slots[1]=0;
 lists->free_head=next;
 task->slots[4]=(void *)(callback ? callback : (uintptr_t)0x8c098440u);
 if(level!=7) {
  if(level==8)level=7;
  struct sa_created_task *head=lists->level_heads[level];
  task->slots[1]=0;
  task->slots[0]=head;
  if(head)head->slots[1]=task;
  lists->level_heads[level]=task;
 }
 for(unsigned i=2;i<12;i++)if(i!=4)task->slots[i]=0;
 return task;
}
