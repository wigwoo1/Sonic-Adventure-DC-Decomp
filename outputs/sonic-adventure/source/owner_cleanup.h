#ifndef SA_OWNER_CLEANUP_H
#define SA_OWNER_CLEANUP_H
#include "stage00_task.h"
struct sa_owner_cleanup_services {
 void *context;
 uint32_t (*free_8c098b42)(void *,uint32_t,uint32_t *);
 uint32_t (*helper_8c079476)(void *,uint32_t,uint32_t *);
 uint32_t (*detach_8c07a55c)(void *,uint32_t,uint32_t,uint32_t,uint32_t *);
 uint32_t (*finish_8c07d6c0)(void *,uint32_t,uint32_t *);
};
SA_STAGE_EXPORT uint32_t sa_owner_cleanup_8c06c5ca(uint8_t *,uint32_t *,uint32_t,
 const struct sa_owner_cleanup_services *);
#endif
