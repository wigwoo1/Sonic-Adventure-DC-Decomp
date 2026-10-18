#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
int sa_tikal_preupdate(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
int sa_tikal_flags_transition(uint8_t *,uint32_t *,uint32_t *);
int sa_tikal_idle_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,void *,service);
struct context {void *user;service external;};
static int call(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=opaque;
 if(target==0x0cb00e74u)return sa_tikal_flags_transition(m,sr,r);
 if(target==0x0cb00e90u)return sa_tikal_idle_transition(m,f,sr,r,pr,c->user,c->external);
 return c->external(c->user,target,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_tikal_preupdate_predicates(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *user,service external){
 struct context c={user,external};return sa_tikal_preupdate(m,f,sr,r,pr,fpul,macl,&c,call);
}
