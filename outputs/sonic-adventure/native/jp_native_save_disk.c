#include "jp_native_save_disk.h"
#pragma push_macro("NULL")
#undef NULL
#include "save/save.h"
#include "save/card.h"
#pragma pop_macro("NULL")
#include <windows.h>
#include <string.h>
_Static_assert(sizeof(struct save)==0x4a0 && SAVE_COUNT==3 && CHARACTER_COUNT==8,"qualified Japanese save format");
static struct {
    struct jp_save_disk_info info;wchar_t root[32768];
    uint64_t sequence;DWORD owner;int initialized;
} disk;
static int owner(void)
{return !disk.initialized?JP_SAVE_DISK_NOT_INITIALIZED:disk.owner!=GetCurrentThreadId()?JP_SAVE_DISK_THREAD:JP_SAVE_DISK_OK;}
static int ready(void){int e=owner();return e?e:(int)disk.info.status;}
static int fail(int e,DWORD os){disk.info.status=(uint32_t)e;disk.info.os_error=os;return e;}
int jp_save_disk_status(void){return ready();}
void jp_save_disk_clear_error(void){if(!owner()){disk.info.status=0;disk.info.os_error=0;}}
int jp_save_disk_snapshot(struct jp_save_disk_info *out)
{int e=owner();if(e)return e;if(!out)return JP_SAVE_DISK_ARGUMENT;*out=disk.info;return 0;}
int jp_save_disk_initialize(const wchar_t *root)
{
    if(disk.initialized)return disk.owner==GetCurrentThreadId()?JP_SAVE_DISK_STATE:JP_SAVE_DISK_THREAD;
    if(!root||!*root)return JP_SAVE_DISK_ARGUMENT;
    wchar_t full[32768];DWORD n=GetFullPathNameW(root,32768,full,NULL);
    if(!n||n>=32768-128)return JP_SAVE_DISK_ARGUMENT;
    DWORD attr=GetFileAttributesW(full);
    if(attr==INVALID_FILE_ATTRIBUTES||!(attr&FILE_ATTRIBUTE_DIRECTORY))return JP_SAVE_DISK_ARGUMENT;
    memset(&disk,0,sizeof disk);memcpy(disk.root,full,(n+1)*sizeof(wchar_t));disk.owner=GetCurrentThreadId();disk.initialized=1;
    disk.info.record_bytes=sizeof(struct save);disk.info.record_count=SAVE_COUNT;return 0;
}
int jp_save_disk_shutdown(void)
{if(!disk.initialized)return 0;int e=owner();if(e)return e;memset(&disk,0,sizeof disk);return 0;}
static size_t prefix(wchar_t *path)
{size_t n=0;while(disk.root[n])n++;memcpy(path,disk.root,n*sizeof(wchar_t));if(n&&path[n-1]!=L'\\'&&path[n-1]!=L'/')path[n++]=L'\\';return n;}
static void target_path(wchar_t *path)
{size_t n=prefix(path);const wchar_t name[]=L"SONICADV_JP.SAV";memcpy(path+n,name,sizeof name);}
static size_t hex(wchar_t *p,uint64_t v,unsigned count)
{for(unsigned i=0;i<count;i++)p[i]=L"0123456789ABCDEF"[(v>>((count-i-1)*4))&15];return count;}
static int valid(struct save *data)
{
    if(!save_check_all(data))return 0;
    for(unsigned i=0;i<SAVE_COUNT;i++)if(data[i].character>=CHARACTER_COUNT)return 0;
    return 1;
}
int jp_save_disk_write(void)
{
    int e=ready();if(e)return e;
    struct save snapshot[SAVE_COUNT];memcpy(snapshot,saves,sizeof snapshot);
    if(!valid(snapshot))return fail(JP_SAVE_DISK_FORMAT,0);
    wchar_t target[32768],temporary[32768];target_path(target);HANDLE f=INVALID_HANDLE_VALUE;
    for(unsigned attempt=0;attempt<16;attempt++){
        size_t n=prefix(temporary);const wchar_t tag[]=L".sa-native-save-";memcpy(temporary+n,tag,sizeof tag-sizeof(wchar_t));n+=sizeof tag/sizeof(wchar_t)-1;
        n+=hex(temporary+n,GetCurrentProcessId(),8);n+=hex(temporary+n,disk.owner,8);n+=hex(temporary+n,++disk.sequence,16);memcpy(temporary+n,L".tmp",5*sizeof(wchar_t));
        f=CreateFileW(temporary,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
        if(f!=INVALID_HANDLE_VALUE)break;
        DWORD os=GetLastError();if(os!=ERROR_FILE_EXISTS&&os!=ERROR_ALREADY_EXISTS)return fail(JP_SAVE_DISK_IO,os);
    }
    if(f==INVALID_HANDLE_VALUE)return fail(JP_SAVE_DISK_IO,ERROR_FILE_EXISTS);
    DWORD wrote=0,os=0;
    if(!WriteFile(f,snapshot,sizeof snapshot,&wrote,NULL))os=GetLastError();
    else if(wrote!=sizeof snapshot)os=ERROR_WRITE_FAULT;
    else if(!FlushFileBuffers(f))os=GetLastError();
    if(!CloseHandle(f)&&!os)os=GetLastError();
    if(!os&&!MoveFileExW(temporary,target,MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH))os=GetLastError();
    if(os){DeleteFileW(temporary);return fail(JP_SAVE_DISK_IO,os);}
    disk.info.writes++;return 0;
}
int jp_save_disk_read(void)
{
    int e=ready();if(e)return e;
    wchar_t path[32768];target_path(path);HANDLE f=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if(f==INVALID_HANDLE_VALUE)return fail(JP_SAVE_DISK_IO,GetLastError());
    struct save pending[SAVE_COUNT];LARGE_INTEGER size;DWORD os=0,got=0;int result=0;
    if(!GetFileSizeEx(f,&size)){os=GetLastError();result=JP_SAVE_DISK_IO;}
    else if(size.QuadPart!=sizeof pending)result=JP_SAVE_DISK_FORMAT;
    else if(!ReadFile(f,pending,sizeof pending,&got,NULL)){os=GetLastError();result=JP_SAVE_DISK_IO;}
    else if(got!=sizeof pending){os=ERROR_HANDLE_EOF;result=JP_SAVE_DISK_IO;}
    if(!CloseHandle(f)&&!result){os=GetLastError();result=JP_SAVE_DISK_IO;}
    if(result)return fail(result,os);
    if(!valid(pending))return fail(JP_SAVE_DISK_FORMAT,0);
    memcpy(saves,pending,sizeof pending);disk.info.reads++;return 0;
}
int jp_save_disk_apply(uint32_t slot)
{
    int e=ready();if(e)return e;if(slot>=SAVE_COUNT)return fail(JP_SAVE_DISK_ARGUMENT,0);
    if(!valid(saves))return fail(JP_SAVE_DISK_FORMAT,0);
    save_slot=(int)slot;save_load();disk.info.applies++;return 0;
}
