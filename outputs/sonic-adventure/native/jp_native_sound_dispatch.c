#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <string.h>
#include "jp_native_sound_dispatch.h"
_Static_assert(sizeof(struct jp_sound_dispatch_info)==64,"sound dispatch ABI");
static SRWLOCK lock=SRWLOCK_INIT;
static uint64_t next_token=1;
static struct jp_sound_dispatch_info state;
static jp_sound_dispatch_callback service_callback,queue_callback;
static void *callback_context;
static int access_owner(void)
{
    if(state.bound&&state.creator_thread!=GetCurrentThreadId())return JP_SOUND_DISPATCH_THREAD;
    return 0;
}
static int finish(int code)
{
    if(code!=JP_SOUND_DISPATCH_THREAD)state.status=(uint32_t)code;
    ReleaseSRWLockExclusive(&lock);return code;
}
int jp_sound_dispatch_bind(jp_sound_dispatch_callback service,
    jp_sound_dispatch_callback queue,void *context,uint64_t *token)
{
    AcquireSRWLockExclusive(&lock);int code=access_owner();if(code)return finish(code);
    if(state.in_callback)return finish(JP_SOUND_DISPATCH_REENTRANT);
    if(state.bound)return finish(JP_SOUND_DISPATCH_BOUND);
    if(!service||!token)return finish(JP_SOUND_DISPATCH_ARGUMENT);
    if(!next_token)return finish(JP_SOUND_DISPATCH_BOUND);
    service_callback=service;queue_callback=queue;callback_context=context;
    memset(&state,0,sizeof state);state.bound=1;state.creator_thread=GetCurrentThreadId();
    state.has_queue=queue!=NULL;state.token=next_token++;*token=state.token;return finish(0);
}
int jp_sound_dispatch_unbind(uint64_t token)
{
    AcquireSRWLockExclusive(&lock);int code=access_owner();if(code)return finish(code);
    if(state.in_callback)return finish(JP_SOUND_DISPATCH_REENTRANT);
    if(!state.bound||!token||token!=state.token)return finish(JP_SOUND_DISPATCH_UNBOUND);
    service_callback=queue_callback=NULL;callback_context=NULL;
    state.bound=state.has_queue=0;state.token=0;return finish(0);
}
static int dispatch(int queue,int32_t *native_result)
{
    AcquireSRWLockExclusive(&lock);int code=access_owner();if(code)return finish(code);
    if(!native_result)return finish(JP_SOUND_DISPATCH_ARGUMENT);
    if(state.in_callback)return finish(JP_SOUND_DISPATCH_REENTRANT);
    if(!state.bound)return finish(JP_SOUND_DISPATCH_UNBOUND);
    jp_sound_dispatch_callback callback=queue?queue_callback:service_callback;
    if(!callback)return finish(JP_SOUND_DISPATCH_UNAVAILABLE);
    void *context=callback_context;state.in_callback=1;state.active_kind=queue?2:1;
    if(queue)state.queue_calls++;else state.service_calls++;
    ReleaseSRWLockExclusive(&lock);
    int32_t result=callback(context);
    AcquireSRWLockExclusive(&lock);state.in_callback=state.active_kind=0;
    state.native_result=result;*native_result=result;
    if(result){state.failures++;return finish(JP_SOUND_DISPATCH_CALLBACK);}
    return finish(0);
}
int jp_sound_dispatch_service(int32_t *result){return dispatch(0,result);}
int jp_sound_dispatch_queue(int32_t *result){return dispatch(1,result);}
int jp_sound_dispatch_snapshot(struct jp_sound_dispatch_info *out)
{
    AcquireSRWLockExclusive(&lock);int code=access_owner();
    if(!code&&!out)code=JP_SOUND_DISPATCH_ARGUMENT;
    if(!code)*out=state;
    ReleaseSRWLockExclusive(&lock);return code;
}
