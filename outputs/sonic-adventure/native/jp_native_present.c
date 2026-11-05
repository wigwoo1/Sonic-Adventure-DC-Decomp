#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <kamui.h>
#include <string.h>
#include "jp_native_present.h"
#include "jp_native_frames.h"
#include "jp_native_queue.h"
_Static_assert(sizeof(KMDWORD)==4,"native SDK statistics are 32-bit");
struct kmi_interrupt_statistics kmiIntStatisticData;
static struct {
    struct jp_gpu *gpu;struct jp_window *window;volatile struct kmi_device_extension *device;
    struct jp_gpu_config config;struct jp_present_info info;
    struct jp_gpu_timing timing;struct jp_gpu_report report;
    struct jp_present_callback_info callbacks;
    uint64_t started,cadence_tick;DWORD owner;int initialized,busy,status,dispatching,draining;
} backend={.status=JP_PRESENT_NOT_INITIALIZED};
static void dispatch(unsigned slot,void *argument)
{
    PKMCALLBACKFUNC fn=backend.device->callback[slot].function;
    if(!fn)return;
    backend.dispatching=1;fn(argument);backend.dispatching=0;
}
static int fault(int code)
{
    int first=backend.initialized && backend.status==JP_PRESENT_OK;
    if(!backend.initialized||backend.status==0||backend.status==JP_PRESENT_NOT_INITIALIZED)backend.status=code;
    backend.info.status=(uint32_t)backend.status;
    if(first && code && !backend.draining){
        backend.callbacks.last_fatal=code==JP_PRESENT_TIMEOUT?KMI_FATAL_FLIP_TIMEOUT:0x10000u+(uint32_t)code;
        if(backend.device->callback[FATAL_ERROR_CALLBACK].function){
            ++backend.callbacks.fatal_calls;
            dispatch(FATAL_ERROR_CALLBACK,(void *)(uintptr_t)backend.callbacks.last_fatal);
        }
    }
    return backend.status;
}
static int owner(void)
{if(!backend.initialized)return JP_PRESENT_NOT_INITIALIZED;if(backend.owner!=GetCurrentThreadId())return JP_PRESENT_THREAD;return JP_PRESENT_OK;}
static int wall(uint64_t *out)
{*out=jp_clock_wall_counts();return jp_clock_status()==JP_CLOCK_OK?0:fault(JP_PRESENT_CLOCK);}
static int elapsed(uint64_t start,uint64_t end,KMDWORD *out)
{if(end<start||end-start>UINT32_MAX)return fault(JP_PRESENT_TIMING);*out=(KMDWORD)(end-start);return 0;}
static int expired(uint64_t begin)
{uint64_t now;if(wall(&now))return -1;if(now<begin){fault(JP_PRESENT_CLOCK);return -1;}return now-begin>=((uint64_t)backend.info.timeout_ms*781250+999)/1000;}
static KMSTATUS sdk_result(int code){return code?KMSTATUS_INVALID_PARAMETER:KMSTATUS_SUCCESS;}
int jp_present_initialize(struct jp_gpu *gpu,struct jp_window *window,
    volatile struct kmi_device_extension *device,const struct jp_gpu_config *config,uint32_t timeout_ms,unsigned interval)
{
    if(backend.initialized)return owner()?JP_PRESENT_THREAD:JP_PRESENT_STATE;
    if(!gpu||!window||!device||!config||!timeout_ms||timeout_ms>60000||interval>1||config->textures>1||
       (config->flat_color_vertex!=0 && config->flat_color_vertex!=2)||!config->budget)return JP_PRESENT_ARGUMENT;
    unsigned width,height;if(jp_window_size(window,&width,&height)||config->width!=width||config->height!=height)return JP_PRESENT_ARGUMENT;
    struct jp_wait_snapshot s;if(jp_clock_status()!=JP_CLOCK_OK||jp_wait_snapshot(&s)||jp_wait_status()!=JP_WAIT_OK)return JP_PRESENT_CLOCK;
    backend.gpu=gpu;backend.window=window;backend.device=device;backend.config=*config;backend.config.palette=NULL;backend.config.palette_entries=0;
    backend.info=(struct jp_present_info){0};backend.info.timeout_ms=timeout_ms;backend.info.interval=interval;
    backend.callbacks=(struct jp_present_callback_info){0};
    backend.owner=GetCurrentThreadId();backend.initialized=1;backend.busy=backend.dispatching=backend.draining=0;backend.status=JP_PRESENT_OK;
    static const unsigned owned_callbacks[]={END_RENDER_CALLBACK,END_VERTEX_CALLBACK,FATAL_ERROR_CALLBACK};
    for(unsigned i=0;i<sizeof owned_callbacks/sizeof owned_callbacks[0];++i){
        unsigned slot=owned_callbacks[i];device->callback[slot].function=NULL;device->callback[slot].argument=NULL;
    }
    memset(&kmiIntStatisticData,0,sizeof kmiIntStatisticData);device->render_busy=device->ta_busy=device->flip_pending=0;
    backend.cadence_tick=s.ticks;device->vsync_remaining=device->vsync_wait=0;
    return JP_PRESENT_OK;
}
int jp_present_status(void){return backend.status;}
int jp_present_snapshot(struct jp_present_info *out)
{int rc=owner();if(rc)return rc;if(!out)return JP_PRESENT_ARGUMENT;*out=backend.info;return 0;}
int jp_present_callback_snapshot(struct jp_present_callback_info *out)
{int rc=owner();if(rc)return rc;if(!out)return JP_PRESENT_ARGUMENT;*out=backend.callbacks;return 0;}
static int cadence(uint64_t tick)
{
    if(tick<backend.cadence_tick || backend.device->vsync_remaining<0 || backend.device->vsync_wait<0)
        return fault(JP_PRESENT_STATE);
    uint64_t delta=tick-backend.cadence_tick;
    uint64_t remaining=(uint32_t)backend.device->vsync_remaining;
    uint64_t period=(uint64_t)(uint32_t)backend.device->vsync_wait+1;
    if(delta<=remaining)remaining-=delta;
    else remaining=period-1-(delta-remaining-1)%period;
    backend.device->vsync_remaining=(KMINT32)remaining;
    backend.device->vsync_count=(KMDWORD)tick;backend.cadence_tick=tick;
    return JP_PRESENT_OK;
}
int jp_present_cadence_snapshot(struct jp_present_cadence_info *out)
{
    int rc=owner();if(rc)return rc;if(!out)return JP_PRESENT_ARGUMENT;
    struct jp_wait_snapshot s;if(jp_wait_snapshot(&s))return JP_PRESENT_WAIT;
    if(cadence(s.ticks))return backend.status;
    *out=(struct jp_present_cadence_info){s.ticks,backend.device->vsync_remaining,backend.device->vsync_wait};return 0;
}
KMSTATUS kmSetWaitVsyncCount(KMINT32 count)
{
    int rc=owner();if(rc)return sdk_result(rc);if(backend.status)return sdk_result(backend.status);
    if(count<1)return KMSTATUS_INVALID_PARAMETER;
    if(backend.busy||backend.dispatching)return sdk_result(JP_PRESENT_REENTRANT);
    if(jp_wait_sync())return sdk_result(fault(JP_PRESENT_WAIT));
    struct jp_wait_snapshot s;if(jp_wait_snapshot(&s))return sdk_result(fault(JP_PRESENT_WAIT));
    if(cadence(s.ticks))return sdk_result(backend.status);
    backend.device->vsync_wait=count-1;return KMSTATUS_SUCCESS;
}
size_t jp_present_stats_size(void){return sizeof kmiIntStatisticData;}
int jp_present_read_stats(struct kmi_interrupt_statistics *out,size_t bytes)
{int rc=owner();if(rc)return rc;if(!out||bytes!=sizeof *out)return JP_PRESENT_ARGUMENT;memcpy(out,&kmiIntStatisticData,sizeof *out);return 0;}
int jp_present_timing_ready(void){return owner()==0 && backend.status==0 && backend.info.history;}
static int poll(void)
{
    if(!backend.info.job)return JP_PRESENT_STATE;
    if(backend.info.ready)return JP_PRESENT_OK;
    struct jp_gpu_failure failure;int rc=jp_gpu_job_poll(backend.gpu,backend.info.job,&backend.timing,&failure);
    if(rc==JP_GPU_JOB_PENDING){++kmiIntStatisticData.render_wait;return JP_PRESENT_PENDING;}
    if(rc)return fault(JP_PRESENT_GPU);
    uint64_t now;if(wall(&now)||elapsed(backend.started,now,&kmiIntStatisticData.render_time))return backend.status;
    backend.info.ready=1;backend.info.history=1;backend.info.gpu_timing_valid=backend.timing.gpu_timing_valid;
    ++backend.info.completions;backend.device->render_busy=0;
    if(backend.device->callback[END_RENDER_CALLBACK].function){
        ++backend.info.eor_calls;dispatch(END_RENDER_CALLBACK,backend.device->callback[END_RENDER_CALLBACK].argument);
    }
    return backend.status;
}
int jp_present_poll(void)
{
    int rc=owner();if(rc)return rc;if(backend.busy||backend.dispatching)return JP_PRESENT_REENTRANT;if(backend.status)return backend.status;
    backend.busy=1;rc=poll();backend.busy=0;return rc;
}
static KMSTATUS register_callback(unsigned slot,PKMCALLBACKFUNC function,void *argument)
{
    int rc=owner();if(rc)return sdk_result(rc);if(backend.status)return sdk_result(backend.status);
    if(backend.busy||backend.dispatching)return sdk_result(JP_PRESENT_REENTRANT);
    backend.device->callback[slot].function=function;backend.device->callback[slot].argument=argument;return KMSTATUS_SUCCESS;
}
KMSTATUS kmSetEORCallback(PKMCALLBACKFUNC function,void *argument)
{return register_callback(END_RENDER_CALLBACK,function,argument);}
KMSTATUS kmSetEndOfVertexCallback(PKMCALLBACKFUNC function,void *argument)
{return register_callback(END_VERTEX_CALLBACK,function,argument);}
KMSTATUS kmfSetFatalErrorCallback(PKMCALLBACKFUNC function)
{return register_callback(FATAL_ERROR_CALLBACK,function,NULL);}
KMSTATUS kmRender(void)
{
    int rc=owner();if(rc)return sdk_result(rc);if(backend.dispatching)return sdk_result(JP_PRESENT_REENTRANT);if(backend.busy)return sdk_result(fault(JP_PRESENT_REENTRANT));if(backend.status)return sdk_result(backend.status);
    if(backend.info.frame||backend.info.job)return sdk_result(fault(JP_PRESENT_STATE));
    backend.busy=1;
    if(jp_wait_sync()){rc=fault(JP_PRESENT_WAIT);goto done;}
    struct jp_wait_snapshot s;if(jp_wait_snapshot(&s)){rc=fault(JP_PRESENT_WAIT);goto done;}
    if(wall(&backend.started)){rc=backend.status;goto done;}
    kmiIntStatisticData.ta_start=kmiIntStatisticData.render_start=~(KMDWORD)backend.started;
    backend.device->ta_busy=1;
    if(jp_frames_submit(&backend.info.frame)){backend.device->ta_busy=0;rc=fault(JP_PRESENT_FRAME);goto done;}
    struct jp_gpu_failure failure;
    if(jp_gpu_submit_frame(backend.gpu,backend.info.frame,&backend.config,&backend.info.job,&backend.report,&failure)){
        backend.device->ta_busy=0;rc=fault(JP_PRESENT_GPU);goto done;
    }
    backend.device->ta_busy=0;backend.device->render_busy=1;backend.info.submitted_tick=s.ticks;
    backend.info.required_tick=backend.info.presented_tick=0;backend.info.ready=0;++backend.info.submissions;kmiIntStatisticData.render_wait=0;
    if(backend.device->callback[END_VERTEX_CALLBACK].function){
        ++backend.callbacks.eov_calls;dispatch(END_VERTEX_CALLBACK,backend.device->callback[END_VERTEX_CALLBACK].argument);
    }
    uint64_t now;if(wall(&now)||elapsed(backend.started,now,&kmiIntStatisticData.ta_time)){rc=backend.status;goto done;}
    static const unsigned lists[5]={KM_OPAQUE_POLYGON,KM_TRANS_POLYGON,KM_PUNCHTHROUGH_POLYGON,KM_OPAQUE_MODIFIER,KM_TRANS_MODIFIER};
    for(unsigned i=0;i<5;i++){
        size_t bytes;const void *data=jp_frame_list(backend.info.frame,i,&bytes);
        if((bytes && !data)||bytes>UINT32_MAX){rc=fault(JP_PRESENT_FRAME);goto done;}
        if(bytes>kmiIntStatisticData.list_max[lists[i]])kmiIntStatisticData.list_max[lists[i]]=(KMDWORD)bytes;
    }
    rc=JP_PRESENT_OK;
done:
    backend.busy=0;return sdk_result(rc);
}
KMSTATUS kmFlipFrameBuffer(void)
{
    int rc=owner();if(rc)return sdk_result(rc);if(backend.dispatching)return sdk_result(JP_PRESENT_REENTRANT);if(backend.busy)return sdk_result(fault(JP_PRESENT_REENTRANT));if(backend.status)return sdk_result(backend.status);
    if(!backend.info.job)return sdk_result(fault(JP_PRESENT_STATE));
    backend.busy=1;backend.device->flip_pending=1;uint64_t begin;
    if(wall(&begin)){rc=backend.status;goto done;}
    while(!backend.info.ready){
        if(jp_wait_sync()){rc=fault(JP_PRESENT_WAIT);goto done;}
        rc=poll();if(rc!=JP_PRESENT_OK && rc!=JP_PRESENT_PENDING)goto done;
        if(!backend.info.ready){
            int timeout=expired(begin);if(timeout){if(timeout>0){++kmiIntStatisticData.render_timeouts;++backend.info.timeouts;fault(JP_PRESENT_TIMEOUT);}rc=backend.status;goto done;}
            if(jp_wait_service(1)){rc=fault(JP_PRESENT_WAIT);goto done;}
        }
    }
    /* A flip request uses a future logical boundary after readiness has been
     * observed and overdue callbacks serviced. It never presents mid-tick. */
    if(jp_wait_sync()){rc=fault(JP_PRESENT_WAIT);goto done;}
    struct jp_wait_snapshot s;if(jp_wait_snapshot(&s)||s.ticks==UINT64_MAX){rc=fault(JP_PRESENT_WAIT);goto done;}
    if(cadence(s.ticks)){rc=backend.status;goto done;}
    uint64_t delay=(uint64_t)(uint32_t)backend.device->vsync_remaining+1;
    if(s.ticks>UINT64_MAX-delay){rc=fault(JP_PRESENT_WAIT);goto done;}
    backend.info.required_tick=s.ticks+delay;
    while(s.ticks<backend.info.required_tick){
        int timeout=expired(begin);if(timeout){if(timeout>0){++backend.info.timeouts;fault(JP_PRESENT_TIMEOUT);}rc=backend.status;goto done;}
        if(jp_wait_service(1)||jp_wait_snapshot(&s)){rc=fault(JP_PRESENT_WAIT);goto done;}
    }
    if(cadence(s.ticks)){rc=backend.status;goto done;}
    struct jp_gpu_failure failure;
    if(jp_window_present_job(backend.window,backend.info.job,backend.info.interval,&failure)){rc=fault(JP_PRESENT_GPU);goto done;}
    ++backend.info.presentations;backend.info.presented_tick=s.ticks;
    if(jp_gpu_job_release(backend.gpu,backend.info.job,&failure)){rc=fault(JP_PRESENT_GPU);goto done;}
    backend.info.job=0;
    if(jp_frame_release(backend.info.frame)){rc=fault(JP_PRESENT_FRAME);goto done;}
    backend.info.frame=0;backend.info.ready=0;backend.device->write_slot=(backend.device->write_slot+1)&3;
    rc=JP_PRESENT_OK;
done:
    backend.device->flip_pending=0;backend.busy=0;return sdk_result(rc);
}
void kmResetPointer(void)
{
    int rc=owner();if(rc)return;if(backend.dispatching)return;if(backend.busy){fault(JP_PRESENT_REENTRANT);return;}
    if(backend.info.job||backend.info.frame){fault(JP_PRESENT_STATE);return;}
    if(jp_queue_reset_frame())fault(JP_PRESENT_FRAME);
}
int jp_present_shutdown(void)
{
    int rc=owner();if(rc)return rc;if(backend.busy||backend.dispatching)return JP_PRESENT_REENTRANT;
    backend.busy=1;backend.draining=1;uint64_t begin=GetTickCount64();
    if(backend.info.job){
        struct jp_gpu_failure failure;struct jp_gpu_timing t;
        for(;;){
            int code=jp_gpu_job_poll(backend.gpu,backend.info.job,&t,&failure);
            if(code!=JP_GPU_JOB_PENDING){
                if(jp_gpu_job_release(backend.gpu,backend.info.job,&failure)){backend.busy=0;return fault(JP_PRESENT_GPU);}
                backend.info.job=0;break;
            }
            if(GetTickCount64()-begin>=backend.info.timeout_ms){backend.busy=0;return fault(JP_PRESENT_TIMEOUT);}
            Sleep(1);
        }
    }
    if(backend.info.frame){if(jp_frame_release(backend.info.frame)){backend.busy=0;return fault(JP_PRESENT_FRAME);}backend.info.frame=0;}
    backend.device->render_busy=backend.device->ta_busy=backend.device->flip_pending=0;
    static const unsigned owned_callbacks[]={END_RENDER_CALLBACK,END_VERTEX_CALLBACK,FATAL_ERROR_CALLBACK};
    for(unsigned i=0;i<sizeof owned_callbacks/sizeof owned_callbacks[0];++i){
        unsigned slot=owned_callbacks[i];backend.device->callback[slot].function=NULL;backend.device->callback[slot].argument=NULL;
    }
    backend.initialized=0;backend.busy=0;backend.gpu=NULL;backend.window=NULL;backend.device=NULL;fault(JP_PRESENT_NOT_INITIALIZED);return JP_PRESENT_OK;
}
