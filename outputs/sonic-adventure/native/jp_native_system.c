/* Real compiled host replacement for the pinned original sbInitSystem order. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#define JP_DEVICE_API __declspec(dllimport)
#include "jp_native_system.h"
#define JP_SDK_HEAP_API __declspec(dllimport)
#include "jp_native_sdk_heap.h"
#define JP_RTC_API __declspec(dllimport)
#include "jp_native_rtc.h"
#define JP_GDFS_API __declspec(dllimport)
#include "jp_native_gdfs.h"
#define JP_EXIT_API __declspec(dllimport)
#include "jp_native_exit.h"
/* Exact input ABI, without re-exporting its existing owner. The input sample
 * tag is shared with jp_native_input.h; callbacks have the same pointer type. */
__declspec(dllimport) int jp_input_configure(void *,uint32_t,jp_system_input_source,void *);
__declspec(dllimport) int jp_input_use_virtual_cards(void);
__declspec(dllimport) int jp_input_bootstrap_status(void);
__declspec(dllimport) int jp_input_status(void);
__declspec(dllimport) int jp_input_poll(void);
__declspec(dllimport) int jp_input_shutdown(void);
__declspec(dllimport) void pdInitPeripheral(long,void *,void *);
__declspec(dllimport) long gdFsChangeDir(const char *);
__declspec(dllimport) void *jp_window_native_handle(struct jp_window *);
enum {CORE=JP_EXIT_SDK_HEAP|JP_EXIT_DEVICE|JP_EXIT_INPUT|JP_EXIT_RTC|JP_EXIT_GDFS};
static struct {
    struct jp_system_options options;struct jp_system_info info;
    wchar_t path[32768];DWORD thread;int busy,registered,terminal;
} system_owner={.info={.status=JP_SYSTEM_STATE}};
static int access_owner(void)
{return system_owner.thread&&system_owner.thread!=GetCurrentThreadId()?JP_SYSTEM_THREAD:0;}
static int result(int code,uint32_t resource,int native)
{system_owner.info.status=(uint32_t)code;system_owner.info.failed_resource=resource;system_owner.info.native_error=(uint32_t)native;return code;}
static int close_rtc(void)
{
    struct jp_rtc_info info;int rc=jp_rtc_snapshot(&info);if(rc)return rc;
    if(info.initialized){rc=jp_rtc_shutdown();if(rc)return rc;}
    else if(info.chain)return JP_RTC_CHAIN;
    return jp_rtc_configure(NULL,NULL);
}
static int shutdown_owned(void)
{
    system_owner.info.phase=JP_SYSTEM_DRAINING;system_owner.info.cleanups++;
#define CLOSE(bit,call) do{if(system_owner.info.resources&(bit)){int rc=(call);if(rc)return result(JP_SYSTEM_CLEANUP,(bit),rc);system_owner.info.resources&=~(bit);}}while(0)
    CLOSE(JP_EXIT_GDFS,jp_gdfs_shutdown());
    CLOSE(JP_EXIT_RTC,close_rtc());
    CLOSE(JP_EXIT_INPUT,jp_input_shutdown());
    CLOSE(JP_EXIT_DEVICE,jp_device_shutdown());
    CLOSE(JP_EXIT_SDK_HEAP,jp_sdk_heap_shutdown());
#undef CLOSE
    system_owner.info.window=0;system_owner.info.phase=system_owner.terminal?JP_SYSTEM_CLOSED:JP_SYSTEM_CONFIGURED;
    return result(0,0,0);
}
static int bootstrap_cleanup(void *context)
{
    (void)context;int rc=access_owner();if(rc)return rc;
    if(system_owner.busy)return JP_SYSTEM_REENTRANT;
    system_owner.terminal=1;system_owner.busy=1;rc=shutdown_owned();system_owner.busy=0;return rc;
}
int jp_system_configure(const struct jp_system_options *options)
{
    int rc=access_owner();if(rc)return rc;
    if(system_owner.busy)return JP_SYSTEM_REENTRANT;
    if(system_owner.terminal||system_owner.info.resources||system_owner.registered||system_owner.info.phase==JP_SYSTEM_DELEGATED)return JP_SYSTEM_STATE;
    if(!options||options->bytes!=sizeof *options||!options->track||!*options->track||options->reserved||
       (options->raw_sector_bytes!=2048&&options->raw_sector_bytes!=2352)||!options->chunk_sectors||options->chunk_sectors>65536||
       !options->timeout_ms||options->timeout_ms>60000||(options->input_flags&~3u)||
       (!options->input_source&&(!options->input_flags||options->input_context))||(!options->rtc_reader&&options->rtc_context))return JP_SYSTEM_ARGUMENT;
    const struct jp_device_options *d=&options->device;
    if(!d->render_width||!d->render_height||d->render_width>8192||d->render_height>8192||d->visible>1||d->present_interval>1||
       !d->timeout_ms||d->timeout_ms>60000||d->reserved||!d->gpu_budget||d->gpu_budget>SIZE_MAX)return JP_SYSTEM_ARGUMENT;
    wchar_t path[32768];DWORD n=GetFullPathNameW(options->track,32768,path,NULL);if(!n||n>=32768)return JP_SYSTEM_ARGUMENT;
    system_owner.options=*options;memcpy(system_owner.path,path,((size_t)n+1)*sizeof(wchar_t));system_owner.options.track=system_owner.path;
    system_owner.thread=GetCurrentThreadId();system_owner.info.phase=JP_SYSTEM_CONFIGURED;return result(0,0,0);
}
static int cold_owners(void)
{
    struct jp_device_info d;struct jp_rtc_info rtc;struct jp_gdfs_info fs;struct jp_sdk_heap_info heap;
    int rc=jp_sdk_heap_snapshot(&heap);
    if((rc&&rc!=JP_SDK_HEAP_NOT_INITIALIZED)||(!rc&&heap.initialized))return 0;
    if(jp_device_snapshot(&d)||d.phase||d.resources||jp_rtc_snapshot(&rtc)||rtc.initialized||
       jp_gdfs_snapshot(&fs)||fs.phase>JP_GDFS_CONFIGURED||jp_input_status()!=4||jp_exit_status()!=JP_EXIT_NOT_BOUND)return 0;
    return 1;
}
int jp_system_initialize(int32_t mode,int32_t frame,int32_t count)
{
    int rc=access_owner();if(rc)return rc;if(system_owner.busy)return JP_SYSTEM_REENTRANT;
    if(system_owner.terminal||system_owner.info.phase!=JP_SYSTEM_CONFIGURED||system_owner.info.resources||system_owner.registered)return JP_SYSTEM_STATE;
    if(!cold_owners())return result(JP_SYSTEM_STATE,0,0);
    system_owner.busy=1;system_owner.info.phase=JP_SYSTEM_STARTING;system_owner.info.mode=mode;system_owner.info.frame=frame;system_owner.info.count=count;system_owner.info.attempts=0;
    uint32_t failure=JP_EXIT_SDK_HEAP;int error=JP_SYSTEM_HEAP;
    rc=jp_sdk_heap_create((1u<<20)+32);if(rc)goto failed;system_owner.info.resources|=JP_EXIT_SDK_HEAP;
    failure=JP_EXIT_DEVICE;error=JP_SYSTEM_DEVICE;rc=jp_device_configure(&system_owner.options.device);if(rc)goto failed;
    system_owner.info.resources|=JP_EXIT_DEVICE;rc=jp_device_initialize(mode,frame,count);if(rc)goto failed;
    struct jp_window *window=jp_device_window();void *hwnd=jp_window_native_handle(window);
    if(!hwnd){rc=JP_DEVICE_WINDOW;goto failed;}system_owner.info.window=(uint64_t)(uintptr_t)hwnd;
    failure=JP_EXIT_INPUT;error=JP_SYSTEM_INPUT;
    rc=jp_input_configure(hwnd,system_owner.options.input_flags,system_owner.options.input_source,system_owner.options.input_context);if(rc)goto failed;
    system_owner.info.resources|=JP_EXIT_INPUT;rc=jp_input_use_virtual_cards();if(rc)goto failed;
    pdInitPeripheral(0,NULL,NULL);rc=jp_input_bootstrap_status();if(rc)goto failed;
    rc=jp_input_poll();if(rc)goto failed;
    failure=JP_EXIT_RTC;error=JP_SYSTEM_RTC;rc=jp_rtc_configure(system_owner.options.rtc_reader,system_owner.options.rtc_context);if(rc)goto failed;
    system_owner.info.resources|=JP_EXIT_RTC;rc=(int)syRtcInit();if(rc){rc=jp_rtc_status();goto failed;}
    failure=JP_EXIT_GDFS;error=JP_SYSTEM_GDFS;
    rc=jp_gdfs_configure(system_owner.path,45150,system_owner.options.raw_sector_bytes,system_owner.options.chunk_sectors,system_owner.options.timeout_ms);if(rc)goto failed;
    system_owner.info.resources|=JP_EXIT_GDFS;
    for(unsigned tries=0;tries<8;tries++){
        system_owner.info.attempts++;rc=jp_gdfs_initialize(8,1920);if(!rc)break;
        struct jp_gdfs_info fs;if(jp_gdfs_snapshot(&fs)||fs.source_error==-23||fs.source_error==-33)break;
        int closed=jp_gdfs_shutdown();if(closed){rc=closed;break;}
    }
    if(rc)goto failed;
    error=JP_SYSTEM_DIRECTORY;rc=(int)gdFsChangeDir("SONICADV");if(rc)goto failed;
    failure=0;error=JP_SYSTEM_EXIT;rc=jp_exit_prepare_bootstrap(CORE,bootstrap_cleanup,&system_owner);if(rc)goto failed;
    system_owner.registered=1;system_owner.info.initializations++;system_owner.info.phase=JP_SYSTEM_READY;system_owner.busy=0;return result(0,0,0);
failed:
    {int native=rc;int cleanup=shutdown_owned();system_owner.busy=0;
     if(cleanup)return cleanup;system_owner.info.phase=JP_SYSTEM_FAILED;return result(error,failure,native);}
}
int jp_system_bind_exit(uint32_t resources)
{
    int rc=access_owner();if(rc)return rc;if(system_owner.busy)return JP_SYSTEM_REENTRANT;
    if(system_owner.info.phase!=JP_SYSTEM_READY||!system_owner.registered||system_owner.terminal)return JP_SYSTEM_STATE;
    struct jp_exit_config cfg={sizeof cfg,resources,NULL,NULL};rc=jp_exit_bind(&cfg);
    if(rc)return result(JP_SYSTEM_EXIT,0,rc);
    system_owner.info.resources=0;system_owner.registered=0;system_owner.terminal=1;system_owner.info.phase=JP_SYSTEM_DELEGATED;return result(0,0,0);
}
int jp_system_shutdown(void)
{
    int rc=access_owner();if(rc)return rc;if(system_owner.busy)return JP_SYSTEM_REENTRANT;
    if(system_owner.info.phase==JP_SYSTEM_COLD)return 0;
    if(system_owner.info.phase==JP_SYSTEM_DELEGATED){
        rc=jp_exit_cleanup();if(rc)return result(JP_SYSTEM_CLEANUP,0,rc);
        system_owner.info.phase=JP_SYSTEM_CLOSED;system_owner.info.window=0;return result(0,0,0);
    }
    system_owner.busy=1;rc=shutdown_owned();system_owner.busy=0;if(rc)return rc;
    if(system_owner.registered&&!system_owner.terminal){
        rc=jp_exit_cancel_bootstrap(bootstrap_cleanup,&system_owner);if(rc)return result(JP_SYSTEM_EXIT,0,rc);system_owner.registered=0;
    }
    return 0;
}
int jp_system_status(void){int rc=access_owner();return rc?rc:(int)system_owner.info.status;}
int jp_system_snapshot(struct jp_system_info *out)
{
    int rc=access_owner();if(rc)return rc;if(!out)return JP_SYSTEM_ARGUMENT;
    if(system_owner.info.phase==JP_SYSTEM_DELEGATED){struct jp_exit_info state;if(!jp_exit_snapshot(&state)&&!state.remaining){system_owner.info.phase=JP_SYSTEM_CLOSED;system_owner.info.window=0;}}
    *out=system_owner.info;return 0;
}
void sbInitSystem(int32_t mode,int32_t frame,int32_t count)
{int rc=jp_system_initialize(mode,frame,count);if(rc){(void)jp_system_shutdown();ExitProcess(64u+(unsigned)rc);}}
