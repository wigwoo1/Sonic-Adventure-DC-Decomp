#ifndef SA_FRAME_COUNTDOWN_H
#define SA_FRAME_COUNTDOWN_H
#include "stage00_task.h"
struct sa_frame_countdown {
 uint32_t initial_ticks,current_ticks,iteration_limit,iteration_index;
};
struct sa_frame_countdown_services {
 void *context;
 void (*register_8c605234)(void *,uintptr_t);
 void (*finish_8c055c44)(void *);
};
SA_STAGE_EXPORT void sa_frame_countdown_configure_8c051760(
 struct sa_frame_countdown *,uint32_t,uint32_t,const struct sa_frame_countdown_services *);
struct sa_frame_tick_services {
 void *context;
 uint32_t (*query_8c04f7e0)(void *);
 void (*dispatch)(void *,uint32_t);
 void (*zero_8c643f98)(void *);
 void (*every_8c602cb8)(void *);
 void (*finish_8c09ce40)(void *);
};
SA_STAGE_EXPORT void sa_frame_countdown_tick_8c051810(
 struct sa_frame_countdown *,const uint32_t *,const uint32_t *,
 const struct sa_frame_tick_services *);
struct sa_countdown_registration_services {
 void *context;
 void (*remove_8c645c54)(void *,uint32_t);
 uint32_t (*register_8c64594c)(void *,uint32_t,uint32_t,uint32_t,uint32_t);
};
SA_STAGE_EXPORT void sa_countdown_register_8c605234(uint32_t *,uint32_t,
 const struct sa_countdown_registration_services *);
#endif
