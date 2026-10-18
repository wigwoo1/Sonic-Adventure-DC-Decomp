#ifndef SA_OWNER_HEAP_RUNTIME_H
#define SA_OWNER_HEAP_RUNTIME_H
#include "owner_cleanup.h"
/* Only helper and detach callbacks are used; heap and entity finish are native. */
SA_STAGE_EXPORT uint32_t sa_owner_cleanup_with_heap(uint8_t *,uint32_t *,uint32_t,const struct sa_owner_cleanup_services *);
#endif
