#ifndef SA_FRAME_RUNTIME_H
#define SA_FRAME_RUNTIME_H
#include "frame_countdown.h"
/* Native adapter; not an additional recovered Dreamcast routine. */
struct sa_frame_runtime {
 struct sa_frame_countdown *state;
 uint32_t *registration_handle;
 struct sa_countdown_registration_services registration;
 void *context;
 void (*configured_8c055c44)(void *);
 const uint32_t *mode;
 const uint32_t *dispatch_table;
 struct sa_frame_tick_services tick;
};
SA_STAGE_EXPORT void sa_frame_runtime_configure(struct sa_frame_runtime *,uint32_t,uint32_t);
SA_STAGE_EXPORT void sa_frame_runtime_deliver_tick(struct sa_frame_runtime *);
#endif
