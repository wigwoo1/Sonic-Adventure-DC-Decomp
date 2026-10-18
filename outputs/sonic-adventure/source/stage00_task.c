/* Recovered STG00 task state machine at 0x0c900000..0x0c900061.
 * Query (main 0x8c04f7e0) and initialization (STG00 0x0c900d00) remain
 * service dependencies. The original retains its work pointer across callbacks.
 */
#include "stage00_task.h"
SA_STAGE_EXPORT void sa_stage00_destructor_0c900d72(struct sa_stage00_task *task) {
 (void)task; /* Original: rts; nop. */
}
SA_STAGE_EXPORT void sa_stage00_update_0c900000(struct sa_stage00_task *task,
 const struct sa_stage00_services *services) {
 struct sa_stage00_work *work=task->work;
 switch(work->state) {
 case 0:
  work->cached_variant=(uint8_t)services->query_variant(services->context);
  task->destructor=sa_stage00_destructor_0c900d72;
  work->state=1;
  break;
 case 1:
  services->initialize(services->context,task);
  work->state=2;
  break;
 case 2:
  if((uint8_t)services->query_variant(services->context)!=work->cached_variant)work->state=3;
  break;
 case 3:work->state=1;break;
 default:break;
 }
}
/* Connect the recovered main-binary getter to this overlay without a RAM emulator. */
struct stage00_adapter {
 const struct sa_level_variant_fields *fields;
 void *context;
 void (*initialize)(void *context,struct sa_stage00_task *task);
};
static uint32_t query_fields(void *context) {
 return sa_level_variant_8c04f7e0(((struct stage00_adapter *)context)->fields);
}
static void initialize_fields(void *context,struct sa_stage00_task *task) {
 struct stage00_adapter *adapter=context;
 adapter->initialize(adapter->context,task);
}
SA_STAGE_EXPORT void sa_stage00_update_with_fields(struct sa_stage00_task *task,
 const struct sa_level_variant_fields *fields,void *context,
 void (*initialize)(void *context,struct sa_stage00_task *task)) {
 struct stage00_adapter adapter={fields,context,initialize};
 struct sa_stage00_services services={&adapter,query_fields,initialize_fields};
 sa_stage00_update_0c900000(task,&services);
}
