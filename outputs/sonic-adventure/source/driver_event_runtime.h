#ifndef SA_DRIVER_EVENT_RUNTIME_H
#define SA_DRIVER_EVENT_RUNTIME_H
#include "driver_dispatch.h"
/* Native connection adapter; callback bodies remain supplied by the caller. */
SA_STAGE_EXPORT uint32_t sa_driver_event_dispatch_connected(uint8_t *,uint32_t *,
 uint32_t,void *,uint32_t (*)(void *,uint32_t,uint32_t,uint32_t *));
#endif
