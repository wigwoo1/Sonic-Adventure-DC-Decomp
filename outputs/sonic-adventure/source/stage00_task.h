#ifndef SA_STAGE00_TASK_H
#define SA_STAGE00_TASK_H
#include <stdint.h>
#include "level_variant.h"
#ifdef _WIN32
#define SA_STAGE_EXPORT __declspec(dllexport)
#else
#define SA_STAGE_EXPORT
#endif
/* Native view of fields used by this routine, not the complete engine task ABI. */
struct sa_stage00_work { uint8_t state, reserved[5]; uint16_t cached_variant; };
struct sa_stage00_task {
 struct sa_stage00_work *work;
 void (*destructor)(struct sa_stage00_task *task);
};
struct sa_stage00_services {
 void *context;
 uint32_t (*query_variant)(void *context);
 void (*initialize)(void *context,struct sa_stage00_task *task);
};
SA_STAGE_EXPORT void sa_stage00_update_0c900000(struct sa_stage00_task *task,
 const struct sa_stage00_services *services);
SA_STAGE_EXPORT void sa_stage00_destructor_0c900d72(struct sa_stage00_task *task);
SA_STAGE_EXPORT void sa_stage00_update_with_fields(struct sa_stage00_task *task,
 const struct sa_level_variant_fields *fields,void *context,
 void (*initialize)(void *context,struct sa_stage00_task *task));
#endif
