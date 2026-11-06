/* Native persistence boundary for the original Shinobi flash packet manager.
 * Partition facts: KallistiOS dc/flashrom.h; Flycast DCFlashChip geometry.
 * No emulator implementation or retail flash contents are copied here. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "jp_native_flash.h"
#define IMAGE_BYTES UINT32_C(131072)
static const int32_t starts[5]={0x1a000,0x18000,0x1c000,0x10000,0};
static const int32_t lengths[5]={0x2000,0x2000,0x4000,0x8000,0x10000};
static struct {
    wchar_t directory[32768];HANDLE lock;unsigned char *image;
    DWORD thread;uint64_t sequence;int fm_call;
    struct jp_flash_info info;
} flash;
int jp_flash_owner_access(void)
{return !flash.image?JP_FLASH_NOT_OPEN:flash.thread!=GetCurrentThreadId()?JP_FLASH_THREAD:0;}
static int fail(int code,DWORD os)
{flash.info.status=(uint32_t)code;flash.info.os_error=os;return code;}
int jp_flash_argument_error(void){return fail(JP_FLASH_ARGUMENT,0);}
int jp_flash_status(void)
{int rc=jp_flash_owner_access();return rc?rc:(int)flash.info.status;}
void jp_flash_clear_error(void)
{if(!jp_flash_owner_access()&&!flash.fm_call){flash.info.status=0;flash.info.os_error=0;}}
int jp_flash_snapshot(struct jp_flash_info *out)
{int rc=jp_flash_owner_access();if(rc)return rc;if(!out)return JP_FLASH_ARGUMENT;*out=flash.info;return 0;}
int jp_flash_fm_begin(void)
{int rc=jp_flash_owner_access();if(rc)return rc;if(flash.fm_call)return JP_FLASH_BUSY;flash.fm_call=1;return 0;}
void jp_flash_fm_end(void){flash.fm_call=0;}
static size_t prefix(wchar_t *path)
{
    size_t n=wcslen(flash.directory);memcpy(path,flash.directory,n*sizeof *path);
    if(n&&path[n-1]!=L'/'&&path[n-1]!=L'\\')path[n++]=L'\\';return n;
}
static void filename(wchar_t *path,int lock)
{size_t n=prefix(path);const wchar_t *name=lock?L"SONIC_FLASH.DAT.lock":L"SONIC_FLASH.DAT";memcpy(path+n,name,(wcslen(name)+1)*sizeof *path);}
static int load(unsigned char *out)
{
    wchar_t path[32768];filename(path,0);
    HANDLE h=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if(h==INVALID_HANDLE_VALUE)return fail(JP_FLASH_IO,GetLastError());
    LARGE_INTEGER size;DWORD got=0,os=0;int code=0;
    if(!GetFileSizeEx(h,&size)){code=JP_FLASH_IO;os=GetLastError();}
    else if(size.QuadPart!=IMAGE_BYTES)code=JP_FLASH_FORMAT;
    else if(!ReadFile(h,out,IMAGE_BYTES,&got,NULL)){code=JP_FLASH_IO;os=GetLastError();}
    else if(got!=IMAGE_BYTES){code=JP_FLASH_IO;os=ERROR_HANDLE_EOF;}
    if(!CloseHandle(h)&&!code){code=JP_FLASH_IO;os=GetLastError();}
    return code?fail(code,os):0;
}
static void hex(wchar_t *out,uint64_t n,unsigned count)
{for(unsigned i=0;i<count;i++)out[i]=L"0123456789ABCDEF"[(n>>((count-i-1)*4))&15];}
static int persist(const unsigned char *image,int replace)
{
    wchar_t path[32768],temp[32768];filename(path,0);HANDLE h=INVALID_HANDLE_VALUE;
    for(unsigned i=0;i<16;i++){
        size_t n=prefix(temp);memcpy(temp+n,L".flash-",7*sizeof *temp);n+=7;
        hex(temp+n,GetCurrentProcessId(),8);n+=8;hex(temp+n,++flash.sequence,16);n+=16;
        memcpy(temp+n,L".tmp",5*sizeof *temp);
        h=CreateFileW(temp,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
        if(h!=INVALID_HANDLE_VALUE)break;
        DWORD os=GetLastError();if(os!=ERROR_FILE_EXISTS&&os!=ERROR_ALREADY_EXISTS)return fail(JP_FLASH_IO,os);
    }
    if(h==INVALID_HANDLE_VALUE)return fail(JP_FLASH_IO,ERROR_FILE_EXISTS);
    DWORD count=0,os=0;
    if(!WriteFile(h,image,IMAGE_BYTES,&count,NULL))os=GetLastError();
    else if(count!=IMAGE_BYTES)os=ERROR_WRITE_FAULT;
    else if(!FlushFileBuffers(h))os=GetLastError();
    if(!CloseHandle(h)&&!os)os=GetLastError();
    if(!os&&!MoveFileExW(temp,path,MOVEFILE_WRITE_THROUGH|(replace?MOVEFILE_REPLACE_EXISTING:0)))os=GetLastError();
    if(os){DeleteFileW(temp);return fail(JP_FLASH_IO,os);}return 0;
}
int jp_flash_open(const wchar_t *directory,uint32_t create)
{
    if(flash.image)return jp_flash_owner_access()?JP_FLASH_THREAD:JP_FLASH_STATE;
    if(!directory||!*directory||create>1)return JP_FLASH_ARGUMENT;
    wchar_t full[32768];DWORD n=GetFullPathNameW(directory,32768,full,NULL);
    if(!n||n>=32768-128)return JP_FLASH_ARGUMENT;
    DWORD attr=GetFileAttributesW(full);
    if(attr==INVALID_FILE_ATTRIBUTES||!(attr&FILE_ATTRIBUTE_DIRECTORY))return JP_FLASH_ARGUMENT;
    memset(&flash,0,sizeof flash);memcpy(flash.directory,full,(n+1)*sizeof *full);
    wchar_t path[32768];filename(path,1);
    flash.lock=CreateFileW(path,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_HIDDEN,NULL);
    if(flash.lock==INVALID_HANDLE_VALUE)return fail(JP_FLASH_BUSY,GetLastError());
    unsigned char *image=malloc(IMAGE_BYTES);int rc=0;
    if(!image)rc=fail(JP_FLASH_ALLOCATION,0);
    else {
        filename(path,0);DWORD attributes=GetFileAttributesW(path);DWORD os=GetLastError();
        if(attributes==INVALID_FILE_ATTRIBUTES&&os==ERROR_FILE_NOT_FOUND&&create){
            memset(image,255,IMAGE_BYTES);rc=persist(image,0);
        }else rc=load(image);
    }
    if(rc){free(image);CloseHandle(flash.lock);flash.lock=INVALID_HANDLE_VALUE;return rc;}
    flash.image=image;flash.thread=GetCurrentThreadId();
    flash.info=(struct jp_flash_info){.creator_thread=flash.thread,.opened=1};return 0;
}
int jp_flash_close(void)
{
    if(!flash.image)return 0;int rc=jp_flash_owner_access();if(rc)return rc;
    if(flash.fm_call)return JP_FLASH_BUSY;
    rc=jp_flash_fm_close();if(rc)return rc;
    if(!CloseHandle(flash.lock))return fail(JP_FLASH_IO,GetLastError());
    free(flash.image);memset(&flash,0,sizeof flash);return 0;
}
struct block_info {int32_t address,bytes;};
__declspec(dllexport) int32_t syBtFlGetInfo(int32_t part,struct block_info *out)
{
    if(jp_flash_status()||!out||part<0||part>=5)return -1;
    *out=(struct block_info){starts[part],lengths[part]};return 0;
}
static int region(int32_t address,int32_t bytes,const void *buffer)
{
    if(jp_flash_status())return -1;
    if(address<0||bytes<=0||(uint32_t)address>=IMAGE_BYTES||(uint32_t)bytes>IMAGE_BYTES-(uint32_t)address||
       !buffer||(uintptr_t)buffer>UINTPTR_MAX-(uint32_t)bytes){fail(JP_FLASH_ARGUMENT,0);return -1;}return 0;
}
__declspec(dllexport) int32_t syBtFlRead(int32_t address,void *out,int32_t bytes)
{
    if(region(address,bytes,out))return -1;
    memcpy(out,flash.image+address,(size_t)bytes);flash.info.reads++;return bytes;
}
static int modify(int32_t address,const void *buffer,int32_t bytes,int erase)
{
    if(region(address,bytes,buffer))return -1;
    if(jp_flash_fm_is_open()&&!flash.fm_call){fail(JP_FLASH_BUSY,0);return -1;}
    int part=-1;
    for(int i=0;i<5;i++)if(address>=starts[i]&&address<starts[i]+lengths[i]){part=i;break;}
    if(part<0||(uint32_t)bytes>(uint32_t)(starts[part]+lengths[part]-address)){fail(JP_FLASH_ARGUMENT,0);return -1;}
    if(part==0){fail(JP_FLASH_PROTECTED,0);return -1;}
    unsigned char *next=malloc(IMAGE_BYTES);
    if(!next){fail(JP_FLASH_ALLOCATION,0);return -1;}
    int rc=load(next);
    if(!rc&&memcmp(next,flash.image,IMAGE_BYTES))rc=fail(JP_FLASH_CHANGED,0);
    if(!rc){
        if(erase)memset(next+address,255,(size_t)bytes);
        else for(int32_t i=0;i<bytes;i++)next[address+i]&=((const unsigned char *)buffer)[i];
        rc=persist(next,1);
    }
    if(!rc){free(flash.image);flash.image=next;flash.info.commits++;
        if(erase)flash.info.erases++;else flash.info.programs++;
    }else free(next);
    return rc?-1:erase?0:bytes;
}
__declspec(dllexport) int32_t syBtFlWrite(int32_t address,void *buffer,int32_t bytes)
{return modify(address,buffer,bytes,0);}
__declspec(dllexport) int32_t syBtFlDeleteSct(int32_t address)
{
    if(jp_flash_status())return -1;
    for(int i=0;i<5;i++)if(address>=starts[i]&&address<starts[i]+lengths[i])
        return modify(starts[i],flash.image+starts[i],lengths[i],1);
    fail(JP_FLASH_ARGUMENT,0);return -1;
}
