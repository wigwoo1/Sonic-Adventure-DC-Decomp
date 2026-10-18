#include "fpu_cleanup_transfer.h"
typedef int (*sa_reset_service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
typedef void (*sa_reset_observer)(void *,uint32_t,const uint8_t *,const struct sa_fpu_banks *,const uint32_t *,const uint32_t *,const uint32_t *);
int sa_main_dispatch_reset_8c05e788(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_reset_observer);
int sa_main_dispatch_active_query_8c05e6e8(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_reset_observer);
int sa_main_dispatch_status_query_8c06cebe(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_reset_service);
int sa_main_dispatch_shared_check_8c06c880(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_reset_service);
int sa_main_dispatch_status_transition_8c0afc98(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_reset_service);
struct reset_context {void *user;sa_reset_service external;sa_reset_observer observe;uint32_t *fpul;uint32_t *macl;};
static int dispatch(void *context,uint32_t target,uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct reset_context *services=(struct reset_context *)context;
 if(services->observe)services->observe(services->user,target,ram,fpu,sr,r,pr);
 if(target==0x8c05e788u)return sa_main_dispatch_reset_8c05e788(ram,fpu,sr,r,pr,services->fpul,services->macl,services->user,services->observe);
 if(target==0x8c05e6e8u)return sa_main_dispatch_active_query_8c05e6e8(ram,fpu,sr,r,pr,services->fpul,services->macl,services->user,services->observe);
 if(target==0x8c06cebeu)return sa_main_dispatch_status_query_8c06cebe(ram,fpu,sr,r,pr,services->fpul,services->macl,context,dispatch);
 if(target==0x8c06c880u)return sa_main_dispatch_shared_check_8c06c880(ram,fpu,sr,r,pr,services->fpul,services->macl,context,dispatch);
 return services->external?services->external(services->user,target,ram,fpu,sr,r,pr):-2;
}
/* Host integration only. The reset, clear, query, predicate, active query
 * and shared check are native. Detach/vector/sound remain external services.
 * Null external service reaches -2 at its first remaining target, retaining
 * the exact partial guest state. Observation is optional test support.
 */
SA_STAGE_EXPORT int sa_main_dispatch_status_reset_connected(uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *user,sa_reset_service external,sa_reset_observer observe){
 struct reset_context services={user,external,observe,fpul,macl};
 return sa_main_dispatch_status_transition_8c0afc98(ram,fpu,sr,r,pr,fpul,macl,&services,dispatch);
}
