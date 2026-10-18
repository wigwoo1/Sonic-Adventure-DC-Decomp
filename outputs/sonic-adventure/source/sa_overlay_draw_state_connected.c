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

#include "sh4_vector_numeric.h"
struct sa_cpu_state {uint32_t r[16],sr,pr,pc;};
typedef void (*cpu_service)(uint8_t*,struct sa_cpu_state*);
void sa_global_word_clear_mask_8c056990(uint8_t*,struct sa_cpu_state*);
void sa_global_word_set_mask_8c056986(uint8_t*,struct sa_cpu_state*);
void sa_copy_global_pair_8c056926(uint8_t*,struct sa_cpu_state*);
void sa_restore_global_pair_8c056938(uint8_t*,struct sa_cpu_state*);
void sa_global_pair_flags_or_8c05694a(uint8_t*,struct sa_cpu_state*);
void sa_store_nonzero_third_global_pointer_8c608ccc(uint8_t*,struct sa_cpu_state*);
int sa_model_matrix_stack_push_8c639c70(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*);
int sa_model_matrix_stack_pop_8c639b98(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*);
typedef int (*vector_op)(void*,uint32_t,struct sa_fpu_banks*);
typedef int (*axis_op)(void*,uint32_t,struct sa_fpu_banks*,uint32_t);
int sa_model_matrix_translate(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,void*,vector_op);
int sa_matrix_rotate_x(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,void*,axis_op);
int sa_matrix_rotate_y(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,void*,axis_op);
int sa_matrix_rotate_z(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,void*,axis_op);
DECL(sa_draw_matrix_scale_registers_8c63a69c);
DECL(sa_draw_matrix_translate_registers_8c63a878);
static int axis(void *user,uint32_t w,struct sa_fpu_banks *f,uint32_t fpul){(void)user;return sa_sh4_vector_numeric(w,f,fpul);}
static int cpu_call(cpu_service call,uint8_t *m,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t target){
 struct sa_cpu_state s;for(unsigned i=0;i<16;i++)s.r[i]=r[i];s.sr=*sr;s.pr=*pr;s.pc=target;
 call(m,&s);for(unsigned i=0;i<16;i++)r[i]=s.r[i];*sr=s.sr;*pr=s.pr;return 0;
}
struct context {uint32_t *fpul,*macl,stop;void *user;observer observe;};
static int vector(void *user,uint32_t w,struct sa_fpu_banks *f){struct context *c=(struct context*)user;return sa_sh4_vector_numeric(w,f,*c->fpul);}
static int dispatch(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=(struct context*)opaque;c->stop=target;
 if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);
 if(target==0x0c9058a0u)return sa_overlay_object_draw_candidate_0c9058a0(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x0c905618u)return sa_overlay_scale_spring_0c905618(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c038d00u)return sa_draw_predicate_8c038d00(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c043a20u)return sa_draw_float_state_wrapper_8c043a20(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60edf0u)return sa_draw_float_state_copy_8c60edf0(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c056990u)return cpu_call(sa_global_word_clear_mask_8c056990,m,sr,r,pr,target);
 if(target==0x8c056986u)return cpu_call(sa_global_word_set_mask_8c056986,m,sr,r,pr,target);
 if(target==0x8c056926u)return cpu_call(sa_copy_global_pair_8c056926,m,sr,r,pr,target);
 if(target==0x8c056938u)return cpu_call(sa_restore_global_pair_8c056938,m,sr,r,pr,target);
 if(target==0x8c05694au)return cpu_call(sa_global_pair_flags_or_8c05694a,m,sr,r,pr,target);
 if(target==0x8c608cccu)return cpu_call(sa_store_nonzero_third_global_pointer_8c608ccc,m,sr,r,pr,target);
 if(target==0x8c639c70u)return sa_model_matrix_stack_push_8c639c70(m,f,sr,r);
 if(target==0x8c639b98u)return sa_model_matrix_stack_pop_8c639b98(m,f,sr,r);
 if(target==0x8c63a804u)return sa_model_matrix_translate(m,f,sr,r,c,vector);
 if(target==0x8c639ec8u)return sa_matrix_rotate_x(m,f,sr,r,c->fpul,c,axis);
 if(target==0x8c639f5cu)return sa_matrix_rotate_y(m,f,sr,r,c->fpul,c,axis);
 if(target==0x8c63a1ccu)return sa_matrix_rotate_z(m,f,sr,r,c->fpul,c,axis);
 if(target==0x8c63a69cu)return sa_draw_matrix_scale_registers_8c63a69c(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c63a878u)return sa_draw_matrix_translate_registers_8c63a878(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 return -2;
}
SA_STAGE_EXPORT int sa_overlay_draw_state_connected(uint8_t *m,struct cpu *s,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *macl,uint32_t entry,void *user,observer observe){
 struct context c={fpul,macl,entry,user,observe};int status=dispatch(&c,entry,m,f,&s->sr,s->r,&s->pr);
 s->pc=status?c.stop:s->pr;return status;
}
