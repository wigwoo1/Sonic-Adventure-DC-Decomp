/* Original 0x8c098b0c..0x8c098b41; heap dependency connects through services. */
#include "task_allocate.h"
SA_STAGE_EXPORT void *sa_task_allocate_zero_8c098b0c(uint32_t count,uint32_t size,
 const struct sa_task_allocator *allocator) {
 uint32_t total=((size+3u)&~3u)*count;
 void *result=allocator->allocate_8c098a80(allocator->context,total);
 if(result) {
  volatile uint32_t *words=result;
  for(uint32_t i=0;i<(total>>2);i++)words[i]=0;
 }
 return result;
}
