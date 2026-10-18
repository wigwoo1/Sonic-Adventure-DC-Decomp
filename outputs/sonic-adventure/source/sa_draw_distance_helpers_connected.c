#include "fpu_cleanup_transfer.h"
struct cpu {uint32_t r[16],sr,pr,pc;};
typedef int (*service)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
typedef void (*observer)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
#define DECL(name) int name(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*)
DECL(sa_draw_distance_8c640128);
DECL(sa_draw_signed_sqrt_8c63a9c4);
struct context {uint32_t *fpul,*macl,stop;void *user;observer observe;};
static int dispatch(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=(struct context*)opaque;c->stop=target;
 if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);
 if(target==0x8c640128u)return sa_draw_distance_8c640128(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c63a9c4u)return sa_draw_signed_sqrt_8c63a9c4(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 return -2;
}
SA_STAGE_EXPORT int sa_draw_distance_helpers_connected(uint8_t *m,struct cpu *s,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *macl,uint32_t entry,void *user,observer observe){
 struct context c={fpul,macl,entry,user,observe};int status=dispatch(&c,entry,m,f,&s->sr,s->r,&s->pr);
 s->pc=status?c.stop:s->pr;return status;
}
