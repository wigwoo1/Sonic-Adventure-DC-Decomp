#include "task_runtime.h"
static void dispose(void *context,struct sa_created_task *task) {
 sa_task_runtime_dispose(context,task);
}
static void children(void *context,struct sa_created_task *task) {
 sa_task_runtime_children(context,task);
}
static void invoke(void *context,uintptr_t token,struct sa_created_task *task) {
 struct sa_task_runtime *runtime=context;
 if(token==0x8c09865cu)sa_task_runtime_dispose(runtime,task);
 else if(token==0x8c098e60u)sa_task_runtime_children(runtime,task);
 else runtime->invoke_external(runtime->context,token,task);
}
static void process(void *context,struct sa_created_task *task) {
 sa_task_process_children_8c098e98(task,context,invoke);
}
static void work(void *context,struct sa_created_task *task) {
 struct sa_task_runtime *runtime=context;
 runtime->release_work(runtime->context,task);
}
static void free_allocation(void *context,void *pointer) {
 struct sa_task_runtime *runtime=context;
 runtime->free_allocation(runtime->context,pointer);
}
SA_STAGE_EXPORT void sa_task_runtime_dispose(struct sa_task_runtime *runtime,struct sa_created_task *task) {
 struct sa_task_dispose_services services={runtime,invoke,children,work,free_allocation};
 sa_task_dispose_8c09865c(runtime->lists,task,&services);
}
SA_STAGE_EXPORT void sa_task_runtime_children(struct sa_task_runtime *runtime,struct sa_created_task *task) {
 struct sa_task_children_services services={runtime,process,dispose};
 sa_task_children_8c098e60(task,&services);
}
