#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "jp_native_history.h"
#define JP_RTC_API __declspec(dllimport)
#include "jp_native_rtc.h"
#define JP_FLASH_API __declspec(dllimport)
#include "jp_native_flash_history.h"
__declspec(dllimport) int jp_rtc_status(void);
__declspec(dllimport) int jp_input_status(void);
void jp_history_source_init(void);
void jp_history_source_write(void);
void jp_history_source_read(uint16_t *,uint32_t *);
void jp_history_source_count_play(int);
void jp_history_source_count_clear(int);
int jp_history_source_seed(void);
int jp_history_source_ready(void);
int jp_history_source_recover(void);
int jp_history_source_refresh(void);
void jp_history_source_reset(void);
int jp_history_source_identity_valid(void);
static struct {
    wchar_t directory[32768],track[32768];char header[256];
    struct jp_history_info info;uint32_t sector,create;DWORD thread;
    void *work3,*work4;int busy,source_live,created;
} history;
static int access(void)
{return history.thread&&history.thread!=GetCurrentThreadId()?JP_HISTORY_THREAD:history.busy?JP_HISTORY_REENTRANT:0;}
static int result(int code,int native)
{history.info.status=(uint32_t)code;history.info.native_error=(uint32_t)native;return code;}
void jp_history_source_problem(uint32_t code,uint32_t native)
{if(!history.info.status)result((int)code,(int)native);}
const char *jp_history_boot_header(void){return history.header;}
void *jp_history_work(uint32_t kind){return kind==2?history.work3:kind==3?history.work4:NULL;}
int jp_history_source_begin_flash(void)
{
    if(!history.busy||history.thread!=GetCurrentThreadId())return JP_HISTORY_STATE;
    int rc=jp_flash_history_enter(history.info.flash_token);
    if(rc)jp_history_source_problem(JP_HISTORY_FLASH,(uint32_t)rc);return rc;
}
void jp_history_source_end_flash(void)
{int rc=jp_flash_history_leave(history.info.flash_token);if(rc)jp_history_source_problem(JP_HISTORY_FLASH,(uint32_t)rc);}
int jp_history_snapshot(struct jp_history_info *out)
{
    if(history.thread&&history.thread!=GetCurrentThreadId())return JP_HISTORY_THREAD;
    if(!out)return JP_HISTORY_ARGUMENT;*out=history.info;return 0;
}
int jp_history_configure(const struct jp_history_options *opt)
{
    int rc=access();if(rc)return rc;
    if(history.info.resources||history.source_live)return JP_HISTORY_STATE;
    if(!opt||opt->bytes!=sizeof *opt||opt->create_if_missing>1||!opt->directory||!opt->track||opt->reserved||
       (opt->raw_sector_bytes!=2048&&opt->raw_sector_bytes!=2352))return JP_HISTORY_ARGUMENT;
    wchar_t directory[32768],track[32768];DWORD n=GetFullPathNameW(opt->directory,32768,directory,NULL);
    if(!n||n>=32768)return JP_HISTORY_ARGUMENT;DWORD m=GetFullPathNameW(opt->track,32768,track,NULL);
    if(!m||m>=32768)return JP_HISTORY_ARGUMENT;
    memcpy(history.directory,directory,(n+1)*sizeof *directory);memcpy(history.track,track,(m+1)*sizeof *track);
    history.create=opt->create_if_missing;history.sector=opt->raw_sector_bytes;history.thread=GetCurrentThreadId();
    history.info.creator_thread=history.thread;history.info.phase=JP_HISTORY_CONFIGURED;return result(0,0);
}
static int load_identity(void)
{
    HANDLE h=CreateFileW(history.track,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if(h==INVALID_HANDLE_VALUE)return result(JP_HISTORY_IDENTITY,(int)GetLastError());
    LARGE_INTEGER at;at.QuadPart=history.sector==2352?16:0;DWORD got=0;char header[256];int rc=0;
    if(!SetFilePointerEx(h,at,NULL,FILE_BEGIN)||!ReadFile(h,header,sizeof header,&got,NULL))rc=(int)GetLastError();
    else if(got!=sizeof header)rc=ERROR_HANDLE_EOF;
    if(!CloseHandle(h)&&!rc)rc=(int)GetLastError();
    if(rc)return result(JP_HISTORY_IDENTITY,rc);
    if(memcmp(header,"SEGA SEGAKATANA ",16))return result(JP_HISTORY_IDENTITY,0);
    for(unsigned i=43;i<48;i++)if(header[i]!='/'&&header[i]!=' '&&(header[i]<'0'||header[i]>'9'))return result(JP_HISTORY_IDENTITY,0);
    for(unsigned i=64;i<74;i++)if((unsigned char)header[i]<32||(unsigned char)header[i]>126)return result(JP_HISTORY_IDENTITY,0);
    memcpy(history.header,header,sizeof header);
    return jp_history_source_identity_valid()?0:result(JP_HISTORY_IDENTITY,0);
}
int jp_history_initialize(void)
{
    int rc=access();if(rc)return rc;
    if((history.info.phase!=JP_HISTORY_CONFIGURED&&history.info.phase!=JP_HISTORY_FAILED)||history.source_live)return JP_HISTORY_STATE;
    result(0,0);struct jp_rtc_info rtc_state;
    rc=jp_rtc_snapshot(&rtc_state);
    if(rc||!rtc_state.initialized)return result(JP_HISTORY_RTC,rc?rc:JP_RTC_NOT_INITIALIZED);
    if((rc=jp_rtc_status()))return result(JP_HISTORY_RTC,rc);
    if((rc=jp_input_status()))return result(JP_HISTORY_INPUT,rc);
    if(load_identity())return (int)history.info.status;
    history.busy=1;history.info.phase=JP_HISTORY_STARTING;history.info.starts++;
    if(!(history.info.resources&1)){
        rc=jp_flash_open(history.directory,history.create);if(rc)goto flash_failed;
        history.info.resources|=1;
    }
    if(!history.info.flash_token){
        rc=jp_flash_history_claim(&history.info.flash_token);if(rc)goto flash_failed;
        struct jp_flash_history_info state;rc=jp_flash_history_snapshot(&state);if(rc)goto flash_failed;
        history.created=(int)state.created_profile;
    }
    if(!history.work3)history.work3=calloc(1,65536);
    if(!history.work4)history.work4=calloc(1,32768);
    if(!history.work3||!history.work4){result(JP_HISTORY_MEMORY,0);goto failed;}
    history.info.resources|=2;history.info.work3=(uintptr_t)history.work3;history.info.work4=(uintptr_t)history.work4;
    if(!jp_history_source_begin_flash()){jp_flash_clear_error();jp_history_source_end_flash();}
    if(history.info.status)goto failed;
    rc=jp_flash_history_transaction_begin(history.info.flash_token);if(rc)goto flash_failed;
    if(history.created&&!jp_history_source_seed()){
        if(!history.info.status)result(JP_HISTORY_SOURCE,0);
        (void)jp_flash_history_transaction_abort(history.info.flash_token);goto failed;
    }
    jp_history_source_init();
    if(!history.info.status&&!jp_history_source_ready())result(JP_HISTORY_SOURCE,0);
    if(history.info.status){(void)jp_flash_history_transaction_abort(history.info.flash_token);goto failed;}
    rc=jp_flash_history_transaction_commit(history.info.flash_token);
    if(rc){(void)jp_flash_history_transaction_abort(history.info.flash_token);goto flash_failed;}
    history.source_live=1;history.created=0;history.info.dirty=1;history.info.phase=JP_HISTORY_READY;history.busy=0;return 0;
flash_failed:
    result(JP_HISTORY_FLASH,rc);
failed:
    history.info.phase=JP_HISTORY_FAILED;history.busy=0;return (int)history.info.status;
}
static int save_owned(void)
{
    result(0,0);int rc=jp_flash_history_transaction_begin(history.info.flash_token);
    if(rc)return result(JP_HISTORY_FLASH,rc);
    jp_history_source_write();
    if(history.info.status){(void)jp_flash_history_transaction_abort(history.info.flash_token);return (int)history.info.status;}
    rc=jp_flash_history_transaction_commit(history.info.flash_token);
    if(rc){(void)jp_flash_history_transaction_abort(history.info.flash_token);return result(JP_HISTORY_FLASH,rc);}
    history.info.saves++;history.info.dirty=0;return 0;
}
int jp_history_save(void)
{
    int rc=access();if(rc)return rc;
    if(!history.source_live||history.info.phase!=JP_HISTORY_READY)return JP_HISTORY_STATE;
    history.busy=1;history.info.dirty=1;rc=save_owned();history.busy=0;return rc;
}
int jp_history_read(uint16_t times[24],uint32_t starts[3])
{
    int rc=access();if(rc)return rc;if(!times||!starts)return JP_HISTORY_ARGUMENT;
    uintptr_t a=(uintptr_t)times,b=(uintptr_t)starts;
    if(a>UINTPTR_MAX-48||b>UINTPTR_MAX-12||(a<=b?b-a<48:a-b<12))return JP_HISTORY_ARGUMENT;
    if(!history.source_live||history.info.phase!=JP_HISTORY_READY)return JP_HISTORY_STATE;
    uint16_t t[24]={0};uint32_t s[3]={0};history.busy=1;result(0,0);
    /* The original read reinitializes UF from disk. Preserve pending live
     * counts/minutes before that actual reload rather than silently lose them. */
    if(history.info.dirty){rc=save_owned();if(rc){history.busy=0;return rc;}}
    rc=jp_flash_history_transaction_begin(history.info.flash_token);
    if(rc){history.busy=0;return result(JP_HISTORY_FLASH,rc);}
    jp_history_source_read(t,s);rc=(int)history.info.status;
    if(!rc){rc=jp_flash_history_transaction_commit(history.info.flash_token);if(rc)result(JP_HISTORY_FLASH,rc);}
    if(rc)(void)jp_flash_history_transaction_abort(history.info.flash_token);
    if(!rc){memcpy(times,t,sizeof t);memcpy(starts,s,sizeof s);}
    else rc=(int)history.info.status;
    history.busy=0;return rc;
}
static int count(int32_t character,int clear)
{
    int rc=access();if(rc)return rc;if(!history.source_live||history.info.phase!=JP_HISTORY_READY)return JP_HISTORY_STATE;
    history.busy=1;if(clear)jp_history_source_count_clear(character);else jp_history_source_count_play(character);
    history.busy=0;return 0;
}
int jp_history_count_play(int32_t character){return count(character,0);}
int jp_history_count_clear(int32_t character){return count(character,1);}
int jp_history_peer_begin(int dirty)
{
    int rc=access();if(rc)return rc;
    if(!history.source_live||history.info.phase!=JP_HISTORY_READY)return JP_HISTORY_STATE;
    history.busy=1;result(0,0);if(dirty)history.info.dirty=1;return 0;
}
int jp_history_peer_end(void)
{int rc=(int)history.info.status;history.busy=0;return rc;}
int jp_history_recover(void)
{
    int rc=access();if(rc)return rc;if(!history.source_live)return JP_HISTORY_STATE;
    history.busy=1;result(0,0);
    if(!jp_history_source_begin_flash()){jp_flash_clear_error();jp_history_source_end_flash();}
    if(!history.info.status){
        rc=jp_flash_history_transaction_begin(history.info.flash_token);
        if(rc)result(JP_HISTORY_FLASH,rc);
        else{
            if(!jp_history_source_recover()&&!history.info.status)result(JP_HISTORY_SOURCE,0);
            if(!history.info.status){rc=jp_flash_history_transaction_commit(history.info.flash_token);if(rc)result(JP_HISTORY_FLASH,rc);}
            if(history.info.status)(void)jp_flash_history_transaction_abort(history.info.flash_token);
        }
    }
    rc=(int)history.info.status;history.busy=0;return rc;
}
int jp_history_refresh(void)
{
    int rc=access();if(rc)return rc;
    if(!history.source_live||history.info.phase!=JP_HISTORY_READY)return JP_HISTORY_STATE;
    history.busy=1;result(0,0);rc=jp_flash_history_transaction_begin(history.info.flash_token);
    if(rc)result(JP_HISTORY_FLASH,rc);
    else{
        if(!jp_history_source_refresh()&&!history.info.status)result(JP_HISTORY_SOURCE,0);
        if(!history.info.status){rc=jp_flash_history_transaction_commit(history.info.flash_token);if(rc)result(JP_HISTORY_FLASH,rc);}
        if(history.info.status)(void)jp_flash_history_transaction_abort(history.info.flash_token);
    }
    rc=(int)history.info.status;history.busy=0;return rc;
}
int jp_history_shutdown(void)
{
    int rc=access();if(rc)return rc;if(!history.info.resources)return 0;
    history.busy=1;
    if(history.source_live&&history.info.dirty){history.info.phase=JP_HISTORY_DRAINING;rc=save_owned();if(rc){history.busy=0;return rc;}}
    result(0,0);if(history.info.flash_token)jp_history_source_reset();
    if(history.info.status){history.busy=0;return (int)history.info.status;}
    history.source_live=0;
    if(history.info.flash_token){rc=jp_flash_history_cancel(history.info.flash_token);if(rc){history.busy=0;return result(JP_HISTORY_FLASH,rc);}history.info.flash_token=0;}
    rc=jp_flash_close();if(rc){history.busy=0;return result(JP_HISTORY_FLASH,rc);}
    free(history.work3);free(history.work4);history.work3=history.work4=NULL;
    history.info.work3=history.info.work4=0;history.info.resources=0;history.info.closes++;
    history.info.phase=JP_HISTORY_CONFIGURED;history.busy=0;return result(0,0);
}
