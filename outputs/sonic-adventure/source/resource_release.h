#ifndef SA_RESOURCE_RELEASE_H
#define SA_RESOURCE_RELEASE_H
#include "stage00_task.h"
typedef uint32_t (*sa_resource_release_invoke)(void *,uint32_t,uint32_t,uint32_t *);
SA_STAGE_EXPORT uint32_t sa_resource_release_8c64ddc0(uint8_t *,uint32_t *,uint32_t,void *,sa_resource_release_invoke);
#endif
