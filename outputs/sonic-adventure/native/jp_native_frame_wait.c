#include "jp_native_frame_wait.h"
#include <windows.h>
#include <stddef.h>
static struct jp_wait_snapshot state;
static uint64_t floor_deadline,phase,period,part;
static jp_wait_idle host;
static void *host_context;
static void (*vsync)(void);
static DWORD owner;
static int initialized,busy,terminal=JP_WAIT_NOT_INITIALIZED;
static int fault(int code){terminal=code;state.status=(uint32_t)code;return code;}
static int access_owner(void)
{if(!initialized)return JP_WAIT_NOT_INITIALIZED;if(owner!=GetCurrentThreadId())return JP_WAIT_ARGUMENT;return terminal;}
static int sample(uint64_t *out)
{
    uint64_t now=jp_clock_wall_counts();
    if(jp_clock_status()!=JP_CLOCK_OK || now<state.last_wall)return fault(JP_WAIT_CLOCK);
    state.last_wall=now;*out=now;return JP_WAIT_OK;
}
static int next(void)
{
    uint64_t sum=phase+part,carry=sum/state.rate_numerator;
    if(period>UINT64_MAX-carry || floor_deadline>UINT64_MAX-(period+carry))return fault(JP_WAIT_OVERFLOW);
    uint64_t base=floor_deadline+period+carry,remainder=sum%state.rate_numerator;
    if(remainder && base==UINT64_MAX)return fault(JP_WAIT_OVERFLOW);
    floor_deadline=base;phase=remainder;state.next_deadline=base+(remainder!=0);return JP_WAIT_OK;
}
int jp_wait_initialize(uint32_t num,uint32_t den,uint32_t maximum,jp_wait_idle idle,void *context)
{
    if(initialized && owner!=GetCurrentThreadId())return JP_WAIT_ARGUMENT;
    if(busy)return JP_WAIT_REENTRANT;
    if(!num||!den||!maximum||maximum>4096||!idle)return JP_WAIT_ARGUMENT;
    uint64_t duration=UINT64_C(781250)*den,p=duration/num,q=duration%num;
    if(!p)return JP_WAIT_ARGUMENT;
    uint64_t now=jp_clock_wall_counts();
    if(jp_clock_status()!=JP_CLOCK_OK)return JP_WAIT_CLOCK;
    if(now>UINT64_MAX-p-(q!=0))return JP_WAIT_OVERFLOW;
    state=(struct jp_wait_snapshot){0,0,0,now,now+p+(q!=0),num,den,maximum,JP_WAIT_OK};
    floor_deadline=now+p;phase=q;period=p;part=q;host=idle;host_context=context;vsync=NULL;
    owner=GetCurrentThreadId();initialized=1;terminal=JP_WAIT_OK;return JP_WAIT_OK;
}
int jp_wait_shutdown(void)
{
    if(!initialized)return JP_WAIT_OK;if(owner!=GetCurrentThreadId())return JP_WAIT_ARGUMENT;if(busy)return JP_WAIT_REENTRANT;
    initialized=0;host=NULL;host_context=NULL;vsync=NULL;owner=0;state=(struct jp_wait_snapshot){0};fault(JP_WAIT_NOT_INITIALIZED);return JP_WAIT_OK;
}
int jp_wait_status(void){return terminal;}
int jp_wait_snapshot(struct jp_wait_snapshot *out)
{
    if(!initialized)return JP_WAIT_NOT_INITIALIZED;if(owner!=GetCurrentThreadId()||!out)return JP_WAIT_ARGUMENT;
    *out=state;return JP_WAIT_OK;
}
int jp_wait_set_vsync(void (*callback)(void))
{
    int rc=access_owner();if(rc)return rc;if(busy)return JP_WAIT_REENTRANT;
    uint64_t now;if(sample(&now))return terminal;
    if(now>UINT64_MAX-period-(part!=0))return fault(JP_WAIT_OVERFLOW);
    floor_deadline=now+period;phase=part;state.next_deadline=floor_deadline+(phase!=0);vsync=callback;return JP_WAIT_OK;
}
void njSetVSyncFunction(void (*callback)(void))
{
    int rc=jp_wait_set_vsync(callback);if(rc && initialized && owner==GetCurrentThreadId())fault(rc);
}
static int idle(uint32_t milliseconds)
{
    if(state.idle_calls==UINT64_MAX)return fault(JP_WAIT_OVERFLOW);
    ++state.idle_calls;int rc=host(host_context,milliseconds);
    if(terminal)return terminal;if(rc==0)return fault(JP_WAIT_CLOSED);if(rc<0)return fault(JP_WAIT_HOST);return JP_WAIT_OK;
}
static int pump(uint32_t *processed)
{
    uint32_t count=0;uint64_t now;int rc=idle(0);if(rc){*processed=0;return rc;}
    while(count<state.max_catchup){
        if(sample(&now))break;if(now<state.next_deadline)break;
        if(state.ticks==UINT64_MAX || (vsync && state.callbacks==UINT64_MAX)){fault(JP_WAIT_OVERFLOW);break;}
        if(next())break;++state.ticks;++count;
        if(vsync){++state.callbacks;vsync();if(terminal)break;}
    }
    *processed=count;return terminal;
}
int jp_wait_pump(uint32_t *processed)
{
    int rc=access_owner();if(rc)return rc;if(!processed)return JP_WAIT_ARGUMENT;if(busy)return JP_WAIT_REENTRANT;
    busy=1;rc=pump(processed);busy=0;return rc;
}
int jp_wait_sync(void)
{
    int rc=access_owner();if(rc)return rc;if(busy)return JP_WAIT_REENTRANT;
    busy=1;
    for(;;){uint32_t done;uint64_t now;rc=pump(&done);if(rc)break;
        if(sample(&now)){rc=terminal;break;}if(now<state.next_deadline)break;
    }
    busy=0;return rc;
}
static int wait_for(const volatile int *counter,int value,int equal)
{
    int rc=access_owner();if(rc)return rc;if(!counter)return JP_WAIT_ARGUMENT;if(busy)return JP_WAIT_REENTRANT;
    if(!(equal?*counter==value:*counter>value))return JP_WAIT_OK;
    if(!vsync)return fault(JP_WAIT_NO_CALLBACK);
    busy=1;
    while(equal?*counter==value:*counter>value){
        uint32_t done;rc=pump(&done);if(rc)break;
        if(!(equal?*counter==value:*counter>value))break;
        uint64_t now;if(sample(&now)){rc=terminal;break;}
        if(now>=state.next_deadline)continue;
        uint64_t delta=state.next_deadline-now;
        uint32_t ms=delta>=UINT64_C(39062)?50:(uint32_t)((delta*1000+781249)/781250);
        rc=idle(ms);if(rc)break;
    }
    busy=0;return rc;
}
int jp_wait_until_le(const volatile int *counter,int threshold){return wait_for(counter,threshold,0);}
int jp_wait_while_equal(const volatile int *counter,int value){return wait_for(counter,value,1);}
