#ifndef SA_STARTUP_PLATFORM_BUS_H
#define SA_STARTUP_PLATFORM_BUS_H
#include "transfer_context_bus.h"
#define SA_STARTUP_WORDS (0x1500u/4u)
#define SA_STARTUP_SYSTEM_WORDS 9u
#define SA_STARTUP_CACHE_TAGS 512u
struct sa_startup_platform_memory {
 struct sa_transfer_context_memory context;
 struct sa_vertex_bus context_bus;
 uint32_t startup_registers[SA_STARTUP_WORDS];
 uint32_t system_registers[SA_STARTUP_SYSTEM_WORDS];
 uint32_t cache_tags[SA_STARTUP_CACHE_TAGS];
};
SA_STAGE_EXPORT void sa_startup_platform_bus_init(struct sa_startup_platform_memory *,uint8_t *,struct sa_vertex_bus *,uint32_t,uint32_t);
#endif
