#include "fpu_cleanup_transfer.h"
struct cpu {uint32_t r[16],sr,pr,pc;};
typedef int (*service)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
typedef void (*observer)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
#define DECL(name) int name(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*)
DECL(sa_overlay_object_draw_candidate_0c9058a0);
DECL(sa_overlay_scale_spring_0c905618);
DECL(sa_draw_predicate_8c038d00);
DECL(sa_draw_float_state_wrapper_8c043a20);
DECL(sa_draw_float_state_copy_8c60edf0);
struct context {uint32_t *fpul,*macl,stop;void *user;observer observe;};
static int dispatch(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=(struct context*)opaque;c->stop=target;
 if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);
 if(target==0x0c9058a0u)return sa_overlay_object_draw_candidate_0c9058a0(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x0c905618u)return sa_overlay_scale_spring_0c905618(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c038d00u)return sa_draw_predicate_8c038d00(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c043a20u)return sa_draw_float_state_wrapper_8c043a20(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60edf0u)return sa_draw_float_state_copy_8c60edf0(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 return -2;
}
SA_STAGE_EXPORT int sa_overlay_draw_gate_connected(uint8_t *m,struct cpu *s,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *macl,uint32_t entry,void *user,observer observe){
 struct context c={fpul,macl,entry,user,observe};int status=dispatch(&c,entry,m,f,&s->sr,s->r,&s->pr);
 s->pc=status?c.stop:s->pr;return status;
}
