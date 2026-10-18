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
DECL(sa_draw_distance_8c640128);
DECL(sa_draw_signed_sqrt_8c63a9c4);
DECL(sa_motion_setup_8c60e5e4);
DECL(sa_motion_dispatch_8c60e31c);
DECL(sa_motion_base_position_8c60cfc4);
DECL(sa_motion_base_rotation_8c60cfe2);
DECL(sa_motion_base_rotation_yxz_8c60cffc);
DECL(sa_motion_base_scale_8c60d016);
DECL(sa_motion_format_zero_8c60d034);

int sa_particle_render_state_save_8c605dac(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service);
int sa_particle_render_state_restore_8c605e0a(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service);
int sa_model_matrix_rotate(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,void*,axis_op);
int sa_matrix_rotate_yxz(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,void*,axis_op);
void sa_empty_stub_8c611708(struct sa_cpu_state*);
DECL(sa_motion_format_one_8c60d054);
DECL(sa_motion_format_two_8c60d094);
DECL(sa_motion_format_three_8c60d0d4);
DECL(sa_motion_format_four_8c60d140);
DECL(sa_motion_format_mesh_8c60d180);
DECL(sa_motion_format_six_seven_8c60d19e);
DECL(sa_motion_format_eight_8c60d1c8);
DECL(sa_motion_sample_position_8c60c322);
DECL(sa_motion_sample_scale_8c60c3b8);
DECL(sa_motion_sample_rotation_8c60c488);
DECL(sa_motion_sample_rotation_yxz_8c60c526);
DECL(sa_motion_linear_vector_8c60beb0);
DECL(sa_motion_linear_angles_8c60bf5e);
DECL(sa_motion_cubic_vector_8c60c026);
DECL(sa_motion_cubic_angles_8c60c19c);
int sa_unsigned_remainder_8c10cc80(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service);
int sa_engine_record_copy_dispatch(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service);
int sa_engine_record_copy8_tail(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service);

#include "vertex_bus.h"
DECL(sa_motion_mesh_prepare_8c617c34);
DECL(sa_motion_mesh_wrapper_8c621574);
typedef void (*lower_observer)(void*,uint32_t,const uint8_t*,const struct sa_fpu_banks*,const uint32_t*,const uint32_t*,const uint32_t*);
int sa_motion_mesh_lower_entry(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t,void*,lower_observer,const struct sa_vertex_bus*,uint32_t*);
struct context {uint32_t *published_stop;uint32_t *fpul,*macl,stop;void *user;observer observe;const struct sa_vertex_bus *bus;};
static void lower_observe(void *user,uint32_t target,const uint8_t *m,const struct sa_fpu_banks *f,const uint32_t *sr,const uint32_t *r,const uint32_t *pr){
 struct context *c=(struct context*)user;c->stop=target;
 if(c->observe)c->observe(c->user,target,(uint8_t*)m,(struct sa_fpu_banks*)f,(uint32_t*)sr,(uint32_t*)r,(uint32_t*)pr);
}
static int vector(void *user,uint32_t w,struct sa_fpu_banks *f){struct context *c=(struct context*)user;return sa_sh4_vector_numeric(w,f,*c->fpul);}
static int dispatch(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=(struct context*)opaque;c->stop=target;
 if(target==0x8c6215bau||target==0x8c6215d6u)return sa_motion_mesh_lower_entry(m,f,sr,r,pr,c->fpul,c->macl,target,c,lower_observe,c->bus,&c->stop);
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
 if(target==0x8c640128u)return sa_draw_distance_8c640128(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c63a9c4u)return sa_draw_signed_sqrt_8c63a9c4(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60e5e4u)return sa_motion_setup_8c60e5e4(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60e31cu)return sa_motion_dispatch_8c60e31c(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60cfc4u)return sa_motion_base_position_8c60cfc4(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60cfe2u)return sa_motion_base_rotation_8c60cfe2(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60cffcu)return sa_motion_base_rotation_yxz_8c60cffc(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60d016u)return sa_motion_base_scale_8c60d016(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60d034u)return sa_motion_format_zero_8c60d034(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c605dacu)return sa_particle_render_state_save_8c605dac(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c605e0au)return sa_particle_render_state_restore_8c605e0a(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c639cf4u)return sa_model_matrix_rotate(m,f,sr,r,c->fpul,c,axis);
 if(target==0x8c639ff8u)return sa_matrix_rotate_yxz(m,f,sr,r,c->fpul,c,axis);
 if(target==0x8c611708u){struct sa_cpu_state s;for(unsigned i=0;i<16;i++)s.r[i]=r[i];s.sr=*sr;s.pr=*pr;s.pc=target;sa_empty_stub_8c611708(&s);return 0;}
 if(target==0x8c60d054u)return sa_motion_format_one_8c60d054(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60d094u)return sa_motion_format_two_8c60d094(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60d0d4u)return sa_motion_format_three_8c60d0d4(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60d140u)return sa_motion_format_four_8c60d140(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60d180u)return sa_motion_format_mesh_8c60d180(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60d19eu)return sa_motion_format_six_seven_8c60d19e(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60d1c8u)return sa_motion_format_eight_8c60d1c8(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60c322u)return sa_motion_sample_position_8c60c322(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60c3b8u)return sa_motion_sample_scale_8c60c3b8(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60c488u)return sa_motion_sample_rotation_8c60c488(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60c526u)return sa_motion_sample_rotation_yxz_8c60c526(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60beb0u)return sa_motion_linear_vector_8c60beb0(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60bf5eu)return sa_motion_linear_angles_8c60bf5e(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60c026u)return sa_motion_cubic_vector_8c60c026(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c60c19cu)return sa_motion_cubic_angles_8c60c19c(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c10cc80u)return sa_unsigned_remainder_8c10cc80(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c10cd38u)return sa_engine_record_copy_dispatch(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c10cd8eu)return sa_engine_record_copy8_tail(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c617c34u)return sa_motion_mesh_prepare_8c617c34(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 if(target==0x8c621574u)return sa_motion_mesh_wrapper_8c621574(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);
 return -2;
}
SA_STAGE_EXPORT int sa_draw_motion_mesh_connected(uint8_t *m,struct cpu *s,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *macl,uint32_t entry,void *user,observer observe,const struct sa_vertex_bus *bus){
 struct context c={&c.stop,fpul,macl,entry,user,observe,bus};int status=dispatch(&c,entry,m,f,&s->sr,s->r,&s->pr);
 s->pc=status?c.stop:s->pr;return status;
}
