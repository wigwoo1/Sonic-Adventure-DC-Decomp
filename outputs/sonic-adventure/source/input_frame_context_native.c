#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service call
int sa_input_frame_manager(ARGS);
int sa_input_frame_manager_resume(ARGS);
int sa_input_work_dispatch(ARGS);
int sa_input_work_yield(ARGS);
int sa_input_work_yield_resume(ARGS);
int sa_input_work_create(ARGS);
int sa_input_work_object_create(ARGS);
int sa_input_work_pool_initialize(ARGS);
int sa_input_work_backend_initialize(ARGS);
int sa_input_work_backend_post(ARGS);
int sa_input_channel_worker_entry(ARGS);
int sa_input_channel_worker_resume(ARGS);
int sa_input_channels_initialize(ARGS);
int sa_input_channel_status_pointer(ARGS);
int sa_input_channel_info_pointer(ARGS);
int sa_input_device_info_pointer(ARGS);
int sa_input_device_backend_check(ARGS);
int sa_input_device_record_pointer(ARGS);
int sa_input_device_busy_read(ARGS);
int sa_input_device_request_mark(ARGS);
int sa_input_device_busy(ARGS);
int sa_input_device_mark(ARGS);
int sa_input_request_completion(ARGS);
int sa_input_request_completion_impl(ARGS);
int sa_input_device_available(ARGS);
int sa_input_backend_finalize(ARGS);
int sa_input_backend_finalize_resume(ARGS);
int sa_input_packet_row_copy(ARGS);
int sa_input_backend_request(ARGS);
int sa_input_backend_request_resume(ARGS);
int sa_input_device_request_resume(ARGS);
int sa_input_press_service_resume(ARGS);
int sa_input_device_request(ARGS);
int sa_input_press_service(ARGS);
int sa_input_event_service(ARGS);
int sa_input_event_error(ARGS);
int sa_input_channel_callback(ARGS);
int sa_input_channel_secondary_callback(ARGS);
int sa_memory_move(ARGS);
int sa_memory_fill(ARGS);
int sa_input_context_save(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
int sa_input_context_restore(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
struct frame_context {uint32_t *fpul,*macl,*mach,*gbr;void *user;service external;unsigned backend;};
int sa_input_request_allocate_queued_native(ARGS);int sa_input_request_allocate_simple_queued_native(ARGS);
int sa_input_device_metadata_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);int sa_signed_division(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
int sa_input_packet_read_be(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);int sa_input_packet_read_le(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
static int dispatch(void *ctx,uint32_t t,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct frame_context *c=ctx;
 if(c->backend==3&&t==0x8c66080cu)return sa_input_packet_read_be(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(c->backend==3&&t==0x8c660816u)return sa_input_packet_read_le(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(c->backend==3&&t==0x8c646bbcu)return sa_input_device_metadata_native(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(c->backend==3&&t==0x8c10ca5cu)return sa_signed_division(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(c->backend==3&&t==0x8c6469c8u)return sa_input_request_allocate_queued_native(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(c->backend==3&&t==0x8c646982u)return sa_input_request_allocate_simple_queued_native(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if((c->backend==1||c->backend==3)&&t==0x8c646884u)return sa_input_request_completion(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if((c->backend==1||c->backend==3)&&t==0x8c65f3f4u)return sa_input_request_completion_impl(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if((c->backend==1||c->backend==3)&&t==0x8c646b36u)return sa_input_device_busy(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if((c->backend==1||c->backend==3)&&t==0x8c646b3cu)return sa_input_device_mark(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if((c->backend==1||c->backend==3)&&t==0x8c65f390u)return sa_input_device_record_pointer(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if((c->backend==1||c->backend==3)&&t==0x8c65f40eu)return sa_input_device_busy_read(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if((c->backend==1||c->backend==3)&&t==0x8c65f41eu)return sa_input_device_request_mark(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);

 if((c->backend==1||c->backend==3)&&t==0x8c660decu)return sa_input_device_available(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if((c->backend==1||c->backend==3)&&t==0x8c660a7eu)return sa_input_backend_finalize(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c6607fcu)return sa_input_packet_row_copy(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(c->backend&&t==0x8c64b4e4u)return sa_input_device_backend_check(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(c->backend&&t==0x8c646856u)return sa_input_channel_status_pointer(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(c->backend&&t==0x8c646868u)return sa_input_channel_info_pointer(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(c->backend&&t==0x8c65f3a0u)return sa_input_device_info_pointer(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if((c->backend==1||c->backend==3)&&t==0x8c6608a4u)return sa_input_backend_request(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c64753cu)return sa_input_device_request(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c64839au)return sa_input_press_service(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c6482a0u)return sa_input_event_service(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c64850au)return sa_input_event_error(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c647bb8u)return sa_input_channel_callback(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c647bdeu)return sa_input_channel_secondary_callback(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c66082cu)return sa_input_work_backend_initialize(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c660842u)return sa_input_work_backend_post(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c66086au){r[2]=0x8c668e5cu;r[4]=1;return sa_input_work_yield(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);}
 if(t==0x8c668b38u)return sa_input_work_pool_initialize(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c668b58u)return sa_input_work_object_create(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c668beeu)return sa_input_work_create(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c10d25cu)return sa_memory_move(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c10d31cu)return sa_memory_fill(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c110954u)return sa_input_context_save(m,f,r,*pr,*c->fpul,*c->mach,*c->macl,*c->gbr);
 if(t==0x8c668ccau)return sa_input_work_dispatch(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c1108e0u){
  uint32_t special[3]={*c->mach,*c->macl,*c->gbr};
  int status=sa_input_context_restore(m,f,r,sr,pr,c->fpul,special);if(status)return status;
  *c->mach=special[0];*c->macl=special[1];*c->gbr=special[2];
  return 1; /* Control transfer: *pr is the next native continuation. */
 }
 return c->external?c->external(c->user,t,m,f,sr,r,pr):-3;
}
static int is_caller_return(uint32_t p){return p==0x8c660aaeu||p==0x8c660b18u||p==0x8c660b2eu||p==0x8c64756eu||p==0x8c6475bcu||p==0x8c6483f6u||p==0x8c648400u||p==0x8c648412u||p==0x8c648426u||p==0x8c6479fau||p==0x8c647a08u;}
static int finish_transfer(int status,struct frame_context *c,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 while(status==1||(status==0&&(is_caller_return(*pr)||*pr==0x8c647b74u||*pr==0x8c660906u||*pr==0x8c66091au||*pr==0x8c660932u||*pr==0x8c660a68u))){
  if(status==1&&(*pr==0x8c647980u||(*pr>=0x8c64790eu&&*pr<=0x8c647926u&&((*pr-0x8c64790eu)%4u)==0))){
   r[4]=*pr==0x8c647980u?7u:(*pr-0x8c64790eu)/4u;
   status=sa_input_channel_worker_entry(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  }
  else if(*pr==0x8c660906u||*pr==0x8c66091au||*pr==0x8c660932u||*pr==0x8c660a68u)status=sa_input_backend_request_resume(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  else if(*pr==0x8c660aaeu||*pr==0x8c660b18u||*pr==0x8c660b2eu)status=sa_input_backend_finalize_resume(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  else if(*pr==0x8c64756eu||*pr==0x8c6475bcu)status=sa_input_device_request_resume(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  else if(*pr==0x8c6483f6u||*pr==0x8c648400u||*pr==0x8c648412u||*pr==0x8c648426u)status=sa_input_press_service_resume(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  else if(*pr==0x8c6479fau||*pr==0x8c647a08u)status=sa_input_channel_worker_resume(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  else if(*pr==0x8c647b74u)status=sa_input_channel_worker_resume(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  else if(*pr==0x8c668e48u)status=sa_input_work_yield_resume(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  else if(*pr==0x8c668e92u||*pr==0x8c668edcu||*pr==0x8c668ef2u)status=sa_input_frame_manager_resume(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
  else break;
 }
 return status;
}
/* Extended-state bridge. Result 1 transfers to *pr with the restored CPU state.
 * The caller must dispatch that continuation; it must not treat 1 as completion. */
SA_STAGE_EXPORT int sa_input_frame_context_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,0};int status=sa_input_frame_manager(m,f,sr,r,pr,fpul,macl,&c,dispatch);
 return finish_transfer(status,&c,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_input_frame_resume_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,0};return sa_input_frame_manager_resume(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}
SA_STAGE_EXPORT int sa_input_work_yield_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,0};int status=sa_input_work_yield(m,f,sr,r,pr,fpul,macl,&c,dispatch);
 return finish_transfer(status,&c,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_input_context_restore_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,0};return finish_transfer(dispatch(&c,0x8c1108e0u,m,f,sr,r,pr),&c,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_input_work_create_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,0};return sa_input_work_create(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}
SA_STAGE_EXPORT int sa_input_work_object_create_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,0};return sa_input_work_object_create(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}
SA_STAGE_EXPORT int sa_input_work_pool_initialize_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,0};return sa_input_work_pool_initialize(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}
SA_STAGE_EXPORT int sa_input_work_backend_initialize_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,0};return sa_input_work_backend_initialize(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}
SA_STAGE_EXPORT int sa_input_work_backend_post_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,0};return sa_input_work_backend_post(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}
SA_STAGE_EXPORT int sa_input_channel_worker_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,0};return finish_transfer(sa_input_channel_worker_entry(m,f,sr,r,pr,fpul,macl,&c,dispatch),&c,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_input_channels_initialize_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,0};return sa_input_channels_initialize(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_input_backend_request_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){struct frame_context c={fpul,macl,mach,gbr,ctx,call,0};return finish_transfer(sa_input_backend_request(m,f,sr,r,pr,fpul,macl,&c,dispatch),&c,m,f,sr,r,pr);}

SA_STAGE_EXPORT int sa_input_channel_worker_backend_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,1};return finish_transfer(sa_input_channel_worker_entry(m,f,sr,r,pr,fpul,macl,&c,dispatch),&c,m,f,sr,r,pr);
}

SA_STAGE_EXPORT int sa_input_context_restore_backend_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,1};return finish_transfer(dispatch(&c,0x8c1108e0u,m,f,sr,r,pr),&c,m,f,sr,r,pr);
}

SA_STAGE_EXPORT int sa_input_frame_backend_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,1};int status=sa_input_frame_manager(m,f,sr,r,pr,fpul,macl,&c,dispatch);
 return finish_transfer(status,&c,m,f,sr,r,pr);
}

SA_STAGE_EXPORT int sa_input_press_validated_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){struct frame_context c={fpul,macl,mach,gbr,ctx,call,2};return finish_transfer(sa_input_press_service(m,f,sr,r,pr,fpul,macl,&c,dispatch),&c,m,f,sr,r,pr);}

SA_STAGE_EXPORT int sa_input_backend_finalize_context_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){struct frame_context c={fpul,macl,mach,gbr,ctx,call,0};return finish_transfer(sa_input_backend_finalize(m,f,sr,r,pr,fpul,macl,&c,dispatch),&c,m,f,sr,r,pr);}

SA_STAGE_EXPORT int sa_input_backend_finalize_device_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){struct frame_context c={fpul,macl,mach,gbr,ctx,call,1};return finish_transfer(sa_input_backend_finalize(m,f,sr,r,pr,fpul,macl,&c,dispatch),&c,m,f,sr,r,pr);}

SA_STAGE_EXPORT int sa_input_device_available_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){struct frame_context c={fpul,macl,mach,gbr,ctx,call,1};return sa_input_device_available(m,f,sr,r,pr,fpul,macl,&c,dispatch);}

SA_STAGE_EXPORT int sa_input_backend_finalize_queued_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){struct frame_context c={fpul,macl,mach,gbr,ctx,call,3};return finish_transfer(sa_input_backend_finalize(m,f,sr,r,pr,fpul,macl,&c,dispatch),&c,m,f,sr,r,pr);}

SA_STAGE_EXPORT int sa_input_context_restore_queued_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,3};return finish_transfer(dispatch(&c,0x8c1108e0u,m,f,sr,r,pr),&c,m,f,sr,r,pr);
}

SA_STAGE_EXPORT int sa_input_backend_request_queued_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){struct frame_context c={fpul,macl,mach,gbr,ctx,call,3};return finish_transfer(sa_input_backend_request(m,f,sr,r,pr,fpul,macl,&c,dispatch),&c,m,f,sr,r,pr);}

SA_STAGE_EXPORT int sa_input_frame_queued_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){
 struct frame_context c={fpul,macl,mach,gbr,ctx,call,3};return finish_transfer(sa_input_frame_manager(m,f,sr,r,pr,fpul,macl,&c,dispatch),&c,m,f,sr,r,pr);
}

SA_STAGE_EXPORT int sa_input_channel_worker_queued_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t *mach,uint32_t *gbr,void *ctx,service call){struct frame_context c={fpul,macl,mach,gbr,ctx,call,3};return finish_transfer(sa_input_channel_worker_entry(m,f,sr,r,pr,fpul,macl,&c,dispatch),&c,m,f,sr,r,pr);}
