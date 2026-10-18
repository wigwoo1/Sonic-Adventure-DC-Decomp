#include "fpu_cleanup_transfer.h"
typedef int (*sa_transform_service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
typedef void (*sa_transform_observer)(void *,uint32_t,const uint8_t *,const struct sa_fpu_banks *,const uint32_t *,const uint32_t *,const uint32_t *);
#define SERVICE_DECL(name) int name(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_transform_service)
#define OBSERVER_DECL(name) int name(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_transform_observer)
SERVICE_DECL(sa_main_dispatch_transform_8c078eb0);
SERVICE_DECL(sa_main_dispatch_detach_8c0b1d60);
SERVICE_DECL(sa_main_dispatch_vector_store_8c07cb54);
SERVICE_DECL(sa_main_dispatch_shared_check_8c06c880);
SERVICE_DECL(sa_main_dispatch_status_query_8c06cebe);
SERVICE_DECL(sa_main_dispatch_status_transition_8c0afc98);
SERVICE_DECL(sa_main_dispatch_flag_transition_8c0b11c0);
OBSERVER_DECL(sa_main_dispatch_reset_8c05e788);
OBSERVER_DECL(sa_main_dispatch_active_query_8c05e6e8);
int sa_model_matrix_stack_pop_8c639b98(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
struct transform_context {void *user;sa_transform_service external;sa_transform_observer observe;uint32_t *fpul,*macl;};
static int dispatch(void *context,uint32_t target,uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct transform_context *s=(struct transform_context *)context;
 if(s->observe)s->observe(s->user,target,ram,fpu,sr,r,pr);
 if(target==0x8c05e788u)return sa_main_dispatch_reset_8c05e788(ram,fpu,sr,r,pr,s->fpul,s->macl,s->user,s->observe);
 if(target==0x8c05e6e8u)return sa_main_dispatch_active_query_8c05e6e8(ram,fpu,sr,r,pr,s->fpul,s->macl,s->user,s->observe);
 if(target==0x8c06cebeu)return sa_main_dispatch_status_query_8c06cebe(ram,fpu,sr,r,pr,s->fpul,s->macl,context,dispatch);
 if(target==0x8c06c880u)return sa_main_dispatch_shared_check_8c06c880(ram,fpu,sr,r,pr,s->fpul,s->macl,context,dispatch);
 if(target==0x8c0b1d60u)return sa_main_dispatch_detach_8c0b1d60(ram,fpu,sr,r,pr,s->fpul,s->macl,context,dispatch);
 if(target==0x8c07cb54u)return sa_main_dispatch_vector_store_8c07cb54(ram,fpu,sr,r,pr,s->fpul,s->macl,context,dispatch);
 if(target==0x8c078eb0u)return sa_main_dispatch_transform_8c078eb0(ram,fpu,sr,r,pr,s->fpul,s->macl,context,dispatch);
 if(target==0x8c639b98u)return sa_model_matrix_stack_pop_8c639b98(ram,fpu,sr,r);
 return s->external?s->external(s->user,target,ram,fpu,sr,r,pr):-2;
}
/* Host integration. Matrix push/three rotations/vector calculation and sound
 * remain services. The inherited native pop supports PR=0/SZ=0; its -1 guard
 * propagates exactly at the call boundary without unwinding guest frames.
 */
#define CONNECT(name,callee) \
SA_STAGE_EXPORT int name(uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *user,sa_transform_service external,sa_transform_observer observe){ \
 struct transform_context s={user,external,observe,fpul,macl}; \
 return callee(ram,fpu,sr,r,pr,fpul,macl,&s,dispatch); \
}
CONNECT(sa_main_dispatch_transform_connected,sa_main_dispatch_transform_8c078eb0)
CONNECT(sa_main_dispatch_detach_transform_connected,sa_main_dispatch_detach_8c0b1d60)
CONNECT(sa_main_dispatch_status_transform_connected,sa_main_dispatch_status_transition_8c0afc98)
CONNECT(sa_main_dispatch_flag_transform_connected,sa_main_dispatch_flag_transition_8c0b11c0)
