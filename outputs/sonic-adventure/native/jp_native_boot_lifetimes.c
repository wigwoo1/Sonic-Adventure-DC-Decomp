/* Actual card/font lifetime authorities, linked beside their existing state. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include "jp_native_boot_lifetimes.h"
#include "jp_native_card_state.h"
#include "jp_native_fibers.h"
extern int buExit(void);
extern void buExecServer(void);
extern void njExitPrint(void);
extern int jp_textures_status(void);
static struct {
    struct jp_boot_life_info info;DWORD provider_thread;
    uint64_t provider_generation;int call;
} lifetimes[2];
static int access_owner(uint32_t kind)
{
    if(kind>1)return JP_BOOT_LIFE_ARGUMENT;
    DWORD thread=lifetimes[kind].info.creator_thread;
    return thread&&thread!=GetCurrentThreadId()?JP_BOOT_LIFE_THREAD:0;
}
static int result(uint32_t kind,int code,int native)
{lifetimes[kind].info.status=(uint32_t)code;lifetimes[kind].info.native_error=(uint32_t)native;return code;}
void jp_boot_lifetime_created(uint32_t kind)
{
    if(kind>1)return;
    lifetimes[kind].provider_thread=GetCurrentThreadId();
    lifetimes[kind].provider_generation++;
}
int jp_boot_lifetime_guard(uint32_t kind,uint32_t op)
{
    if(kind>1||op>3)return JP_BOOT_LIFE_ARGUMENT;
    if(lifetimes[kind].info.token||lifetimes[kind].info.terminal){
        int rc=access_owner(kind);if(rc)return rc;
        if(lifetimes[kind].call)return 0;
        if(op==1&&lifetimes[kind].info.finished)return 0;
        if(op==3&&!lifetimes[kind].info.closing&&!lifetimes[kind].info.terminal)return 0;
        return JP_BOOT_LIFE_STATE;
    }
    return 0;
}
int jp_boot_lifetime_can_close(uint32_t kind,uint64_t token)
{
    int rc=access_owner(kind);if(rc)return rc;
    if(!token||lifetimes[kind].info.token!=token)return JP_BOOT_LIFE_STATE;
    if(lifetimes[kind].call||(kind==0?jp_boot_card_active():jp_boot_print_active()))return JP_BOOT_LIFE_REENTRANT;
    return 0;
}
int jp_boot_lifetime_snapshot(uint32_t kind,struct jp_boot_life_info *out)
{
    int rc=access_owner(kind);if(rc)return rc;
    if(!out)return JP_BOOT_LIFE_ARGUMENT;*out=lifetimes[kind].info;return 0;
}
int jp_boot_lifetime_claim(uint32_t kind,uint64_t *out)
{
    int rc=access_owner(kind);if(rc)return rc;
    if(!out)return JP_BOOT_LIFE_ARGUMENT;
    if(!lifetimes[kind].provider_thread)return JP_BOOT_LIFE_STATE;
    if(lifetimes[kind].provider_thread!=GetCurrentThreadId())return JP_BOOT_LIFE_THREAD;
    if(lifetimes[kind].info.token||lifetimes[kind].info.terminal||lifetimes[kind].info.next_token==UINT64_MAX)return JP_BOOT_LIFE_STATE;
    if(kind==0?jp_boot_card_active():jp_boot_print_active())return JP_BOOT_LIFE_REENTRANT;
    uint64_t identity=0;
    if(kind==0){
        struct jp_card_state card;struct jp_rf_info fibers;
        if(jp_card_state_read(0,&card)||jp_rf_snapshot(&fibers))return JP_BOOT_LIFE_PROVIDER;
        if(!card.backend_initialized&&!fibers.servers&&!fibers.fibers)return JP_BOOT_LIFE_STATE;
        if(fibers.servers>1)return JP_BOOT_LIFE_STATE;
        identity=card.original_work_address;
    }else if(!jp_boot_font_identity(&identity))return JP_BOOT_LIFE_STATE;
    lifetimes[kind].info.creator_thread=GetCurrentThreadId();
    lifetimes[kind].info.token=++lifetimes[kind].info.next_token;
    lifetimes[kind].info.identity=identity;lifetimes[kind].info.generation=lifetimes[kind].provider_generation;
    lifetimes[kind].info.closing=lifetimes[kind].info.finished=0;
    *out=lifetimes[kind].info.token;return result(kind,0,0);
}
int jp_boot_lifetime_cancel(uint32_t kind,uint64_t token)
{
    int rc=jp_boot_lifetime_can_close(kind,token);if(rc)return rc;
    if(lifetimes[kind].info.terminal||(lifetimes[kind].info.closing&&!lifetimes[kind].info.finished))return JP_BOOT_LIFE_STATE;
    lifetimes[kind].info.token=0;lifetimes[kind].info.closing=lifetimes[kind].info.finished=0;return result(kind,0,0);
}
static int close_resource(uint32_t kind,uint64_t token,uint32_t timeout,int terminal)
{
    int rc=jp_boot_lifetime_can_close(kind,token);if(rc)return rc;
    if(timeout>60000)return JP_BOOT_LIFE_ARGUMENT;
    if(!terminal&&lifetimes[kind].info.terminal)return JP_BOOT_LIFE_STATE;
    if(lifetimes[kind].info.generation!=lifetimes[kind].provider_generation)return JP_BOOT_LIFE_STATE;
    if(terminal)lifetimes[kind].info.terminal=1;
    if(lifetimes[kind].info.finished)return 0;
    lifetimes[kind].info.closing=1;lifetimes[kind].call=1;
    int native=0;
    if(kind==0){
        ULONGLONG start=GetTickCount64();
        for(;;){
            native=buExit();
            if(!native){
                struct jp_rf_info fibers;
                native=jp_rf_snapshot(&fibers);
                if(!native&&(fibers.servers||fibers.fibers||!jp_boot_rf_retired())){
                    rfExitSystem();native=jp_rf_snapshot(&fibers);
                    if(!native&&(fibers.servers||fibers.fibers||!jp_boot_rf_retired()))native=JP_RF_STATE;
                }
                rc=native?JP_BOOT_LIFE_PROVIDER:0;break;
            }
            if(GetTickCount64()-start>=timeout){rc=JP_BOOT_LIFE_TIMEOUT;break;}
            buExecServer();Sleep(1);
        }
    }else{
        uint64_t identity=0;
        if(!jp_boot_font_identity(&identity)||identity!=lifetimes[kind].info.identity)rc=JP_BOOT_LIFE_STATE;
        else{
            njExitPrint();
            if(jp_boot_font_identity(&identity)){native=jp_textures_status();rc=JP_BOOT_LIFE_PROVIDER;}
            else rc=0;
        }
    }
    lifetimes[kind].call=0;
    if(!rc)lifetimes[kind].info.finished=1;
    return result(kind,rc,native);
}
int jp_boot_lifetime_close(uint32_t kind,uint64_t token,uint32_t timeout)
{return close_resource(kind,token,timeout,0);}
int jp_boot_lifetime_finish(uint32_t kind,uint64_t token,uint32_t timeout)
{return close_resource(kind,token,timeout,1);}
