/* Guard the genuine main initializer; reuse actual System/sound ownership. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#define JP_GAME_SOUND_API __declspec(dllimport)
#define JP_SOUND_CLOCK_API __declspec(dllimport)
#include "jp_native_main_boot.h"
#define JP_SYSTEM_API __declspec(dllimport)
#include "jp_native_system.h"
#define JP_SYSTEM_SOUND_CLOCK_API __declspec(dllimport)
#include "jp_native_system_sound_clock.h"
#include "jp_main_boot_card_imports.h"
__declspec(dllimport) int save_drive;
__declspec(dllimport) int jp_vmu_store_status(void);
__declspec(dllimport) int jp_vertex_lifecycle_status(void);
__declspec(dllimport) int jp_print_status(void);
__declspec(dllimport) int jp_render_state_check_guards(void);
__declspec(dllimport) int jp_render_state_buffer_is_borrowed(void);
__declspec(dllimport) int jp_queue_status(void);
__declspec(dllimport) int jp_textures_status(void);
__declspec(dllimport) uint32_t jp_textures_live_surfaces(void);
__declspec(dllimport) int jp_wait_status(void);
__declspec(dllimport) int jp_vblank_chain_status(void);
__declspec(dllimport) int buExit(void);
__declspec(dllimport) void buExecServer(void);
__declspec(dllimport) void njExitPrint(void);
void jp_original_system_boot_init(void);
static struct {
    struct jp_sound_options sound;struct jp_sound_clock_options clock;
    struct jp_main_boot_info info;char driver[256];DWORD thread;
    int busy,guard,printed,system_owned;
} boot;
static int access_owner(void)
{return boot.thread&&boot.thread!=GetCurrentThreadId()?JP_MAIN_BOOT_THREAD:0;}
static int result(int code,int native)
{boot.info.status=(uint32_t)code;boot.info.native_error=(uint32_t)native;return code;}
int jp_main_boot_snapshot(struct jp_main_boot_info *out)
{int rc=access_owner();if(rc)return rc;if(!out)return JP_MAIN_BOOT_ARGUMENT;*out=boot.info;return 0;}
int jp_main_boot_configure(const struct jp_sound_options *sound,const struct jp_sound_clock_options *clock)
{
    int rc=access_owner();if(rc)return rc;if(boot.busy)return JP_MAIN_BOOT_REENTRANT;
    if(boot.info.phase==2||boot.info.phase==3||boot.info.card_owned||boot.printed||boot.system_owned)return JP_MAIN_BOOT_STATE;
    if(!sound||sound->bytes!=sizeof *sound||!clock||clock->bytes!=sizeof *clock||clock->activity||clock->activity_context)return JP_MAIN_BOOT_ARGUMENT;
    size_t n=0;if(sound->driver_name){while(n<sizeof boot.driver&&sound->driver_name[n])n++;if(!n||n>=sizeof boot.driver)return JP_MAIN_BOOT_ARGUMENT;}
    boot.sound=*sound;boot.clock=*clock;
    if(n){memcpy(boot.driver,sound->driver_name,n+1);boot.sound.driver_name=boot.driver;}
    boot.thread=GetCurrentThreadId();boot.info.creator_thread=boot.thread;boot.info.phase=1;return result(0,0);
}
/* This is the original parent's actual sound call, not a second SDK producer.
 * The private native parent checks the actual result immediately afterward.
 * No nonlocal stack unwinding or substitute successful initializer is used. */
void sound_system_init(void)
{
    if(access_owner()||!boot.busy||!boot.guard||boot.info.phase!=2)ExitProcess(72);
    /* The original preceding sbInitSystem returned successfully. Only now does
     * this parent acquire cleanup authority over that actual System instance. */
    boot.system_owned=1;
    boot.info.sound_calls++;
    int rc=jp_system_start_sound_clock(&boot.sound,&boot.clock);
    if(rc)result(JP_MAIN_BOOT_SOUND,rc);
}
int jp_main_boot_sound_failed(void){return boot.info.status!=0;}
int jp_main_boot_initialize(void)
{
    int rc=access_owner();if(rc)return rc;if(boot.busy)return JP_MAIN_BOOT_REENTRANT;
    struct jp_system_info system;
    if(boot.info.phase!=1||jp_system_snapshot(&system)||system.phase!=JP_SYSTEM_CONFIGURED||system.resources)return JP_MAIN_BOOT_STATE;
    if(save_drive<0||save_drive>=8)return result(JP_MAIN_BOOT_CARD,JP_MAIN_BOOT_ARGUMENT);
    if((rc=jp_vmu_store_status()))return result(JP_MAIN_BOOT_CARD,rc);
    if(jp_textures_live_surfaces())return result(JP_MAIN_BOOT_RENDER,JP_MAIN_BOOT_STATE);
    struct jp_card_state prior_card;
    if(jp_card_state_read((uint32_t)save_drive,&prior_card)||prior_card.backend_initialized)return result(JP_MAIN_BOOT_CARD,JP_MAIN_BOOT_STATE);
    boot.busy=boot.guard=1;boot.info.phase=2;boot.info.starts++;result(0,0);
    jp_original_system_boot_init();boot.guard=0;
    if(boot.info.status){boot.busy=0;boot.info.phase=4;return (int)boot.info.status;}
    boot.printed=jp_textures_live_surfaces()!=0;boot.info.card_owned=1;
    struct jp_card_state card;
    if(jp_card_state_read((uint32_t)save_drive,&card)||!card.backend_initialized||!jp_card_selected_work_matches()){
        boot.busy=0;boot.info.phase=4;return result(JP_MAIN_BOOT_CARD,0);
    }
    if(jp_vertex_lifecycle_status()||jp_print_status()||jp_render_state_check_guards()||
       !jp_render_state_buffer_is_borrowed()||jp_queue_status()||jp_textures_status()||jp_wait_status()||jp_vblank_chain_status()){
        boot.busy=0;boot.info.phase=4;return result(JP_MAIN_BOOT_RENDER,0);
    }
    boot.info.completions++;boot.info.phase=3;boot.busy=0;return result(0,0);
}
int jp_main_boot_shutdown(uint32_t timeout_ms)
{
    int rc=access_owner();if(rc)return rc;if(boot.busy)return JP_MAIN_BOOT_REENTRANT;
    if(!timeout_ms||timeout_ms>60000)return JP_MAIN_BOOT_ARGUMENT;
    boot.busy=1;
    if(boot.info.card_owned){
        ULONGLONG start=GetTickCount64();
        do{rc=buExit();if(!rc)break;buExecServer();Sleep(1);}while(GetTickCount64()-start<timeout_ms);
        if(rc){boot.busy=0;return result(JP_MAIN_BOOT_CLEANUP,rc);}boot.info.card_owned=0;
    }
    if(boot.system_owned){
        rc=jp_system_shutdown();if(rc){boot.busy=0;return result(JP_MAIN_BOOT_CLEANUP,rc);}boot.system_owned=0;
    }
    if(boot.printed){
        uint32_t before=jp_textures_live_surfaces();
        if(before){
            njExitPrint();
            if(jp_textures_live_surfaces()>=before){
                boot.busy=0;int error=jp_textures_status();return result(JP_MAIN_BOOT_CLEANUP,error?error:JP_MAIN_BOOT_RENDER);
            }
        }
        boot.printed=0;
    }
    boot.info.shutdowns++;boot.info.phase=0;boot.busy=0;return result(0,0);
}
void system_boot_init(void)
{
    if(jp_main_boot_initialize()){
        (void)jp_main_boot_shutdown(5000);ExitProcess(72);
    }
}
