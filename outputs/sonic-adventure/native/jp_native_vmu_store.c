/* Real virtual VMU media; follows the qualified save-disk owner's flush/rename
 * pattern without changing or reinterpreting that owner's save-record format. */
#include <stddef.h>
#include "jp_native_vmu_store.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#define IMAGE_BYTES (JP_VMU_BLOCK_BYTES*JP_VMU_BLOCKS)
static struct {
    wchar_t root[32768];DWORD owner;int initialized;uint64_t sequence;
    unsigned char *images[JP_VMU_DRIVES];HANDLE locks[JP_VMU_DRIVES];
    struct jp_vmu_store_info info;
} media;
static int owner(void){return !media.initialized?JP_VMU_NOT_INITIALIZED:
    media.owner!=GetCurrentThreadId()?JP_VMU_THREAD:JP_VMU_OK;}
static int ready(void){int e=owner();return e?e:(int)media.info.status;}
static int fail(int e,DWORD os){media.info.status=(uint32_t)e;media.info.os_error=os;return e;}
int jp_vmu_store_status(void){return ready();}
void jp_vmu_store_clear_error(void){if(!owner()){media.info.status=0;media.info.os_error=0;}}
int jp_vmu_store_snapshot(struct jp_vmu_store_info *out)
{int e=owner();if(e)return e;if(!out)return JP_VMU_ARGUMENT;*out=media.info;return 0;}
int jp_vmu_store_initialize(const wchar_t *root)
{
    if(media.initialized)return owner()?JP_VMU_THREAD:JP_VMU_STATE;
    if(!root || !*root)return JP_VMU_ARGUMENT;
    wchar_t full[32768];DWORD n=GetFullPathNameW(root,32768,full,NULL);
    if(!n || n>=32768-128)return JP_VMU_ARGUMENT;
    DWORD attr=GetFileAttributesW(full);
    if(attr==INVALID_FILE_ATTRIBUTES || !(attr&FILE_ATTRIBUTE_DIRECTORY))return JP_VMU_ARGUMENT;
    memset(&media,0,sizeof media);memcpy(media.root,full,(n+1)*sizeof(wchar_t));
    media.owner=GetCurrentThreadId();media.initialized=1;
    for(unsigned i=0;i<JP_VMU_DRIVES;i++)media.locks[i]=INVALID_HANDLE_VALUE;
    return 0;
}
static size_t prefix(wchar_t *path)
{
    size_t n=wcslen(media.root);memcpy(path,media.root,n*sizeof(wchar_t));
    if(n && path[n-1]!=L'\\' && path[n-1]!=L'/')path[n++]=L'\\';return n;
}
static void path_for(wchar_t *path,uint32_t drive,int lock)
{
    size_t n=prefix(path);wchar_t name[]=L"VMU_A1.VMU";name[4]=(wchar_t)(L'A'+drive/2);name[5]=(wchar_t)(L'1'+drive%2);
    memcpy(path+n,name,sizeof name);if(lock){n+=wcslen(name);memcpy(path+n,L".lock",6*sizeof(wchar_t));}
}
static HANDLE take_lock(uint32_t drive)
{
    wchar_t path[32768];path_for(path,drive,1);
    HANDLE h=CreateFileW(path,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_HIDDEN,NULL);
    if(h==INVALID_HANDLE_VALUE){DWORD os=GetLastError();fail(os==ERROR_SHARING_VIOLATION?JP_VMU_BUSY:JP_VMU_IO,os);}return h;
}
static int load(uint32_t drive,unsigned char *bytes)
{
    wchar_t path[32768];path_for(path,drive,0);HANDLE h=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if(h==INVALID_HANDLE_VALUE)return fail(JP_VMU_IO,GetLastError());
    LARGE_INTEGER size;DWORD got=0,os=0;int result=0;
    if(!GetFileSizeEx(h,&size)){result=JP_VMU_IO;os=GetLastError();}
    else if(size.QuadPart!=IMAGE_BYTES)result=JP_VMU_FORMAT;
    else if(!ReadFile(h,bytes,IMAGE_BYTES,&got,NULL)){result=JP_VMU_IO;os=GetLastError();}
    else if(got!=IMAGE_BYTES){result=JP_VMU_IO;os=ERROR_HANDLE_EOF;}
    if(!CloseHandle(h) && !result){result=JP_VMU_IO;os=GetLastError();}
    return result?fail(result,os):0;
}
static void hex(wchar_t *p,uint64_t value,unsigned count)
{for(unsigned i=0;i<count;i++)p[i]=L"0123456789ABCDEF"[(value>>((count-i-1)*4))&15];}
static int persist(uint32_t drive,const unsigned char *bytes,int replace)
{
    wchar_t target[32768],temp[32768];path_for(target,drive,0);HANDLE h=INVALID_HANDLE_VALUE;
    for(unsigned attempt=0;attempt<16;attempt++){
        size_t n=prefix(temp);memcpy(temp+n,L".vmu-",5*sizeof(wchar_t));n+=5;
        hex(temp+n,media.owner,8);n+=8;hex(temp+n,++media.sequence,16);n+=16;memcpy(temp+n,L".tmp",5*sizeof(wchar_t));
        h=CreateFileW(temp,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
        if(h!=INVALID_HANDLE_VALUE)break;
        DWORD os=GetLastError();if(os!=ERROR_FILE_EXISTS && os!=ERROR_ALREADY_EXISTS)return fail(JP_VMU_IO,os);
    }
    if(h==INVALID_HANDLE_VALUE)return fail(JP_VMU_IO,ERROR_FILE_EXISTS);
    DWORD wrote=0,os=0;
    if(!WriteFile(h,bytes,IMAGE_BYTES,&wrote,NULL))os=GetLastError();
    else if(wrote!=IMAGE_BYTES)os=ERROR_WRITE_FAULT;
    else if(!FlushFileBuffers(h))os=GetLastError();
    if(!CloseHandle(h) && !os)os=GetLastError();
    if(!os && !MoveFileExW(temp,target,MOVEFILE_WRITE_THROUGH|(replace?MOVEFILE_REPLACE_EXISTING:0)))os=GetLastError();
    if(os){DeleteFileW(temp);return fail(!replace && (os==ERROR_FILE_EXISTS || os==ERROR_ALREADY_EXISTS)?JP_VMU_EXISTS:JP_VMU_IO,os);}return 0;
}
int jp_vmu_store_create(uint32_t drive)
{
    int e=ready();if(e)return e;if(drive>=JP_VMU_DRIVES)return fail(JP_VMU_ARGUMENT,0);
    if(media.images[drive])return fail(JP_VMU_STATE,0);
    HANDLE lock=take_lock(drive);if(lock==INVALID_HANDLE_VALUE)return (int)media.info.status;
    unsigned char *bytes=calloc(1,IMAGE_BYTES);if(!bytes){CloseHandle(lock);return fail(JP_VMU_ALLOCATION,0);}
    e=persist(drive,bytes,0);free(bytes);CloseHandle(lock);if(!e)media.info.created++;return e;
}
int jp_vmu_store_open(uint32_t drive)
{
    int e=ready();if(e)return e;if(drive>=JP_VMU_DRIVES)return fail(JP_VMU_ARGUMENT,0);
    if(media.images[drive])return fail(JP_VMU_STATE,0);
    HANDLE lock=take_lock(drive);if(lock==INVALID_HANDLE_VALUE)return (int)media.info.status;
    unsigned char *bytes=malloc(IMAGE_BYTES);if(!bytes){CloseHandle(lock);return fail(JP_VMU_ALLOCATION,0);}
    e=load(drive,bytes);if(e){free(bytes);CloseHandle(lock);return e;}
    media.images[drive]=bytes;media.locks[drive]=lock;media.info.attached_mask|=1u<<drive;return 0;
}
int jp_vmu_store_close(uint32_t drive)
{
    int e=owner();if(e)return e;if(drive>=JP_VMU_DRIVES)return JP_VMU_ARGUMENT;
    if(!media.images[drive])return 0;
    free(media.images[drive]);media.images[drive]=NULL;CloseHandle(media.locks[drive]);media.locks[drive]=INVALID_HANDLE_VALUE;media.info.attached_mask&=~(1u<<drive);return 0;
}
int jp_vmu_store_shutdown(void)
{
    if(!media.initialized)return 0;int e=owner();if(e)return e;
    for(uint32_t i=0;i<JP_VMU_DRIVES;i++)jp_vmu_store_close(i);memset(&media,0,sizeof media);return 0;
}
static int block(uint32_t drive,uint32_t index,const void *data)
{
    int e=ready();if(e)return e;
    if(drive>=JP_VMU_DRIVES || index>=JP_VMU_BLOCKS || !data || (uintptr_t)data>UINTPTR_MAX-JP_VMU_BLOCK_BYTES)return fail(JP_VMU_ARGUMENT,0);
    return media.images[drive]?0:fail(JP_VMU_STATE,0);
}
int jp_vmu_store_read(uint32_t drive,uint32_t index,void *out)
{
    int e=block(drive,index,out);if(e)return e;memcpy(out,media.images[drive]+index*JP_VMU_BLOCK_BYTES,JP_VMU_BLOCK_BYTES);media.info.block_reads++;return 0;
}
int jp_vmu_store_write(uint32_t drive,uint32_t index,const void *in)
{
    int e=block(drive,index,in);if(e)return e;
    unsigned char *next=malloc(IMAGE_BYTES);if(!next)return fail(JP_VMU_ALLOCATION,0);
    e=load(drive,next);if(!e && memcmp(next,media.images[drive],IMAGE_BYTES))e=fail(JP_VMU_STATE,0);
    if(!e){memcpy(next+index*JP_VMU_BLOCK_BYTES,in,JP_VMU_BLOCK_BYTES);e=persist(drive,next,1);}
    if(!e){free(media.images[drive]);media.images[drive]=next;media.info.block_writes++;}else free(next);
    return e;
}
