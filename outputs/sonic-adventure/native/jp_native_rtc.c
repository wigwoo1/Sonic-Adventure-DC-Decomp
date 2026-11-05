#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include "jp_native_rtc_internal.h"
typedef void (*chain_callback)(void *);
struct chain_record {uint32_t priority,next;uintptr_t callback,argument;};
__declspec(dllimport) unsigned long syChainAddHandler(short,chain_callback,unsigned long,void *);
__declspec(dllimport) void syChainDeleteHandler(unsigned long);
__declspec(dllimport) int jp_vblank_chain_status(void);
__declspec(dllimport) int jp_vblank_chain_describe(uint32_t,struct chain_record *);
static struct {
    struct jp_rtc_info info;
    jp_rtc_reader reader;void *context;
    DWORD owner;int busy,exit_started;uint64_t exit_token;
} rtc;
static uint64_t next_exit_token=1;
static int fail(int code){rtc.info.status=(uint32_t)code;return code;}
static int access(int live)
{
    if(rtc.owner && rtc.owner!=GetCurrentThreadId())return JP_RTC_THREAD;
    if(rtc.busy)return JP_RTC_REENTRANT;
    if(live && !rtc.info.initialized)return fail(JP_RTC_NOT_INITIALIZED);
    return 0;
}
static int overlap(const void *a,size_t an,const void *b,size_t bn)
{uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;return x<=y?y-x<an:x-y<bn;}
static int count_for(const struct jp_rtc_date *d,unsigned long *out)
{
    static const uint8_t days[]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(!d||!out)return JP_RTC_ARGUMENT;
    if(d->year<1950||d->year>2086||d->month<1||d->month>12||
       !d->day||d->day>days[d->month-1]+(d->month==2 && d->year%4==0)||
       d->hour>23||d->minute>59||d->second>59)return JP_RTC_RANGE;
    unsigned long count;jp_rtc_source_to_count(d,&count);
    struct jp_rtc_date back={0};jp_rtc_source_to_date(count,&back);
    if(back.year!=d->year||back.month!=d->month||back.day!=d->day||
       back.hour!=d->hour||back.minute!=d->minute||back.second!=d->second)return JP_RTC_RANGE;
    *out=count;return 0;
}
static int windows_read(void *unused,uint32_t *count)
{
    (void)unused;SYSTEMTIME time;GetLocalTime(&time);
    struct jp_rtc_date date={time.wYear,(uint8_t)time.wMonth,(uint8_t)time.wDay,
        (uint8_t)time.wHour,(uint8_t)time.wMinute,(uint8_t)time.wSecond,0,0};
    unsigned long value;int rc=count_for(&date,&value);if(!rc)*count=(uint32_t)value;return rc;
}
static int raw_read(uint32_t *value)
{
    uint32_t count=0;rtc.info.reads++;
    int rc=(rtc.reader?rtc.reader:windows_read)(rtc.context,&count);
    if(rc){rtc.info.failures++;rtc.info.read_error=(uint32_t)rc;return fail(JP_RTC_HOST);}
    *value=count;return 0;
}
long jp_rtc_host_read(unsigned long *out)
{
    uint32_t count;if(raw_read(&count))return -1;
    int64_t value=(int64_t)count+rtc.info.offset;
    if(value<0||value>UINT32_MAX){rtc.info.failures++;fail(JP_RTC_RANGE);return -1;}
    *out=(unsigned long)value;return 0;
}
unsigned long jp_rtc_bounded_force(void)
{
    unsigned long previous=0,current=0;unsigned consecutive=0;
    /* Four equal successful readings, as originally intended. Unlike the
     * console bug, a changed reading becomes the new baseline. Sixteen total
     * attempts bound hardware/provider failures and clocks that never settle. */
    for(unsigned i=0;i<16;i++){
        if(jp_rtc_host_read(&current)){consecutive=0;continue;}
        if(consecutive && previous==current)consecutive++;else consecutive=1;
        previous=current;
        if(consecutive==4){rtc.info.status=rtc.info.read_error=0;return current;}
    }
    if(rtc.info.status==JP_RTC_OK)fail(JP_RTC_UNSTABLE);
    return sygRtcCount;
}
static void tick(void *unused);
static int linked(void)
{
    struct chain_record record;
    int rc=jp_vblank_chain_describe(rtc.info.chain,&record);
    return !rc && (record.priority&UINT32_C(0x7fffffff))==92 &&
        record.callback==(uintptr_t)tick && !record.argument;
}
static void cache_info(void)
{rtc.info.count=(uint32_t)sygRtcCount;rtc.info.mode=(uint32_t)sygRtcStat;}
int jp_rtc_configure(jp_rtc_reader reader,void *context)
{
    int rc=access(0);if(rc)return rc;
    if(rtc.info.initialized||rtc.exit_token)return fail(JP_RTC_STATE);
    if(!reader&&context)return fail(JP_RTC_ARGUMENT);
    memset(&rtc,0,sizeof rtc);rtc.reader=reader;rtc.context=context;return 0;
}
long syRtcInit(void)
{
    int rc=access(0);if(rc)return -1;
    if(rtc.info.initialized||rtc.exit_token){fail(JP_RTC_STATE);return -1;}
    rtc.owner=GetCurrentThreadId();rtc.busy=1;rtc.info.status=rtc.info.read_error=0;
    unsigned long count=jp_rtc_bounded_force();
    if(rtc.info.status){rtc.busy=0;return -1;}
    unsigned long handle=syChainAddHandler(0x900,tick,92,NULL);
    if(!handle){rtc.busy=0;fail(JP_RTC_CHAIN);return -1;}
    sygRtcCount=count;jp_rtc_source_mode(0);rtc.info.chain=handle;
    rtc.info.initialized=1;cache_info();rtc.busy=0;return 0;
}
static int finish(void)
{
    /* Never delete a recycled handle belonging to someone else. Explicitly
     * fail if an external caller has reset or closed our callback's owner. */
    if(!linked())return fail(JP_RTC_CHAIN);
    syChainDeleteHandler(rtc.info.chain);
    if(jp_vblank_chain_status())return fail(JP_RTC_CHAIN);
    rtc.info.initialized=rtc.info.chain=0;rtc.info.offset=0;
    sygRtcCount=0;jp_rtc_source_mode(0);cache_info();return fail(0);
}
int jp_rtc_shutdown(void)
{
    int rc=access(0);if(rc)return rc;
    if(rtc.exit_token)return fail(JP_RTC_STATE);
    if(!rtc.info.initialized)return fail(JP_RTC_NOT_INITIALIZED);
    return finish();
}
void syRtcFinish(void){(void)jp_rtc_shutdown();}
void syRtcExecServer(void)
{
    int rc=access(1);if(rc)return;
    if(!linked()){fail(JP_RTC_CHAIN);return;}
    rtc.busy=1;rtc.info.status=rtc.info.read_error=0;
    unsigned long old=sygRtcCount;jp_rtc_source_server();
    if(rtc.info.status)sygRtcCount=old;else rtc.info.updates++;
    cache_info();rtc.busy=0;
}
static void tick(void *unused)
{(void)unused;rtc.info.ticks++;syRtcExecServer();}
long syRtcGetDate(struct jp_rtc_date *out)
{
    int rc=access(1);if(rc)return -1;
    if(!out){fail(JP_RTC_ARGUMENT);return -1;}
    if(!linked()){fail(JP_RTC_CHAIN);return -1;}
    rtc.busy=1;rtc.info.status=rtc.info.read_error=0;
    struct jp_rtc_date date={0};unsigned long old=sygRtcCount;
    long result=jp_rtc_source_date(&date);rc=(int)rtc.info.status;
    if(rc)sygRtcCount=old;else *out=date;
    cache_info();rtc.busy=0;return rc?-1:result;
}
long syRtcSetDate(const struct jp_rtc_date *date)
{
    int rc=access(1);if(rc)return -1;
    unsigned long target;rc=count_for(date,&target);if(rc){fail(rc);return -1;}
    if(!linked()){fail(JP_RTC_CHAIN);return -1;}
    rtc.busy=1;rtc.info.status=rtc.info.read_error=0;uint32_t raw;
    if(raw_read(&raw)){rtc.busy=0;return -1;}
    int64_t before=rtc.info.offset;rtc.info.offset=(int64_t)target-raw;
    unsigned long current=jp_rtc_bounded_force();
    if(!rtc.info.status && (current==target || (target!=UINT32_MAX && current==target+1))){
        sygRtcCount=current;cache_info();rc=0;
    }else{rtc.info.offset=before;if(!rtc.info.status)fail(JP_RTC_UNSTABLE);rc=-1;}
    rtc.busy=0;return rc;
}
void syRtcSetServerMode(long mode)
{
    int rc=access(1);if(rc)return;
    if(mode&~3L){fail(JP_RTC_UNSUPPORTED);return;}
    jp_rtc_source_mode(mode);cache_info();fail(0);
}
long syRtcGetStat(void)
{int rc=access(1);return rc?-1:jp_rtc_source_stat();}
long syRtcCompareDate(const struct jp_rtc_date *a,const struct jp_rtc_date *b)
{
    int rc=access(0);if(rc)return 0;unsigned long x,y;
    rc=count_for(a,&x);if(!rc)rc=count_for(b,&y);
    if(rc){fail(rc);return 0;}fail(0);return jp_rtc_source_compare(a,b);
}
void syRtcDateToCount(const struct jp_rtc_date *date,unsigned long *out)
{
    int rc=access(0);if(rc)return;
    if(!date||!out||overlap(date,sizeof *date,out,sizeof *out)){fail(JP_RTC_ARGUMENT);return;}
    unsigned long count;rc=count_for(date,&count);if(rc){fail(rc);return;}*out=count;fail(0);
}
void syRtcCountToDate(unsigned long count,struct jp_rtc_date *date)
{
    int rc=access(0);if(rc)return;
    if(!date){fail(JP_RTC_ARGUMENT);return;}
    struct jp_rtc_date result={0};jp_rtc_source_to_date(count,&result);*date=result;fail(0);
}
int jp_rtc_status(void)
{return rtc.owner && rtc.owner!=GetCurrentThreadId()?JP_RTC_THREAD:(int)rtc.info.status;}
int jp_rtc_snapshot(struct jp_rtc_info *out)
{
    if(rtc.owner && rtc.owner!=GetCurrentThreadId())return JP_RTC_THREAD;
    if(!out)return JP_RTC_ARGUMENT;*out=rtc.info;return 0;
}
int jp_rtc_exit_claim(uint64_t *token)
{
    int rc=access(1);if(rc)return rc;
    if(!token)return fail(JP_RTC_ARGUMENT);
    if(rtc.exit_token||!next_exit_token)return fail(JP_RTC_STATE);
    if(!linked())return fail(JP_RTC_CHAIN);
    rtc.exit_token=next_exit_token++;rtc.exit_started=0;*token=rtc.exit_token;return fail(0);
}
static int exit_access(uint64_t token)
{
    int rc=access(0);if(rc)return rc;
    return !token||token!=rtc.exit_token?JP_RTC_STATE:0;
}
int jp_rtc_exit_cancel(uint64_t token)
{
    int rc=exit_access(token);if(rc)return rc;
    if(rtc.exit_started)return fail(JP_RTC_STATE);
    rtc.exit_token=0;return fail(0);
}
int jp_rtc_exit_finish(uint64_t token)
{
    int rc=exit_access(token);if(rc)return rc;
    rtc.exit_started=1;return rtc.info.initialized?finish():fail(0);
}
