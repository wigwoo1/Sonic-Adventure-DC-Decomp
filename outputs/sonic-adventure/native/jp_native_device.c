/* Real native resource owner around the original Ninja initialization body.
 * This unit is compiled against host headers, without SDK CRT shadowing. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "jp_native_device_internal.h"
#include "jp_native_present.h"
#include "jp_native_frames_internal.h"
#include "jp_native_queue.h"
extern int jp_textures_status(void);
static struct {
    struct jp_device_options options;
    struct jp_device_info info;
    struct jp_gpu *gpu;struct jp_window *window;
    DWORD thread;int busy;uint64_t exit_token;
} device={.options={640,480,1,0,5000,0,64u<<20},.info={.status=JP_DEVICE_STATE}};
static uint64_t next_exit_token=1;
enum {OWN_CLOCK=1,OWN_WAIT=2,OWN_GPU=4,OWN_WINDOW=8,OWN_VIDEO=16,OWN_PRESENT=32};
static int access(void)
{return device.thread && device.thread!=GetCurrentThreadId()?JP_DEVICE_THREAD:JP_DEVICE_OK;}
int jp_device_fail(int error)
{if(!device.info.status)device.info.status=(uint32_t)error;return (int)device.info.status;}
int jp_device_status(void)
{
    int rc=access();if(rc)return rc;
    if(!device.info.status && device.info.phase==JP_DEVICE_READY){
        if(jp_present_status())jp_device_fail(JP_DEVICE_PRESENT);
        else if(jp_wait_status())jp_device_fail(JP_DEVICE_WAIT);
        else if(jp_clock_status())jp_device_fail(JP_DEVICE_CLOCK);
        else if(jp_video_status())jp_device_fail(JP_DEVICE_VIDEO);
    }
    return (int)device.info.status;
}
int jp_device_phase_is(unsigned phase)
{return !access() && device.busy && device.info.phase==phase && !device.info.status;}
int jp_device_configure(const struct jp_device_options *options)
{
    int rc=access();if(rc)return rc;if(device.info.phase || device.busy || device.exit_token)return JP_DEVICE_STATE;
    if(!options || !options->render_width || !options->render_height || options->render_width>8192 || options->render_height>8192 ||
       options->visible>1 || options->present_interval>1 || !options->timeout_ms || options->timeout_ms>60000 ||
       options->reserved || !options->gpu_budget || options->gpu_budget>SIZE_MAX)return JP_DEVICE_ARGUMENT;
    device.options=*options;return 0;
}
static int initial_idle(void *context,uint32_t milliseconds)
{
    (void)context;
    if(device.window)return jp_wait_window_idle(device.window,milliseconds);
    DWORD result=MsgWaitForMultipleObjectsEx(0,NULL,milliseconds,QS_ALLINPUT,MWMO_INPUTAVAILABLE);
    if(result==WAIT_FAILED)return -1;
    MSG message;while(PeekMessageW(&message,NULL,0,0,PM_REMOVE)){
        if(message.message==WM_QUIT)return 0;TranslateMessage(&message);DispatchMessageW(&message);
    }
    return 1;
}
int jp_device_wait_tick(void)
{
    int rc=access();if(rc)return rc;
    if(!device.info.phase || jp_device_status())return jp_device_fail(JP_DEVICE_WAIT);
    struct jp_wait_snapshot s;if(jp_wait_sync() || jp_wait_snapshot(&s) || s.ticks==UINT64_MAX)return jp_device_fail(JP_DEVICE_WAIT);
    uint64_t target=s.ticks+1,begin=GetTickCount64();
    while(s.ticks<target){
        if(GetTickCount64()-begin>=device.options.timeout_ms || jp_wait_service(1) || jp_wait_snapshot(&s))return jp_device_fail(JP_DEVICE_WAIT);
    }
    device.info.logical_ticks=s.ticks;return 0;
}
int jp_device_set_cable(unsigned cable)
{int rc=access();if(rc)return rc;if(!device.busy || (cable&~0x300u))return jp_device_fail(JP_DEVICE_ARGUMENT);device.info.cable=cable>>8;return 0;}
int jp_device_create_gpu(void)
{
    if(!jp_device_phase_is(JP_DEVICE_PREPARED))return jp_device_fail(JP_DEVICE_STATE);
    struct jp_gpu_failure error;if(jp_gpu_open(&device.gpu,&error))return jp_device_fail(JP_DEVICE_GPU);
    device.info.resources|=OWN_GPU;device.info.phase=JP_DEVICE_GPU_READY;return 0;
}
int jp_device_create_display(unsigned width,unsigned height,void *sdk_device)
{
    if(!jp_device_phase_is(JP_DEVICE_GPU_READY))return jp_device_fail(JP_DEVICE_STATE);
    struct jp_gpu_failure error;
    if(jp_window_open(device.gpu,device.options.render_width,device.options.render_height,
       (int)device.options.visible,&device.window,&error))return jp_device_fail(JP_DEVICE_WINDOW);
    device.info.resources|=OWN_WINDOW;
    struct jp_gpu_config config={0};config.width=device.options.render_width;config.height=device.options.render_height;
    config.logical_width=(float)width;config.logical_height=(float)height;config.textures=1;config.punch_alpha=.5f;
    config.budget=(size_t)device.options.gpu_budget;
    if(jp_present_initialize(device.gpu,device.window,sdk_device,&config,device.options.timeout_ms,device.options.present_interval))return jp_device_fail(JP_DEVICE_PRESENT);
    device.info.resources|=OWN_PRESENT;device.info.phase=JP_DEVICE_DISPLAY_READY;
    device.info.render_width=config.width;device.info.render_height=config.height;return 0;
}
static int drain(void)
{
    if(device.info.resources&OWN_PRESENT){
        if(jp_present_shutdown())return JP_DEVICE_PRESENT;
        device.info.resources&=~OWN_PRESENT;
    }
    if((device.gpu && jp_gpu_jobs_live(device.gpu)) || jp_frames_live_count())return JP_DEVICE_ACTIVE;
    if(device.info.resources&OWN_WAIT){if(jp_wait_shutdown())return JP_DEVICE_WAIT;device.info.resources&=~OWN_WAIT;}
    if(device.info.resources&OWN_WINDOW){jp_window_close(device.window);device.window=NULL;device.info.resources&=~OWN_WINDOW;}
    if(device.info.resources&OWN_GPU){jp_gpu_close(device.gpu);device.gpu=NULL;device.info.resources&=~OWN_GPU;}
    if(device.info.resources&OWN_VIDEO){jp_video_shutdown();device.info.resources&=~OWN_VIDEO;}
    if(device.info.resources&OWN_CLOCK){jp_clock_shutdown();device.info.resources&=~OWN_CLOCK;}
    device.info.phase=JP_DEVICE_COLD;if(!device.exit_token)device.thread=0;return 0;
}
int jp_device_shutdown(void)
{
    int rc=access();if(rc)return rc;if(device.busy || device.exit_token)return JP_DEVICE_STATE;
    if(!device.info.resources)return 0;
    device.busy=1;device.info.phase=JP_DEVICE_DRAINING;rc=drain();device.busy=0;return rc;
}
int jp_device_initialize(int32_t mode,int32_t frame,int32_t count)
{
    int rc=access();if(rc)return rc;if(device.info.phase || device.busy || device.exit_token)return JP_DEVICE_STATE;
    if(jp_device_sdk_validate(mode,frame,count))return JP_DEVICE_ARGUMENT;
    if(jp_queue_status() || jp_frames_status() || jp_textures_status())return JP_DEVICE_QUEUE;
    if(!jp_frame_queue_empty() || jp_frames_live_count())return JP_DEVICE_ACTIVE;
    if(jp_clock_status()!=JP_CLOCK_NOT_INITIALIZED || jp_wait_status()!=JP_WAIT_NOT_INITIALIZED ||
       jp_present_status()!=JP_PRESENT_NOT_INITIALIZED || jp_video_status()!=JP_VIDEO_NOT_INITIALIZED)return JP_DEVICE_STATE;
    device.thread=GetCurrentThreadId();device.busy=1;device.info=(struct jp_device_info){.phase=JP_DEVICE_PREPARED,.wait_count=count};
    if(jp_clock_initialize(NULL,NULL,0)){jp_device_fail(JP_DEVICE_CLOCK);goto failed;}
    device.info.resources|=OWN_CLOCK;
    if(jp_wait_initialize((mode&3)==2?50:60000,(mode&3)==2?1:1001,8,initial_idle,NULL)){jp_device_fail(JP_DEVICE_WAIT);goto failed;}
    device.info.resources|=OWN_WAIT;
    jp_original_ninja_system(mode,frame,count);
    if(device.info.status)goto failed;
    if(device.info.phase!=JP_DEVICE_DISPLAY_READY){jp_device_fail(JP_DEVICE_STATE);goto failed;}
    jp_device_sdk_snapshot(&device.info);device.info.phase=JP_DEVICE_READY;device.busy=0;return 0;
failed:
    device.info.phase=JP_DEVICE_DRAINING;rc=drain();device.busy=0;
    return rc?rc:(int)device.info.status;
}
int jp_device_sdk_video(unsigned video)
{
    /* Called only after the SDK bridge initialized the real video owner. */
    if(!jp_device_phase_is(JP_DEVICE_GPU_READY))return jp_device_fail(JP_DEVICE_STATE);
    device.info.video=video;device.info.resources|=OWN_VIDEO;return 0;
}
int jp_device_snapshot(struct jp_device_info *out)
{
    int rc=access();if(rc)return rc;if(!out)return JP_DEVICE_ARGUMENT;
    (void)jp_device_status();struct jp_device_info info=device.info;
    if(info.resources&OWN_WAIT){struct jp_wait_snapshot s;if(jp_wait_snapshot(&s))return JP_DEVICE_WAIT;info.logical_ticks=s.ticks;}
    if(info.phase==JP_DEVICE_READY)jp_device_sdk_snapshot(&info);
    *out=info;return 0;
}
struct jp_gpu *jp_device_gpu(void){return !access() && device.info.phase==JP_DEVICE_READY?device.gpu:NULL;}
struct jp_window *jp_device_window(void){return !access() && device.info.phase==JP_DEVICE_READY?device.window:NULL;}
int jp_device_exit_claim(uint64_t *token)
{
    int rc=access();if(rc)return rc;if(!token)return JP_DEVICE_ARGUMENT;
    if(device.busy || device.exit_token || !device.info.resources ||
       (device.info.phase!=JP_DEVICE_READY && device.info.phase!=JP_DEVICE_DRAINING) || !next_exit_token)return JP_DEVICE_STATE;
    if(device.info.phase==JP_DEVICE_READY){
        struct jp_present_info present;
        if(jp_present_snapshot(&present) || present.frame || present.job || !jp_frame_queue_empty())return JP_DEVICE_ACTIVE;
    }
    device.exit_token=next_exit_token++;
    *token=device.exit_token;return 0;
}
static int exit_access(uint64_t token)
{
    /* Delegated terminal drains retain their creator-thread authority. */
    int rc=access();if(rc)return rc;
    if(!token || token!=device.exit_token || device.busy)return JP_DEVICE_STATE;
    return 0;
}
int jp_device_exit_quiesce(uint64_t token)
{
    int rc=exit_access(token);if(rc)return rc;if(!device.info.resources)return 0;
    device.busy=1;device.info.phase=JP_DEVICE_DRAINING;
    if(device.info.resources&OWN_PRESENT){
        if(jp_present_shutdown()){device.busy=0;return JP_DEVICE_PRESENT;}
        device.info.resources&=~OWN_PRESENT;
    }
    if(device.info.resources&OWN_WAIT){
        if(jp_wait_shutdown()){device.busy=0;return JP_DEVICE_WAIT;}
        device.info.resources&=~OWN_WAIT;
    }
    device.busy=0;return 0;
}
int jp_device_exit_finish(uint64_t token)
{
    int rc=exit_access(token);if(rc)return rc;if(!device.info.resources)return 0;
    rc=jp_device_exit_quiesce(token);if(rc)return rc;
    device.busy=1;rc=drain();device.busy=0;return rc;
}
