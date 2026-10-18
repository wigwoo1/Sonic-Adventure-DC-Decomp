#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
typedef void (*observer)(void *,uint32_t,const uint8_t *,const struct sa_fpu_banks *,const uint32_t *,const uint32_t *,const uint32_t *);
#define DECL(name) int name(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service)
DECL(sa_main_dispatch_notification_8c109fde);
DECL(sa_notification_create_8c109e50);
DECL(sa_notification_candidate_ready_8c64bed8);
DECL(sa_input_channel_info_pointer);
DECL(sa_signed_division);
DECL(sa_task_create);
DECL(sa_task_create_base);
DECL(sa_heap_allocate);
DECL(sa_heap_allocate_zeroed);
DECL(sa_task_delete);
DECL(sa_main_dispatch_spawn_8c07b320);
DECL(sa_main_dispatch_cancel_8c01ae60);
DECL(sa_main_dispatch_context_restore_8c01aae0);
DECL(sa_main_dispatch_callback_set_8c07b2b8);
DECL(sa_task_data_release);
DECL(sa_task_children_delete);
DECL(sa_task_children_update);
DECL(sa_heap_release);
DECL(sa_main_dispatch_prefix_gate_8c0afb10);
DECL(sa_sound_request_8c0940da);
DECL(sa_sound_slot_select_8c093f02);
struct cpu {uint32_t r[16],sr,pr,pc;};
typedef void (*tail_dispatch)(uint8_t *,struct cpu *,uint32_t,void *);
void sa_callback_tail_8c604370(uint8_t *,struct cpu *,tail_dispatch,void *);
void sa_input_device_record_index_8c65f3a0(uint8_t *,struct cpu *,uint32_t *);
struct context {void *user;service external;observer observe;uint32_t *fpul,*macl;};
static int dispatch(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
struct tail_context {struct context *outer;struct sa_fpu_banks *f;int status;};
static void tail_call(uint8_t *ram,struct cpu *cpu,uint32_t target,void *context){
 struct tail_context *s=(struct tail_context *)context;
 s->status=dispatch(s->outer,target,ram,s->f,&cpu->sr,cpu->r,&cpu->pr);
}
static int dispatch(void *context,uint32_t target,uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *s=(struct context *)context;
 if(s->observe)s->observe(s->user,target,ram,f,sr,r,pr);
#define ROUTE(address,name) if(target==address)return name(ram,f,sr,r,pr,s->fpul,s->macl,context,dispatch)
 ROUTE(0x8c109fdeu,sa_main_dispatch_notification_8c109fde);
 ROUTE(0x8c109e50u,sa_notification_create_8c109e50);
 ROUTE(0x8c64bed8u,sa_notification_candidate_ready_8c64bed8);
 ROUTE(0x8c646868u,sa_input_channel_info_pointer);
 ROUTE(0x8c10ca5cu,sa_signed_division);
 ROUTE(0x8c09852eu,sa_task_create);
 ROUTE(0x8c0985ecu,sa_task_create_base);
 ROUTE(0x8c098a80u,sa_heap_allocate);
 ROUTE(0x8c098b0cu,sa_heap_allocate_zeroed);
 ROUTE(0x8c09865cu,sa_task_delete);
 ROUTE(0x8c07b320u,sa_main_dispatch_spawn_8c07b320);
 ROUTE(0x8c01ae60u,sa_main_dispatch_cancel_8c01ae60);
 ROUTE(0x8c01aae0u,sa_main_dispatch_context_restore_8c01aae0);
 ROUTE(0x8c07b2b8u,sa_main_dispatch_callback_set_8c07b2b8);
 ROUTE(0x8c098998u,sa_task_data_release);
 ROUTE(0x8c098e60u,sa_task_children_delete);
 ROUTE(0x8c098e98u,sa_task_children_update);
 ROUTE(0x8c098b42u,sa_heap_release);
 ROUTE(0x8c0940dau,sa_sound_request_8c0940da);
 ROUTE(0x8c093f02u,sa_sound_slot_select_8c093f02);
#undef ROUTE
 if(target==0x8c604370u||target==0x8c65f3a0u){
  struct cpu cpu;for(unsigned i=0;i<16;i++)cpu.r[i]=r[i];cpu.sr=*sr;cpu.pr=*pr;cpu.pc=target;
  int status=0;
  if(target==0x8c604370u){struct tail_context tail={s,f,0};sa_callback_tail_8c604370(ram,&cpu,tail_call,&tail);status=tail.status;}
  else sa_input_device_record_index_8c65f3a0(ram,&cpu,s->macl);
  for(unsigned i=0;i<16;i++)r[i]=cpu.r[i];*sr=cpu.sr;*pr=cpu.pr;return status;
 }
 return s->external?s->external(s->user,target,ram,f,sr,r,pr):-2;
}
#define CONNECT(name,callee) \
SA_STAGE_EXPORT int name(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *user,service external,observer observe){ \
 struct context s={user,external,observe,fpul,macl}; \
 return callee(ram,f,sr,r,pr,fpul,macl,&s,dispatch); \
}
CONNECT(sa_notification_connected,sa_main_dispatch_notification_8c109fde)
CONNECT(sa_notification_create_connected,sa_notification_create_8c109e50)
CONNECT(sa_notification_ready_connected,sa_notification_candidate_ready_8c64bed8)
CONNECT(sa_main_dispatch_notified_prefix_gate_connected,sa_main_dispatch_prefix_gate_8c0afb10)

CONNECT(sa_task_disposal_connected,sa_task_delete)
CONNECT(sa_task_work_release_connected,sa_task_data_release)
CONNECT(sa_task_children_delete_connected,sa_task_children_delete)
CONNECT(sa_task_children_update_connected,sa_task_children_update)
CONNECT(sa_task_heap_release_connected,sa_heap_release)

CONNECT(sa_main_dispatch_spawn_connected,sa_main_dispatch_spawn_8c07b320)
CONNECT(sa_main_dispatch_callback_set_connected,sa_main_dispatch_callback_set_8c07b2b8)
CONNECT(sa_main_dispatch_cancellation_connected,sa_main_dispatch_cancel_8c01ae60)
CONNECT(sa_main_dispatch_context_restore_connected,sa_main_dispatch_context_restore_8c01aae0)
