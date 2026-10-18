#ifndef SA_TASK_DISPOSE_H
#define SA_TASK_DISPOSE_H
#include "task_lists.h"
struct sa_task_dispose_services {
 void *context;
 void (*invoke_destructor)(void *,uintptr_t,struct sa_created_task *);
 void (*release_children_8c098e60)(void *,struct sa_created_task *);
 void (*release_work_8c098998)(void *,struct sa_created_task *);
 void (*free_8c098b42)(void *,void *);
};
SA_STAGE_EXPORT void sa_task_dispose_8c09865c(struct sa_task_lists *,struct sa_created_task *,
 const struct sa_task_dispose_services *);
#endif
