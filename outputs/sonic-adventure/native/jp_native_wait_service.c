#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "jp_native_wait_service.h"
#include "jp_native_frame_wait.h"
_Static_assert(sizeof(struct jp_wait_service_info)==56,"wait service ABI");
static SRWLOCK service_lock=SRWLOCK_INIT;
static struct jp_wait_service_info info={.next_token=1};
static jp_wait_service_callback callback;
static void *context;
static int owner(void){return info.bound&&info.creator_thread!=GetCurrentThreadId()?JP_WAIT_SERVICE_THREAD:0;}
static int leave(int rc){ReleaseSRWLockExclusive(&service_lock);return rc;}
int jp_wait_service_bind(jp_wait_service_callback fn,void *ctx,uint32_t period,uint64_t *out)
{
    AcquireSRWLockExclusive(&service_lock);int rc=owner();if(rc)return leave(rc);
    if(info.in_callback)return leave(JP_WAIT_SERVICE_REENTRANT);
    if(info.bound||!info.next_token)return leave(JP_WAIT_SERVICE_STATE);
    if(!fn||!out||!period||period>10)return leave(JP_WAIT_SERVICE_ARGUMENT);
    struct jp_wait_snapshot wait;if(jp_wait_snapshot(&wait)||wait.status)return leave(JP_WAIT_SERVICE_STATE);
    uint64_t token=info.next_token++;
    info=(struct jp_wait_service_info){.token=token,.next_token=info.next_token,
        .bound=1,.creator_thread=GetCurrentThreadId(),.period_ms=period};
    callback=fn;context=ctx;*out=token;return leave(0);
}
int jp_wait_service_unbind(uint64_t token)
{
    AcquireSRWLockExclusive(&service_lock);int rc=owner();if(rc)return leave(rc);
    if(info.in_callback)return leave(JP_WAIT_SERVICE_REENTRANT);
    if(!info.bound||!token||token!=info.token)return leave(JP_WAIT_SERVICE_STATE);
    callback=NULL;context=NULL;info.token=0;info.bound=0;return leave(0);
}
int jp_wait_service_run(void)
{
    AcquireSRWLockExclusive(&service_lock);int rc=owner();if(rc)return leave(rc);
    if(info.in_callback)return leave(JP_WAIT_SERVICE_REENTRANT);
    if(!info.bound)return leave(0);
    if(info.calls==UINT64_MAX)return leave(JP_WAIT_SERVICE_STATE);
    jp_wait_service_callback fn=callback;void *ctx=context;info.in_callback=1;info.calls++;
    ReleaseSRWLockExclusive(&service_lock);int32_t native=fn(ctx);
    AcquireSRWLockExclusive(&service_lock);info.in_callback=0;info.native_result=native;
    info.status=native?JP_WAIT_SERVICE_CALLBACK:0;
    if(native&&info.failures!=UINT64_MAX)info.failures++;
    return leave((int)info.status);
}
uint32_t jp_wait_service_limit(uint32_t requested)
{
    AcquireSRWLockExclusive(&service_lock);
    if(!owner()&&info.bound&&requested>info.period_ms)requested=info.period_ms;
    ReleaseSRWLockExclusive(&service_lock);return requested;
}
int jp_wait_service_snapshot(struct jp_wait_service_info *out)
{
    AcquireSRWLockExclusive(&service_lock);int rc=owner();if(rc)return leave(rc);
    if(!out)return leave(JP_WAIT_SERVICE_ARGUMENT);*out=info;return leave(0);
}
