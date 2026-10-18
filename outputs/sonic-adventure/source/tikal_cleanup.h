#ifndef SA_TIKAL_CLEANUP_H
#define SA_TIKAL_CLEANUP_H
#include "stage00_task.h"
SA_STAGE_EXPORT uint32_t sa_tikal_cleanup_0cb00780(uint8_t *,uint32_t *,uint32_t,
 void *,uint32_t (*)(void *,uint32_t,uint32_t,uint32_t *));
SA_STAGE_EXPORT uint32_t sa_tikal_cleanup_with_owner_release(uint8_t *,uint32_t *,uint32_t,
 void *,uint32_t (*)(void *,uint32_t,uint32_t,uint32_t *));
/* argc distinguishes meaningful one-argument services from the three-argument detach. */
typedef uint32_t (*sa_tikal_nested_invoke)(void *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *);
SA_STAGE_EXPORT uint32_t sa_tikal_cleanup_connected(uint8_t *,uint32_t *,uint32_t,
 void *,sa_tikal_nested_invoke);
#endif
