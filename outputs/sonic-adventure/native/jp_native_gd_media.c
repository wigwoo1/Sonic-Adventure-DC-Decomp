#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "jp_native_gd_media.h"
static struct {
    struct jp_gd_media_info info;HANDLE file,event;OVERLAPPED io;
    DWORD owner;unsigned char *raw,*payload;size_t raw_bytes,bytes;
} media;
static int owner(void)
{return !media.info.initialized?JP_GD_NOT_INITIALIZED:media.owner!=GetCurrentThreadId()?JP_GD_THREAD:0;}
static int fail(int code,DWORD os)
{media.info.status=(uint32_t)code;media.info.os_error=os;return code;}
int jp_gd_media_status(void){int rc=owner();return rc?rc:(int)media.info.status;}
int jp_gd_media_snapshot(struct jp_gd_media_info *out)
{int rc=owner();if(rc)return rc;if(!out)return JP_GD_ARGUMENT;*out=media.info;return 0;}
int jp_gd_media_open(const wchar_t *path,uint32_t first,uint32_t raw,uint32_t maximum)
{
    if(media.info.initialized)return media.owner!=GetCurrentThreadId()?JP_GD_THREAD:JP_GD_STATE;
    if(!path||!*path||first<150||(raw!=2048&&raw!=2352)||!maximum||maximum>1048576)return JP_GD_ARGUMENT;
    HANDLE file=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED|FILE_FLAG_RANDOM_ACCESS,NULL);
    if(file==INVALID_HANDLE_VALUE)return JP_GD_IO;
    LARGE_INTEGER size;BY_HANDLE_FILE_INFORMATION fi;
    if(!GetFileSizeEx(file,&size)||!GetFileInformationByHandle(file,&fi)){
        CloseHandle(file);return JP_GD_IO;
    }
    if((fi.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)||size.QuadPart<=0||
       size.QuadPart%raw || (uint64_t)size.QuadPart/raw>UINT32_MAX-first){
        CloseHandle(file);return JP_GD_FORMAT;
    }
    HANDLE event=CreateEventW(NULL,TRUE,FALSE,NULL);
    if(!event){CloseHandle(file);return JP_GD_IO;}
    memset(&media,0,sizeof media);media.file=file;media.event=event;media.owner=GetCurrentThreadId();
    media.info.file_bytes=(uint64_t)size.QuadPart;media.info.first_fad=first;
    media.info.sectors=(uint32_t)((uint64_t)size.QuadPart/raw);media.info.raw_sector_bytes=raw;
    media.info.maximum_request_sectors=maximum;media.info.initialized=1;return 0;
}
int jp_gd_media_request(uint32_t fad,uint32_t sectors)
{
    int rc=owner();if(rc)return rc;
    if(media.info.state!=JP_GD_IDLE)return fail(JP_GD_BUSY,0);
    uint64_t offset=fad>=media.info.first_fad?(uint64_t)fad-media.info.first_fad:UINT64_MAX;
    uint64_t raw_bytes=(uint64_t)sectors*media.info.raw_sector_bytes,bytes=(uint64_t)sectors*2048;
    if(!sectors||sectors>media.info.maximum_request_sectors||offset>=media.info.sectors||
       sectors>media.info.sectors-offset||raw_bytes>UINT32_MAX||raw_bytes>SIZE_MAX||bytes>SIZE_MAX||media.info.requests==UINT64_MAX)return fail(JP_GD_RANGE,0);
    unsigned char *raw=malloc((size_t)raw_bytes),*payload=malloc((size_t)bytes);
    if(!raw||!payload){free(raw);free(payload);return fail(JP_GD_MEMORY,0);}
    if(!ResetEvent(media.event)){DWORD os=GetLastError();free(raw);free(payload);return fail(JP_GD_IO,os);}
    memset(&media.io,0,sizeof media.io);uint64_t at=offset*media.info.raw_sector_bytes;
    media.io.Offset=(DWORD)at;media.io.OffsetHigh=(DWORD)(at>>32);media.io.hEvent=media.event;
    media.raw=raw;media.payload=payload;media.raw_bytes=(size_t)raw_bytes;media.bytes=(size_t)bytes;
    media.info.request_fad=fad;media.info.request_sectors=sectors;media.info.state=JP_GD_READING;
    DWORD got=0;BOOL ok=ReadFile(media.file,raw,(DWORD)raw_bytes,&got,&media.io);
    if(!ok && GetLastError()!=ERROR_IO_PENDING){media.info.state=JP_GD_FAILED;return fail(JP_GD_IO,GetLastError());}
    media.info.requests++;return fail(0,0);
}
static int mode1(const unsigned char *raw)
{
    if(raw[0]||raw[11]||raw[15]!=1)return 0;
    for(unsigned j=1;j<11;j++)if(raw[j]!=255)return 0;
    /* The supplied GD image really uses minute A0 at FAD450000 and C2 near
     * its end: the minute's tens nibble extends beyond nine. Seconds/frames
     * remain ordinary BCD. Address identity and ECC are not claimed here. */
    if((raw[12]&15)>9)return 0;
    for(unsigned j=13;j<15;j++)if((raw[j]&15)>9||(raw[j]>>4)>9)return 0;
    return raw[13]<0x60 && raw[14]<0x75;
}
int jp_gd_media_poll(void)
{
    int rc=owner();if(rc)return rc;
    if(media.info.state==JP_GD_COMPLETE||media.info.state==JP_GD_CANCELLED)return 0;
    if(media.info.state==JP_GD_FAILED)return (int)media.info.status;
    if(media.info.state!=JP_GD_READING&&media.info.state!=JP_GD_CANCELLING)return fail(JP_GD_STATE,0);
    DWORD got=0;BOOL ok=GetOverlappedResult(media.file,&media.io,&got,FALSE);
    if(!ok){DWORD os=GetLastError();if(os==ERROR_IO_INCOMPLETE)return 0;
        if(media.info.state==JP_GD_CANCELLING&&os==ERROR_OPERATION_ABORTED){media.info.state=JP_GD_CANCELLED;media.info.cancelled++;return fail(0,0);}
        media.info.state=JP_GD_FAILED;return fail(JP_GD_IO,os);
    }
    media.info.raw_bytes_read+=got;
    if(media.info.state==JP_GD_CANCELLING){media.info.state=JP_GD_CANCELLED;media.info.cancelled++;return fail(0,0);}
    if(got!=media.raw_bytes){media.info.state=JP_GD_FAILED;return fail(JP_GD_IO,ERROR_HANDLE_EOF);}
    if(media.info.raw_sector_bytes==2352){
        for(uint32_t j=0;j<media.info.request_sectors;j++)if(!mode1(media.raw+(size_t)j*2352)){
            media.info.state=JP_GD_FAILED;return fail(JP_GD_FORMAT,0);
        }
        for(uint32_t j=0;j<media.info.request_sectors;j++)memcpy(media.payload+(size_t)j*2048,media.raw+(size_t)j*2352+16,2048);
    }else memcpy(media.payload,media.raw,media.bytes);
    media.info.completed++;media.info.payload_bytes+=media.bytes;media.info.state=JP_GD_COMPLETE;return fail(0,0);
}
int jp_gd_media_wait(uint32_t timeout)
{
    int rc=owner();if(rc)return rc;if(timeout>60000)return JP_GD_ARGUMENT;
    if(media.info.state==JP_GD_COMPLETE||media.info.state==JP_GD_CANCELLED)return 0;
    if(media.info.state==JP_GD_FAILED)return (int)media.info.status;
    if(media.info.state!=JP_GD_READING&&media.info.state!=JP_GD_CANCELLING)return fail(JP_GD_STATE,0);
    DWORD result=WaitForSingleObject(media.event,timeout);
    if(result==WAIT_TIMEOUT)return fail(JP_GD_TIMEOUT,0);
    if(result!=WAIT_OBJECT_0)return fail(JP_GD_IO,GetLastError());
    return jp_gd_media_poll();
}
int jp_gd_media_view(const void **out,size_t *bytes)
{
    int rc=owner();if(rc)return rc;
    if(!out||!bytes)return JP_GD_ARGUMENT;
    uintptr_t a=(uintptr_t)out,b=(uintptr_t)bytes;
    if(a<=b?b-a<sizeof *out:a-b<sizeof *bytes)return JP_GD_ARGUMENT;
    if(media.info.state!=JP_GD_COMPLETE)return JP_GD_STATE;
    *out=media.payload;*bytes=media.bytes;return 0;
}
int jp_gd_media_release(void)
{
    int rc=owner();if(rc)return rc;
    if(media.info.state==JP_GD_READING||media.info.state==JP_GD_CANCELLING)return fail(JP_GD_BUSY,0);
    free(media.raw);free(media.payload);media.raw=media.payload=NULL;media.raw_bytes=media.bytes=0;
    media.info.request_fad=media.info.request_sectors=0;media.info.state=JP_GD_IDLE;return fail(0,0);
}
int jp_gd_media_cancel(uint32_t timeout)
{
    int rc=owner();if(rc)return rc;if(timeout>60000)return JP_GD_ARGUMENT;
    if(media.info.state!=JP_GD_READING && media.info.state!=JP_GD_CANCELLING){
        if(media.info.state==JP_GD_COMPLETE){media.info.state=JP_GD_CANCELLED;media.info.cancelled++;}
        return 0;
    }
    if(media.info.state==JP_GD_READING){
        if(!CancelIoEx(media.file,&media.io)){DWORD os=GetLastError();if(os!=ERROR_NOT_FOUND)return fail(JP_GD_IO,os);}
        media.info.state=JP_GD_CANCELLING;
    }
    DWORD result=WaitForSingleObject(media.event,timeout);
    if(result==WAIT_TIMEOUT)return fail(JP_GD_TIMEOUT,0);
    if(result!=WAIT_OBJECT_0)return fail(JP_GD_IO,GetLastError());
    return jp_gd_media_poll();
}
int jp_gd_media_close(uint32_t timeout)
{
    int rc=owner();if(rc)return rc;if(timeout>60000)return JP_GD_ARGUMENT;
    if(media.info.state==JP_GD_READING||media.info.state==JP_GD_CANCELLING){rc=jp_gd_media_cancel(timeout);if(rc)return rc;}
    rc=jp_gd_media_release();if(rc)return rc;
    /* Keep a failing close's actual handles for a retry, instead of dropping
     * ownership. Close event only after the file has no live request. */
    if(media.file!=INVALID_HANDLE_VALUE){if(!CloseHandle(media.file))return fail(JP_GD_IO,GetLastError());media.file=INVALID_HANDLE_VALUE;}
    if(media.event){if(!CloseHandle(media.event))return fail(JP_GD_IO,GetLastError());media.event=NULL;}
    memset(&media,0,sizeof media);return 0;
}
