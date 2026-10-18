#include "fpu_cleanup_transfer.h"
#include "interrupt_io.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service external
int sa_task_delete(ARGS);
int sa_task_delete_list(ARGS);
int sa_task_delete_regular(ARGS);
int sa_task_delete_all(ARGS);
int sa_task_create(ARGS);
int sa_task_pool_initialize(ARGS);
int sa_heap_initialize(ARGS);
int sa_engine_setup(ARGS);
int sa_setup_counter_set(ARGS);
int sa_setup_callback_state(ARGS);
int sa_setup_callback_register(ARGS);
int sa_interrupt_handler_primary(ARGS);
int sa_interrupt_handler_secondary(ARGS);
int sa_interrupt_handler_set(ARGS);
int sa_interrupt_handler_reset(ARGS);
int sa_interrupt_dispatch_primary(ARGS);
int sa_interrupt_dispatch_secondary(ARGS);
int sa_interrupt_gameplay_callback(ARGS);
int sa_stage_act_key(ARGS);
int sa_interrupt_service_643f98(ARGS);
int sa_interrupt_input_events(ARGS);
int sa_interrupt_tail_dispatch(ARGS);
int sa_interrupt_job_query(ARGS);
int sa_interrupt_job_dispatch(ARGS);
int sa_input_press_primary(ARGS);
int sa_input_press_alternate(ARGS);
int sa_input_channel_state_get(ARGS);
int sa_input_channel_event_set(ARGS);
int sa_input_channel_request_set(ARGS);
int sa_input_channel_available(ARGS);
int sa_input_sample_mask(ARGS);
int sa_input_channel_query(ARGS);
int sa_input_channel_status_pointer(ARGS);
int sa_input_channel_info_pointer(ARGS);
int sa_input_device_info_pointer(ARGS);
int sa_input_channel_callback(ARGS);
int sa_input_channel_secondary_callback(ARGS);
int sa_input_frame_process(ARGS);
int sa_input_frame_backend(ARGS);
int sa_input_frame_manager(ARGS);
int sa_input_frame_backend_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
int sa_input_frame_context_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
int sa_input_work_dispatch(ARGS);
int sa_position_task_create(ARGS);
int sa_task_create_base(ARGS);
int sa_heap_allocate_zeroed(ARGS);
int sa_heap_allocate(ARGS);
int sa_task_update_list(ARGS);
int sa_task_update_all(ARGS);
int sa_position_task_callback(ARGS);
int sa_task_remove_request(ARGS);
int sa_task_children_delete(ARGS);
int sa_task_children_update(ARGS);
int sa_task_data_release(ARGS);
int sa_heap_release(ARGS);
int sa_task_optional_cleanup(ARGS);
int sa_task_cleanup_list(ARGS);
int sa_task_cleanup_secondary(ARGS);
int sa_task_payload_cleanup(ARGS);
int sa_interrupt_callback_create(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,struct sa_interrupt_io *,void *,service);
int sa_interrupt_callback_remove(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,struct sa_interrupt_io *,void *,service);
struct context {uint32_t *fpul,*macl;void *user;service external;struct sa_interrupt_io *io;uint32_t *mach,*gbr;unsigned backend;};
static int dispatch(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=opaque;
 if(target==0x8c64594cu && c->io)return sa_interrupt_callback_create(m,f,sr,r,pr,c->fpul,c->macl,c->io,c,dispatch);
 if(target==0x8c645c54u && c->io)return sa_interrupt_callback_remove(m,f,sr,r,pr,c->fpul,c->macl,c->io,c,dispatch);
 if(c->io){
  if(target==0x8c647908u)return sa_input_frame_process(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c66085au)return sa_input_frame_backend(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c668e60u&&c->mach&&c->backend)return sa_input_frame_backend_native(m,f,sr,r,pr,c->fpul,c->macl,c->mach,c->gbr,c,dispatch);
  if(target==0x8c668e60u&&c->mach)return sa_input_frame_context_native(m,f,sr,r,pr,c->fpul,c->macl,c->mach,c->gbr,c,dispatch);
  if(target==0x8c668e60u)return sa_input_frame_manager(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c668ccau)return sa_input_work_dispatch(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c647bb8u)return sa_input_channel_callback(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c647bdeu)return sa_input_channel_secondary_callback(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c660dacu)return sa_input_channel_query(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c646856u)return sa_input_channel_status_pointer(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c646868u)return sa_input_channel_info_pointer(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c65f3a0u)return sa_input_device_info_pointer(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c648234u)return sa_input_sample_mask(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c6033e4u)return sa_input_channel_available(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c647ca4u)return sa_input_channel_state_get(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c647cb4u)return sa_input_channel_event_set(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c647cc4u)return sa_input_channel_request_set(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c602df8u)return sa_input_press_primary(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c602e34u)return sa_input_press_alternate(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c60178cu)return sa_interrupt_job_query(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c6017a4u)return sa_interrupt_job_dispatch(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c09ce40u)return sa_interrupt_tail_dispatch(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c602cb8u)return sa_interrupt_input_events(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c643f98u)return sa_interrupt_service_643f98(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c04f7e0u)return sa_stage_act_key(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c051810u)return sa_interrupt_gameplay_callback(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c645b1eu)return sa_interrupt_dispatch_primary(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c645b68u)return sa_interrupt_dispatch_secondary(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c64135au)return sa_interrupt_handler_primary(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c64137cu)return sa_interrupt_handler_secondary(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c6412c2u)return sa_interrupt_handler_set(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  if(target==0x8c6411b0u)return sa_interrupt_handler_reset(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  /* Both retail leaves are RTS with MOV #1,R0 in the delay slot. */
  if(target==0x8c658c20u || target==0x8c658c24u){r[0]=1;return 0;}
 }
 if(target==0x8c09865cu)return sa_task_delete(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c098e60u)return sa_task_children_delete(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c098e98u)return sa_task_children_update(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c098998u)return sa_task_data_release(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c098b42u)return sa_heap_release(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c04b468u)return sa_task_optional_cleanup(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c04b48cu)return sa_task_cleanup_list(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c04b82eu)return sa_task_cleanup_secondary(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c0344a0u)return sa_task_payload_cleanup(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c07d560u)return sa_position_task_callback(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c098786u)return sa_task_remove_request(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c0985ecu)return sa_task_create_base(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c098b0cu)return sa_heap_allocate_zeroed(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c098a80u)return sa_heap_allocate(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c09852eu)return sa_task_create(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c09878cu)return sa_task_update_list(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c098876u)return sa_task_delete_list(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c098a10u)return sa_heap_initialize(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c09844au)return sa_task_pool_initialize(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c09b4aau)return sa_setup_counter_set(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c051760u)return sa_setup_callback_state(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c605234u)return sa_setup_callback_register(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 return c->external(c->user,target,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_task_delete_native(ARGS){
 struct context c={fpul,macl,ctx,external,0,0,0,0};return sa_task_delete(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_task_update_list_native(ARGS){
 struct context c={fpul,macl,ctx,external,0,0,0,0};return sa_task_update_list(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_task_create_native(ARGS){
 struct context c={fpul,macl,ctx,external,0,0,0,0};return sa_task_create(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_position_task_create_native(ARGS){
 struct context c={fpul,macl,ctx,external,0,0,0,0};return sa_position_task_create(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_task_pool_initialize_native(ARGS){
 struct context c={fpul,macl,ctx,external,0,0,0,0};return sa_task_pool_initialize(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_task_children_delete_native(ARGS){
 struct context c={fpul,macl,ctx,external,0,0,0,0};return sa_task_children_delete(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_task_update_all_native(ARGS){
 struct context c={fpul,macl,ctx,external,0,0,0,0};return sa_task_update_all(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_task_delete_list_native(ARGS){
 struct context c={fpul,macl,ctx,external,0,0,0,0};return sa_task_delete_list(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_task_delete_regular_native(ARGS){
 struct context c={fpul,macl,ctx,external,0,0,0,0};return sa_task_delete_regular(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_task_delete_all_native(ARGS){
 struct context c={fpul,macl,ctx,external,0,0,0,0};return sa_task_delete_all(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_engine_setup_native(ARGS){
 struct context c={fpul,macl,ctx,external,0,0,0,0};return sa_engine_setup(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_engine_setup_interrupt_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,struct sa_interrupt_io *io,void *ctx,service external){
 if(!io)return -3;
 struct context c={fpul,macl,ctx,external,io,0,0,0};return sa_engine_setup(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_interrupt_handler_primary_native(ARGS){
 struct sa_interrupt_io io={0};struct context c={fpul,macl,ctx,external,&io,0,0,0};return sa_interrupt_handler_primary(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_interrupt_handler_secondary_native(ARGS){
 struct sa_interrupt_io io={0};struct context c={fpul,macl,ctx,external,&io,0,0,0};return sa_interrupt_handler_secondary(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_interrupt_handler_set_native(ARGS){
 struct sa_interrupt_io io={0};struct context c={fpul,macl,ctx,external,&io,0,0,0};return sa_interrupt_handler_set(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_interrupt_handler_reset_native(ARGS){
 struct sa_interrupt_io io={0};struct context c={fpul,macl,ctx,external,&io,0,0,0};return sa_interrupt_handler_reset(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_interrupt_dispatch_primary_native(ARGS){
 struct sa_interrupt_io io={0};struct context c={fpul,macl,ctx,external,&io,0,0,0};return sa_interrupt_dispatch_primary(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_interrupt_dispatch_secondary_native(ARGS){
 struct sa_interrupt_io io={0};struct context c={fpul,macl,ctx,external,&io,0,0,0};return sa_interrupt_dispatch_secondary(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_interrupt_gameplay_callback_native(ARGS){
 struct sa_interrupt_io io={0};struct context c={fpul,macl,ctx,external,&io,0,0,0};return sa_interrupt_gameplay_callback(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_interrupt_input_events_native(ARGS){
 struct sa_interrupt_io io={0};struct context c={fpul,macl,ctx,external,&io,0,0,0};return sa_interrupt_input_events(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_input_channel_available_native(ARGS){
 struct sa_interrupt_io io={0};struct context c={fpul,macl,ctx,external,&io,0,0,0};return sa_input_channel_available(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_input_channel_query_native(ARGS){
 struct sa_interrupt_io io={0};struct context c={fpul,macl,ctx,external,&io,0,0,0};return sa_input_channel_query(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_interrupt_input_events_extended_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service external){struct sa_interrupt_io io={0};struct context c={fpul,macl,ctx,external,&io,mach,gbr,0};return sa_interrupt_input_events(m,f,sr,r,pr,fpul,macl,&c,dispatch);}

SA_STAGE_EXPORT int sa_interrupt_input_events_backend_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service external){struct sa_interrupt_io io={0};struct context c={fpul,macl,ctx,external,&io,mach,gbr,1};return sa_interrupt_input_events(m,f,sr,r,pr,fpul,macl,&c,dispatch);}
