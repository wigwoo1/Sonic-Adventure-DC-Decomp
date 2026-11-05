#include "jp_native_recording.h"
#pragma push_macro("NULL")
#undef NULL
#include "progress/recorder.h"
#pragma pop_macro("NULL")
#include <windows.h>
#include <string.h>
#include <limits.h>

_Static_assert(sizeof(struct pad_sample)==12,"retail recording sample size");
_Static_assert(offsetof(struct pad_sample,on)==0 && offsetof(struct pad_sample,l)==4 &&
    offsetof(struct pad_sample,r)==6 && offsetof(struct pad_sample,x1)==8 &&
    offsetof(struct pad_sample,y1)==10,"retail recording field offsets");
_Static_assert(JP_RECORDING_MAX_PLAY_BYTES==JP_RECORDING_PLAY_LAST_OFFSET+12,
    "the original playback comparison includes the last sample");
/* One extra element permits the original end-path increment. Storage also
 * covers the historical record-stop double increment without invalid pointer
 * arithmetic; only loaded playback bytes are readable through the guard. */
struct pad_sample jp_recording_samples[JP_RECORDING_STORAGE_SAMPLES];
static struct {
    struct jp_recording_info info;
    wchar_t root[32768];DWORD owner;int initialized;
    uint64_t temporary_sequence;
} state;
static int owner(void)
{return !state.initialized?JP_RECORDING_STATE:
    state.owner!=GetCurrentThreadId()?JP_RECORDING_THREAD:JP_RECORDING_OK;}
static int ready(void){int e=owner();return e?e:(int)state.info.status;}
static int fail(int e,DWORD os)
{state.info.status=(uint32_t)e;state.info.os_error=os;return e;}
int jp_recording_status(void){return ready();}
void jp_recording_clear_error(void)
{if(!owner()){state.info.status=JP_RECORDING_OK;state.info.os_error=0;}}
int jp_recording_snapshot(struct jp_recording_info *out)
{int e=owner();if(e)return e;if(!out)return JP_RECORDING_ARGUMENT;*out=state.info;return JP_RECORDING_OK;}
int jp_recording_initialize(const wchar_t *root)
{
    if(state.initialized)return state.owner==GetCurrentThreadId()?JP_RECORDING_STATE:JP_RECORDING_THREAD;
    if(!root || !*root)return JP_RECORDING_ARGUMENT;
    wchar_t full[32768];DWORD n=GetFullPathNameW(root,32768,full,NULL);
    if(!n || n>=32768-100)return JP_RECORDING_ARGUMENT;
    DWORD attr=GetFileAttributesW(full);
    if(attr==INVALID_FILE_ATTRIBUTES || !(attr&FILE_ATTRIBUTE_DIRECTORY))return JP_RECORDING_ARGUMENT;
    memset(&state,0,sizeof state);memcpy(state.root,full,(n+1)*sizeof(wchar_t));
    memset(jp_recording_samples,0,sizeof jp_recording_samples);
    state.owner=GetCurrentThreadId();state.initialized=1;return JP_RECORDING_OK;
}
int jp_recording_shutdown(void)
{int e=owner();if(e)return e;memset(jp_recording_samples,0,sizeof jp_recording_samples);memset(&state,0,sizeof state);return JP_RECORDING_OK;}
void *jp_recording_buffer(void){return owner()?NULL:jp_recording_samples;}
size_t jp_recording_storage_bytes(void){return sizeof jp_recording_samples;}
int jp_recording_begin_record(void)
{
    int e=ready();if(e)return e;memset(jp_recording_samples,0,sizeof jp_recording_samples);
    state.info.loaded_bytes=state.info.saved_bytes=state.info.legacy_cursor=0;return JP_RECORDING_OK;
}
static int valid_data(const void *source,size_t n)
{
    if(!source || !n || n>JP_RECORDING_MAX_PLAY_BYTES || n%12)return 0;
    const unsigned char *p=source;
    for(size_t i=0;i<n;i+=12){uint32_t on;memcpy(&on,p+i,4);if(on==UINT32_MAX)return 1;}
    return 0;
}
int jp_recording_load_memory(const void *source,size_t n)
{
    int e=ready();if(e)return e;
    uintptr_t b=(uintptr_t)jp_recording_samples,s=(uintptr_t)source;
    if(!source || n>JP_RECORDING_MAX_PLAY_BYTES)return fail(JP_RECORDING_ARGUMENT,0);
    if(s>UINTPTR_MAX-n || (s<b+sizeof jp_recording_samples && s+n>b))return fail(JP_RECORDING_ARGUMENT,0);
    if(!valid_data(source,n))return fail(JP_RECORDING_FORMAT,0);
    memcpy(jp_recording_samples,source,n);
    memset((unsigned char *)jp_recording_samples+n,0,sizeof jp_recording_samples-n);
    state.info.loaded_bytes=(uint32_t)n;state.info.saved_bytes=state.info.legacy_cursor=0;
    return JP_RECORDING_OK;
}
int jp_recording_load_file(const wchar_t *path)
{
    int e=ready();if(e)return e;if(!path || !*path)return fail(JP_RECORDING_ARGUMENT,0);
    HANDLE f=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if(f==INVALID_HANDLE_VALUE)return fail(JP_RECORDING_IO,GetLastError());
    LARGE_INTEGER size;DWORD os=0;unsigned char *bytes=NULL;int result=JP_RECORDING_OK;
    if(!GetFileSizeEx(f,&size)){os=GetLastError();result=JP_RECORDING_IO;}
    else if(size.QuadPart<=0 || size.QuadPart>JP_RECORDING_MAX_PLAY_BYTES || size.QuadPart%12)result=JP_RECORDING_FORMAT;
    else {
        DWORD n=(DWORD)size.QuadPart,got=0;
        bytes=HeapAlloc(GetProcessHeap(),0,n);
        if(!bytes){os=ERROR_NOT_ENOUGH_MEMORY;result=JP_RECORDING_IO;}
        else if(!ReadFile(f,bytes,n,&got,NULL)){os=GetLastError();result=JP_RECORDING_IO;}
        else if(got!=n){os=ERROR_HANDLE_EOF;result=JP_RECORDING_IO;}
        else if(!valid_data(bytes,n))result=JP_RECORDING_FORMAT;
    }
    if(!CloseHandle(f) && !result){os=GetLastError();result=JP_RECORDING_IO;}
    if(!result)result=jp_recording_load_memory(bytes,(size_t)size.QuadPart);
    if(bytes)HeapFree(GetProcessHeap(),0,bytes);
    return result?fail(result,os):JP_RECORDING_OK;
}
int jp_recording_check_cursor(const void *cursor,int mode)
{
    int e=ready();if(e)return e;
    uintptr_t b=(uintptr_t)jp_recording_samples,c=(uintptr_t)cursor;
    if(c<b || c-b>=sizeof jp_recording_samples || (c-b)%12)return fail(JP_RECORDING_BOUNDS,0);
    size_t offset=c-b;
    if(mode==0){if(offset>JP_RECORDING_MAX_RECORD_BYTES-12)return fail(JP_RECORDING_BOUNDS,0);}
    else if(mode==1){if(offset>=state.info.loaded_bytes)return fail(JP_RECORDING_BOUNDS,0);}
    else if(mode!=2)return fail(JP_RECORDING_ARGUMENT,0);
    return JP_RECORDING_OK;
}
static int name_ok(const char *name)
{
    static const char *const names[]={"K_SONIC.BIN","K_MILES.BIN","K_KNUCKLES.BIN","K_AMY.BIN","K_E102.BIN","K_BIG.BIN"};
    if(name)for(unsigned i=0;i<6;i++)if(!strcmp(name,names[i]))return 1;
    return 0;
}
static size_t root_path(wchar_t *path)
{
    size_t n=0;while(state.root[n])n++;memcpy(path,state.root,n*sizeof(wchar_t));
    if(n && path[n-1]!=L'\\' && path[n-1]!=L'/')path[n++]=L'\\';return n;
}
static size_t hex(wchar_t *p,uint64_t value,unsigned digits)
{for(unsigned i=0;i<digits;i++)p[i]=L"0123456789ABCDEF"[(value>>((digits-i-1)*4))&15];return digits;}
int jp_recording_save(const char *name,const void *data,int bytes)
{
    int e=ready();if(e)return e;
    if(!name_ok(name) || data!=jp_recording_samples || bytes<12 ||
        (uint32_t)bytes>JP_RECORDING_MAX_RECORD_BYTES || bytes%12)return fail(JP_RECORDING_ARGUMENT,0);
    const struct pad_sample *samples=data;
    if(samples[bytes/12-1].on!=UINT32_MAX)return fail(JP_RECORDING_FORMAT,0);
    wchar_t target[32768],temporary[32768];size_t n=root_path(target),j=0;
    while(name[j]){target[n+j]=(wchar_t)(unsigned char)name[j];j++;}target[n+j]=0;
    HANDLE f=INVALID_HANDLE_VALUE;
    for(unsigned attempt=0;attempt<16;attempt++){
        n=root_path(temporary);const wchar_t prefix[]=L".jp-recording-";
        memcpy(temporary+n,prefix,sizeof prefix-sizeof(wchar_t));n+=sizeof prefix/sizeof(wchar_t)-1;
        n+=hex(temporary+n,GetCurrentProcessId(),8);n+=hex(temporary+n,state.owner,8);
        n+=hex(temporary+n,++state.temporary_sequence,16);memcpy(temporary+n,L".tmp",5*sizeof(wchar_t));
        f=CreateFileW(temporary,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
        if(f!=INVALID_HANDLE_VALUE)break;
        DWORD os=GetLastError();if(os!=ERROR_FILE_EXISTS && os!=ERROR_ALREADY_EXISTS)return fail(JP_RECORDING_IO,os);
    }
    if(f==INVALID_HANDLE_VALUE)return fail(JP_RECORDING_IO,ERROR_FILE_EXISTS);
    DWORD wrote=0,os=0;
    if(!WriteFile(f,data,(DWORD)bytes,&wrote,NULL))os=GetLastError();
    else if(wrote!=(DWORD)bytes)os=ERROR_WRITE_FAULT;
    else if(!FlushFileBuffers(f))os=GetLastError();
    if(!CloseHandle(f) && !os)os=GetLastError();
    if(!os && !MoveFileExW(temporary,target,MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH))os=GetLastError();
    if(os){DeleteFileW(temporary);return fail(JP_RECORDING_IO,os);}
    state.info.saved_bytes=(uint32_t)bytes;state.info.save_count++;return JP_RECORDING_OK;
}
int jp_recording_debug_cursor(const void *cursor)
{
    int e=jp_recording_check_cursor(cursor,2);if(e)return e;
    uintptr_t offset=(uintptr_t)cursor-(uintptr_t)jp_recording_samples;
    state.info.legacy_cursor=0x0cbe0000u+(uint32_t)offset;state.info.debug_count++;
    wchar_t line[]=L"Sonic Adventure recorder cursor: 00000000\n";
    hex(line+33,state.info.legacy_cursor,8);OutputDebugStringW(line);return JP_RECORDING_OK;
}
