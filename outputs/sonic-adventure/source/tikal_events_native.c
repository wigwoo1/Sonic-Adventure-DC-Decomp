#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
int tikal_c64_event(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
int tikal_cd2_event(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
struct context {void *user;service external;uint32_t *fpul,*macl;};
static int call(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=opaque;if(target==0x0cb00c64u)return tikal_c64_event(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 return c->external(c->user,target,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_tikal_cd2_connected(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *user,service external){
 struct context c={user,external,fpul,macl};return tikal_cd2_event(m,f,sr,r,pr,fpul,macl,&c,call);
}
