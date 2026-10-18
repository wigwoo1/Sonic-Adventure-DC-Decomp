#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
typedef void (*observer)(void *,uint32_t,const uint8_t *,const struct sa_fpu_banks *,const uint32_t *,const uint32_t *,const uint32_t *);
#define DECL(name) int name(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service)
DECL(sa_main_dispatch_prefix_gate_8c0afb10);
DECL(sa_sound_request_8c0940da);
DECL(sa_sound_slot_select_8c093f02);
struct context {void *user;service external;observer observe;uint32_t *fpul,*macl;};
static int dispatch(void *context,uint32_t target,uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *s=(struct context *)context;
 if(s->observe)s->observe(s->user,target,ram,f,sr,r,pr);
 if(target==0x8c0940dau)return sa_sound_request_8c0940da(ram,f,sr,r,pr,s->fpul,s->macl,context,dispatch);
 if(target==0x8c093f02u)return sa_sound_slot_select_8c093f02(ram,f,sr,r,pr,s->fpul,s->macl,context,dispatch);
 return s->external?s->external(s->user,target,ram,f,sr,r,pr):-2;
}
/* Sound is native. Other original services remain explicit and observed. */
SA_STAGE_EXPORT int sa_main_dispatch_prefix_gate_connected(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *user,service external,observer observe){
 struct context s={user,external,observe,fpul,macl};
 return sa_main_dispatch_prefix_gate_8c0afb10(ram,f,sr,r,pr,fpul,macl,&s,dispatch);
}
