#include "fpu_cleanup_transfer.h"
struct sa_cpu_state { uint32_t r[16],sr,pr,pc; };
typedef int (*sa_dispatch)(uint8_t *,struct sa_cpu_state *,uint32_t,void *);
typedef int (*sa_legacy_call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
int sa_file_completion_process(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_legacy_call);
int sa_file_device_status(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_legacy_call);
struct completion_context { struct sa_cpu_state *state;sa_dispatch dispatch;void *user; };
static int service(void *opaque,uint32_t target,uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr) {
    struct completion_context *ctx=(struct completion_context *)opaque;
    (void)f;(void)sr;(void)r;(void)pr;
    ctx->state->pc=target;
    return ctx->dispatch(ram,ctx->state,target,ctx->user);
}
/* The completion body contains integer instructions only. Its legacy ABI's
 * FPU/FPUL/MACL arguments are unused; CPU fields alias the connected state.
 * On host failure the service PC and partial frame remain observable. */
SA_STAGE_EXPORT int sa_file_completion_connected(uint8_t *ram,struct sa_cpu_state *state,sa_dispatch dispatch,void *user) {
    if(!dispatch)return -2;
    struct sa_fpu_banks unused={0};uint32_t fpul=0,macl=0;
    struct completion_context ctx={state,dispatch,user};
    int result=sa_file_completion_process(ram,&unused,&state->sr,state->r,&state->pr,&fpul,&macl,&ctx,service);
    if(!result)state->pc=state->pr;
    return result;
}

SA_STAGE_EXPORT int sa_file_device_status_connected(uint8_t *ram,struct sa_cpu_state *state,sa_dispatch dispatch,void *user) {
    if(!dispatch)return -2;
    struct sa_fpu_banks unused={0};uint32_t fpul=0,macl=0;
    struct completion_context ctx={state,dispatch,user};
    int result=sa_file_device_status(ram,&unused,&state->sr,state->r,&state->pr,&fpul,&macl,&ctx,service);
    if(!result)state->pc=state->pr;
    return result;
}
