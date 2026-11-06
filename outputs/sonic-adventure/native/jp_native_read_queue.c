#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include "jp_native_read_queue.h"
#define JP_GDFS_API __declspec(dllimport)
#include "jp_native_gdfs_read_queue.h"
__declspec(dllimport) void *gdFsOpen(const char *,void *);
__declspec(dllimport) int32_t gdFsGetFileSize(void *,int32_t *);
__declspec(dllimport) int32_t gdFsGetFileFad(void *,int32_t *);
__declspec(dllimport) int32_t gdFsGetNumRd(void *);
__declspec(dllimport) void gdFsExecServer(void);
void jp_original_read_queue_init(void);
int jp_original_read_queue_add(char *,void *);
void jp_original_read_queue_serve(void);
void jp_original_read_queue_cancel(void);
int jp_original_read_queue_selected_slot(void);
void jp_read_queue_source_slot(uint32_t,struct jp_read_queue_slot *);
void *jp_read_queue_source_handle(void);
int jp_read_queue_source_output(const void *,size_t);
static struct {
    struct jp_read_queue_info info;DWORD thread;int busy;void *probe;
    struct {uint64_t capacity,bytes;uint32_t fad;} slot[16];
} queue;
static int access_owner(void)
{return queue.thread&&queue.thread!=GetCurrentThreadId()?JP_READ_QUEUE_THREAD:queue.busy?JP_READ_QUEUE_REENTRANT:0;}
static int result(int code,int source)
{queue.info.status=(uint32_t)code;queue.info.source_error=source;return code;}
static int output_valid(const void *p,size_t n)
{
    uintptr_t a=(uintptr_t)p,b=(uintptr_t)&queue;
    return p&&n<=UINTPTR_MAX-a&&!(a<b+sizeof queue&&b<a+n)&&jp_read_queue_source_output(p,n);
}
static void actual(void)
{
    queue.info.queued=0;
    for(uint32_t i=0;i<16;i++){struct jp_read_queue_slot s;jp_read_queue_source_slot(i,&s);if(s.state)queue.info.queued++;}
    queue.info.current=(uint32_t)jp_original_read_queue_selected_slot();
    queue.info.active_handle=(uintptr_t)(queue.probe?queue.probe:jp_read_queue_source_handle());
}
static int snapshot_output_valid(void *out,size_t bytes)
{
    if(!output_valid(out,bytes))return 0;uintptr_t at=(uintptr_t)out;
    for(uint32_t i=0;i<16;i++){struct jp_read_queue_slot s;jp_read_queue_source_slot(i,&s);
        if(s.state&&queue.slot[i].capacity&&at<s.destination+queue.slot[i].capacity&&s.destination<at+bytes)return 0;}
    return 1;
}
static int enter(void)
{
    int rc=access_owner();if(rc)return rc;if(!queue.info.initialized)return JP_READ_QUEUE_STATE;
    rc=jp_gdfs_read_queue_enter(queue.info.lease);
    if(rc)return rc==JP_GDFS_REENTRANT?JP_READ_QUEUE_REENTRANT:JP_READ_QUEUE_GDFS;
    queue.busy=1;return 0;
}
static int leave(void)
{
    actual();int rc=jp_gdfs_read_queue_leave(queue.info.lease);queue.busy=0;
    return rc?result(JP_READ_QUEUE_GDFS,rc):(int)queue.info.status;
}
void jp_read_queue_source_problem(int error,int source)
{if(!queue.info.status)result(error,source);queue.info.phase=2;}
int jp_read_queue_source_close(void *handle)
{
    int rc=jp_gdfs_read_queue_close(queue.info.lease,handle);
    if(rc)jp_read_queue_source_problem(JP_READ_QUEUE_GDFS,rc);return rc;
}
int jp_read_queue_source_validate(void *handle,int32_t bytes,uint32_t slot)
{
    int32_t fad=0;if(slot>=16||bytes<0||bytes>INT32_MAX-2047||!gdFsGetFileFad(handle,&fad)){
        jp_read_queue_source_problem(JP_READ_QUEUE_SOURCE,-15);return 0;
    }
    uint64_t rounded=((uint64_t)bytes+2047)&~UINT64_C(2047);
    if(queue.slot[slot].bytes!=(uint32_t)bytes||queue.slot[slot].fad!=(uint32_t)fad){jp_read_queue_source_problem(JP_READ_QUEUE_IDENTITY,-24);return 0;}
    if(rounded>queue.slot[slot].capacity){jp_read_queue_source_problem(JP_READ_QUEUE_BUFFER,-24);return 0;}
    return 1;
}
int jp_read_queue_source_complete(void *handle,uint32_t slot)
{
    if(slot>=16||gdFsGetNumRd(handle)!=(int32_t)((queue.slot[slot].bytes+2047)&~UINT64_C(2047))){
        jp_read_queue_source_problem(JP_READ_QUEUE_SOURCE,-19);return 0;
    }
    return 1;
}
void jp_read_queue_source_retired(uint32_t slot)
{memset(&queue.slot[slot],0,sizeof queue.slot[slot]);queue.info.completed++;}
int jp_read_queue_initialize(void)
{
    int rc=access_owner();if(rc)return rc;if(queue.info.initialized)return JP_READ_QUEUE_STATE;
    uint64_t token=0;rc=jp_gdfs_read_queue_claim(&token);
    if(rc)return rc==JP_GDFS_THREAD?JP_READ_QUEUE_THREAD:rc==JP_GDFS_REENTRANT?JP_READ_QUEUE_REENTRANT:JP_READ_QUEUE_GDFS;
    queue.thread=GetCurrentThreadId();queue.info.creator_thread=queue.thread;queue.info.lease=token;queue.info.initialized=1;
    queue.busy=1;jp_original_read_queue_init();memset(queue.slot,0,sizeof queue.slot);queue.busy=0;
    queue.info.phase=1;queue.info.starts++;actual();return result(0,0);
}
int jp_read_queue_snapshot(struct jp_read_queue_info *out)
{
    if(queue.thread&&queue.thread!=GetCurrentThreadId())return JP_READ_QUEUE_THREAD;
    if(!snapshot_output_valid(out,sizeof *out))return JP_READ_QUEUE_ARGUMENT;actual();*out=queue.info;return 0;
}
int jp_read_queue_slot_snapshot(uint32_t index,struct jp_read_queue_slot *out)
{
    if(queue.thread&&queue.thread!=GetCurrentThreadId())return JP_READ_QUEUE_THREAD;
    if(index>=16||!snapshot_output_valid(out,sizeof *out))return JP_READ_QUEUE_ARGUMENT;
    struct jp_read_queue_slot s;jp_read_queue_source_slot(index,&s);
    s.capacity=queue.slot[index].capacity;s.file_bytes=queue.slot[index].bytes;s.fad=queue.slot[index].fad;*out=s;return 0;
}
int jp_read_queue_add(const char *name,void *destination,size_t capacity)
{
    int rc=enter();if(rc)return rc;
    if(queue.info.phase!=1){(void)leave();return JP_READ_QUEUE_STATE;}
    size_t n=0;if(name)while(n<32&&name[n])n++;
    uintptr_t at=(uintptr_t)destination;
    if(!name||!n||n>=32||(at&31)||capacity>UINTPTR_MAX-at||(capacity&&!output_valid(destination,capacity))){(void)leave();return JP_READ_QUEUE_ARGUMENT;}
    for(size_t i=0;i<n;i++)if((unsigned char)name[i]<32||(unsigned char)name[i]>=127||name[i]=='/'||name[i]=='\\'){(void)leave();return JP_READ_QUEUE_ARGUMENT;}
    uint32_t free_slot=16;
    for(uint32_t i=0;i<16;i++){
        struct jp_read_queue_slot s;jp_read_queue_source_slot(i,&s);
        if(!s.state){if(free_slot==16)free_slot=i;continue;}
        /* Native metadata carries capacity; the source owns destination/state. */
        if(capacity&&queue.slot[i].capacity&&at<s.destination+queue.slot[i].capacity&&s.destination<at+capacity){(void)leave();return JP_READ_QUEUE_BUFFER;}
    }
    if(free_slot==16){(void)leave();return JP_READ_QUEUE_FULL;}
    void *h=gdFsOpen(name,NULL);int32_t bytes=0,fad=0;queue.probe=h;
    if(!h){(void)leave();return JP_READ_QUEUE_SOURCE;}
    int valid=gdFsGetFileSize(h,&bytes)&&gdFsGetFileFad(h,&fad)&&bytes>=0&&bytes<=INT32_MAX-2047;
    rc=jp_read_queue_source_close(h);
    if(rc)return leave();
    queue.probe=NULL;
    if(!valid){(void)leave();return JP_READ_QUEUE_SOURCE;}
    if((((uint64_t)bytes+2047)&~UINT64_C(2047))>capacity){(void)leave();return JP_READ_QUEUE_BUFFER;}
    if(!jp_original_read_queue_add((char *)name,destination)){(void)leave();return JP_READ_QUEUE_FULL;}
    queue.slot[free_slot].capacity=capacity;queue.slot[free_slot].bytes=(uint32_t)bytes;queue.slot[free_slot].fad=(uint32_t)fad;
    queue.info.adds++;return leave();
}
int jp_read_queue_serve(void)
{
    int rc=enter();if(rc)return rc;if(queue.info.phase!=1){(void)leave();return JP_READ_QUEUE_STATE;}
    jp_original_read_queue_serve();if(!queue.info.status)gdFsExecServer();return leave();
}
int jp_read_queue_cancel(void)
{
    int rc=enter();if(rc)return rc;result(0,0);
    if(queue.probe){rc=jp_read_queue_source_close(queue.probe);if(rc)return leave();queue.probe=NULL;}
    jp_original_read_queue_cancel();
    if(!queue.info.status){memset(queue.slot,0,sizeof queue.slot);queue.info.phase=1;queue.info.cancels++;}
    return leave();
}
int jp_read_queue_shutdown(void)
{
    int rc=access_owner();if(rc)return rc;if(!queue.info.initialized)return 0;
    rc=jp_read_queue_cancel();if(rc)return rc;
    rc=jp_gdfs_read_queue_release(queue.info.lease);if(rc)return result(JP_READ_QUEUE_GDFS,rc);
    queue.info.lease=0;queue.info.initialized=0;queue.info.phase=0;return result(0,0);
}
