#include "fpu_cleanup_transfer.h"
#include "sh4_vector_numeric.h"
typedef int (*sa_matrix_service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
typedef void (*sa_matrix_observer)(void *,uint32_t,const uint8_t *,const struct sa_fpu_banks *,const uint32_t *,const uint32_t *,const uint32_t *);
typedef int (*sa_axis_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
#define AXIS_DECL(name) int name(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,void *,sa_axis_instruction)
AXIS_DECL(sa_matrix_rotate_x);
AXIS_DECL(sa_matrix_rotate_y);
AXIS_DECL(sa_matrix_rotate_z);
typedef int (*sa_calculation_instruction)(void *,uint32_t,struct sa_fpu_banks *);
int sa_model_direction_transform_extended(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,void *,sa_calculation_instruction);
#define SERVICE_DECL(name) int name(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_matrix_service)
#define OBSERVER_DECL(name) int name(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_matrix_observer)
SERVICE_DECL(sa_main_dispatch_transform_8c078eb0);
SERVICE_DECL(sa_sound_request_8c0940da);
SERVICE_DECL(sa_sound_slot_select_8c093f02);
SERVICE_DECL(sa_main_dispatch_detach_8c0b1d60);
SERVICE_DECL(sa_main_dispatch_vector_store_8c07cb54);
SERVICE_DECL(sa_main_dispatch_shared_check_8c06c880);
SERVICE_DECL(sa_main_dispatch_status_query_8c06cebe);
SERVICE_DECL(sa_main_dispatch_status_transition_8c0afc98);
SERVICE_DECL(sa_main_dispatch_flag_transition_8c0b11c0);
OBSERVER_DECL(sa_main_dispatch_matrix_init_8c057b40);
OBSERVER_DECL(sa_main_dispatch_reset_8c05e788);
OBSERVER_DECL(sa_main_dispatch_active_query_8c05e6e8);
int sa_model_matrix_stack_pop_8c639b98(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
struct matrix_context {void *user;sa_matrix_observer observe;uint32_t *fpul,*macl;sa_axis_instruction instruction;};
static int instruction(void *context,uint32_t word,struct sa_fpu_banks *fpu,uint32_t fpul){
 (void)context;
 return sa_sh4_vector_numeric(word,fpu,fpul);
}
static int calculation_instruction(void *context,uint32_t word,struct sa_fpu_banks *fpu){
 struct matrix_context *s=(struct matrix_context *)context;
 return instruction(s,word,fpu,*s->fpul);
}
static int dispatch(void *context,uint32_t target,uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct matrix_context *s=(struct matrix_context *)context;
 if(s->observe)s->observe(s->user,target,ram,fpu,sr,r,pr);
 if(target==0x8c638f28u)return sa_model_direction_transform_extended(ram,fpu,sr,r,s,calculation_instruction);
 if(target==0x8c639ec8u)return sa_matrix_rotate_x(ram,fpu,sr,r,s->fpul,s,instruction);
 if(target==0x8c639f5cu)return sa_matrix_rotate_y(ram,fpu,sr,r,s->fpul,s,instruction);
 if(target==0x8c63a1ccu)return sa_matrix_rotate_z(ram,fpu,sr,r,s->fpul,s,instruction);
 if(target==0x8c057b40u)return sa_main_dispatch_matrix_init_8c057b40(ram,fpu,sr,r,pr,s->fpul,s->macl,s->user,s->observe);
 if(target==0x8c05e788u)return sa_main_dispatch_reset_8c05e788(ram,fpu,sr,r,pr,s->fpul,s->macl,s->user,s->observe);
 if(target==0x8c05e6e8u)return sa_main_dispatch_active_query_8c05e6e8(ram,fpu,sr,r,pr,s->fpul,s->macl,s->user,s->observe);
 if(target==0x8c06cebeu)return sa_main_dispatch_status_query_8c06cebe(ram,fpu,sr,r,pr,s->fpul,s->macl,context,dispatch);
 if(target==0x8c06c880u)return sa_main_dispatch_shared_check_8c06c880(ram,fpu,sr,r,pr,s->fpul,s->macl,context,dispatch);
 if(target==0x8c0b1d60u)return sa_main_dispatch_detach_8c0b1d60(ram,fpu,sr,r,pr,s->fpul,s->macl,context,dispatch);
 if(target==0x8c07cb54u)return sa_main_dispatch_vector_store_8c07cb54(ram,fpu,sr,r,pr,s->fpul,s->macl,context,dispatch);
 if(target==0x8c078eb0u)return sa_main_dispatch_transform_8c078eb0(ram,fpu,sr,r,pr,s->fpul,s->macl,context,dispatch);
 if(target==0x8c639b98u)return sa_model_matrix_stack_pop_8c639b98(ram,fpu,sr,r);
 if(target==0x8c0940dau)return sa_sound_request_8c0940da(ram,fpu,sr,r,pr,s->fpul,s->macl,context,dispatch);
 if(target==0x8c093f02u)return sa_sound_slot_select_8c093f02(ram,fpu,sr,r,pr,s->fpul,s->macl,context,dispatch);
 return -2;
}
/* Host integration. Transform callees and FSCA/FTRV/FIPR/FSRRA are native;
 * no injected numerical output, instruction service or external game service
 * is accepted. Sound request and slot selection are native. Existing calculation
 * code collapses instruction/scalar errors to -1, retaining partial guest state;
 * rotations retain their own propagated status. Entry PR=0/SZ=0 guard retained.
 * Calculation assumes the original normalization literal at 8c638f8c.
 */
#define CONNECT(name,callee) \
SA_STAGE_EXPORT int name(uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *user,sa_matrix_observer observe){ \
 struct matrix_context s={user,observe,fpul,macl,0}; \
 return callee(ram,fpu,sr,r,pr,fpul,macl,&s,dispatch); \
}
CONNECT(sa_main_dispatch_sound_transform_connected,sa_main_dispatch_transform_8c078eb0)
CONNECT(sa_main_dispatch_sound_detach_connected,sa_main_dispatch_detach_8c0b1d60)
CONNECT(sa_main_dispatch_sound_status_connected,sa_main_dispatch_status_transition_8c0afc98)
CONNECT(sa_main_dispatch_sound_flag_connected,sa_main_dispatch_flag_transition_8c0b11c0)
SA_STAGE_EXPORT int sa_main_dispatch_sound_vector_calc_connected(uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *user,sa_matrix_observer observe){
 struct matrix_context s={user,observe,fpul,macl,0};(void)pr;
 return sa_model_direction_transform_extended(ram,fpu,sr,r,&s,calculation_instruction);
}
#define AXIS_CONNECT(name,callee) \
SA_STAGE_EXPORT int name(uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *user,sa_matrix_observer observe){ \
 struct matrix_context s={user,observe,fpul,macl,0};(void)pr; \
 return callee(ram,fpu,sr,r,fpul,&s,instruction); \
}
AXIS_CONNECT(sa_main_dispatch_sound_axis_x_connected,sa_matrix_rotate_x)
AXIS_CONNECT(sa_main_dispatch_sound_axis_y_connected,sa_matrix_rotate_y)
AXIS_CONNECT(sa_main_dispatch_sound_axis_z_connected,sa_matrix_rotate_z)
