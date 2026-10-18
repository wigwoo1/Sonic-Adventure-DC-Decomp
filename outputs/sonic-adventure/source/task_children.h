#ifndef SA_TASK_CHILDREN_H
#define SA_TASK_CHILDREN_H
#include "task_create.h"
struct sa_task_children_services {
 void *context;
 void (*process_8c098e98)(void *,struct sa_created_task *);
 void (*dispose_8c09865c)(void *,struct sa_created_task *);
};
/* Callback fields currently use original address tokens for identity checks. */
SA_STAGE_EXPORT void sa_task_children_8c098e60(struct sa_created_task *,
 const struct sa_task_children_services *);
SA_STAGE_EXPORT void sa_task_process_children_8c098e98(struct sa_created_task *,
 void *context,void (*invoke)(void *,uintptr_t,struct sa_created_task *));
#endif
