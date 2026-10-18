#ifndef SA_EVENT_REGISTRY_H
#define SA_EVENT_REGISTRY_H
#include "stage00_task.h"
struct sa_event_registry_services {
 void *context;
 void (*install)(void *,uint32_t,uint32_t,uint32_t,uint32_t *);
};
SA_STAGE_EXPORT uint32_t sa_event_register_8c64594c(uint8_t *,uint32_t *,uint32_t,
 uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t,
 const struct sa_event_registry_services *);
SA_STAGE_EXPORT uint32_t sa_event_remove_8c645c54(uint8_t *,uint32_t *,uint32_t,
 uint32_t *,uint32_t *,uint32_t,const struct sa_event_registry_services *);
SA_STAGE_EXPORT uint32_t sa_event_remove_connected(uint8_t *,uint32_t *,uint32_t,
 uint32_t *,uint32_t *,uint32_t,const struct sa_event_registry_services *);
/* Adapters connect both recovered special driver services. */
SA_STAGE_EXPORT uint32_t sa_event_register_connected(uint8_t *,uint32_t *,uint32_t,
 uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t,
 const struct sa_event_registry_services *);
#endif
