#ifndef SA_CONSOLE_SYSINFO_STATUS_H
#define SA_CONSOLE_SYSINFO_STATUS_H
#include "native_console_service.h"
/* Correct SYSINFO_INIT's guest R0 status without rewriting the older service,
 * its state layout, or its historical verification artifacts. */
SA_CONSOLE_EXPORT int sa_console_status_service(struct sa_console_service *,uint8_t *,
 uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *,uint32_t *);
#endif
