#include "native_disc_queue.h"
#include "fpu_cleanup_transfer.h"
struct sa_cpu_state { uint32_t r[16],sr,pr,pc; };
typedef uint32_t (*sa_read)(void *,uint32_t);
typedef void (*sa_observer)(const uint8_t *,const struct sa_cpu_state *);
typedef int (*sa_dispatch)(uint8_t *,struct sa_cpu_state *,uint32_t,void *);
int sa_stream_cleanup_abort_native(uint8_t *,struct sa_cpu_state *,void *,sa_read,sa_dispatch,sa_observer,uint32_t);
int sa_request_abort_native(uint8_t *,struct sa_cpu_state *,void *,sa_read,sa_dispatch,sa_observer,uint32_t);
typedef int (*legacy_call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
int sa_file_close(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,legacy_call);
struct platform_context {struct sa_disc_queue *queue;uint32_t vector;void *user;sa_read read;};
static uint32_t timer(void *opaque,uint32_t address) {
    struct platform_context *ctx=(struct platform_context *)opaque;
    return ctx->read(ctx->user,address);
}
static int device(uint8_t *ram,struct sa_cpu_state *state,uint32_t target,void *opaque) {
    struct platform_context *ctx=(struct platform_context *)opaque;
    if(target!=ctx->vector || state->r[6]!=0)return -8;
    uint32_t result=0;
    int status=sa_disc_service(ctx->queue,ram,state->r[7],state->r[4],state->r[5],&result);
    if(status)return status;
    state->r[0]=result;state->pc=state->pr;return 0;
}
/* Connect recovered cleanup/abort/completion to new native disc platform code.
 * Caller initializes queue and RAM vector, owns stable guest RAM and host I/O.
 * Native disc service policies are explicit; this does not recover firmware. */
SA_DISC_EXPORT int sa_stream_cleanup_disc_native(uint8_t *ram,struct sa_cpu_state *state,
    struct sa_disc_queue *queue,uint32_t vector,void *user,sa_read read,sa_observer observe,uint32_t budget)
{
    if(!queue || !read)return -2;
    struct platform_context ctx={queue,vector,user,read};
    return sa_stream_cleanup_abort_native(ram,state,&ctx,timer,device,observe,budget);
}

struct close_context { struct platform_context platform;struct sa_cpu_state *state;sa_observer observe;uint32_t budget; };
static int close_service(void *opaque,uint32_t target,uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr)
{
    struct close_context *ctx=(struct close_context *)opaque;
    (void)f;(void)sr;(void)r;(void)pr;
    ctx->state->pc=target;
    if(target!=0x8c6013d2u)return -8;
    if(ctx->observe)ctx->observe(ram,ctx->state);
    return sa_request_abort_native(ram,ctx->state,&ctx->platform,timer,device,ctx->observe,ctx->budget);
}
/* Original close, including its pending-request abort, with per-call services. */
SA_DISC_EXPORT int sa_file_close_disc_native(uint8_t *ram,struct sa_cpu_state *state,
    struct sa_disc_queue *queue,uint32_t vector,void *user,sa_read read,sa_observer observe,uint32_t budget)
{
    if(!queue || !read)return -2;
    struct close_context ctx={{queue,vector,user,read},state,observe,budget};
    struct sa_fpu_banks unused={0};uint32_t fpul=0,macl=0;
    int result=sa_file_close(ram,&unused,&state->sr,state->r,&state->pr,&fpul,&macl,&ctx,close_service);
    if(!result)state->pc=state->pr;
    return result;
}
