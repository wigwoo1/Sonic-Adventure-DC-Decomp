#ifndef SA_TASK_LISTS_H
#define SA_TASK_LISTS_H
#include "task_create.h"
struct sa_task_lists {
 struct sa_created_task *free_head;
 struct sa_created_task *level_heads[8];
};
/* Retail levels 0..8: 8 maps to 7, while 7 bypasses active-list insertion.
 * Out-of-range levels access other retail memory and are not valid native input.
 * Null callbacks retain the original default address as an unresolved token. */
SA_STAGE_EXPORT struct sa_created_task *sa_base_task_create_8c0985ec(
 struct sa_task_lists *,uint32_t level,uintptr_t callback);
#endif
