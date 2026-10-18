#ifndef SA_RESOURCE_RESET_RUNTIME_H
#define SA_RESOURCE_RESET_RUNTIME_H
#include "resource_release.h"
typedef uint32_t (*sa_resource_divide)(void *,uint32_t,uint32_t,uint32_t *);
SA_STAGE_EXPORT uint32_t sa_resource_release_with_reset(uint8_t *,uint32_t *,uint32_t,void *,sa_resource_release_invoke,sa_resource_divide);
#endif
