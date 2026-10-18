#ifndef SA_STAGE00_TIMER_H
#define SA_STAGE00_TIMER_H
#include "stage00_task.h"
/* Service names express only confirmed call arguments, not unrecovered semantics. */
struct sa_stage00_timer_services {
 void *context;
 void (*call_8c09852e)(void *,int32_t,int32_t,uint32_t);
 void (*call_8c107ac6)(void *,int32_t);
 void (*call_8c09865c)(void *,struct sa_stage00_task *);
};
SA_STAGE_EXPORT void sa_stage00_timer_0c900d76(struct sa_stage00_task *,
 const struct sa_stage00_timer_services *);
#endif
