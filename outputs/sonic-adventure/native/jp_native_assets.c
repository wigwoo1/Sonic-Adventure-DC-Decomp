#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#undef TRUE
#undef FALSE
#include "jp_native_textures.h"
#include "jp_native_assets.h"
#include "prskit.h"
#define ASSET_SCRATCH 512
#define ASSET_PATH 32768
static struct {
    wchar_t *root;NJS_TEXNAME *residents,*scratch;char *names;
    void *data;struct jp_asset_limits limits;struct jp_asset_info info;
    NJS_TEXLIST pending;DWORD owner;int initialized,busy;
} assets;
static int asset_owner(void){return !assets.initialized?JP_ASSET_STATE:assets.owner!=GetCurrentThreadId()?JP_ASSET_THREAD:JP_ASSET_OK;}
static int asset_fail(int code){if(!assets.info.status)assets.info.status=(uint32_t)code;return code;}
static int asset_ready(void){int rc=asset_owner();return rc?rc:(int)assets.info.status;}
static int os_fail(int code){assets.info.os_error=GetLastError();return asset_fail(code);}
static uint32_t resident_count(void)
{uint32_t n=0;for(uint32_t i=0;i<assets.limits.resident_names;i++)if(jp_memory_owner_bound(&assets.residents[i].texaddr))++n;return n;}
int jp_assets_status(void){int rc=asset_owner();return rc?rc:(int)assets.info.status;}
void jp_assets_clear_error(void){if(!asset_owner()){assets.info.status=0;assets.info.os_error=assets.info.prs_error=0;}}
size_t jp_assets_info_size(void){return sizeof(struct jp_asset_info);}
int jp_assets_snapshot(struct jp_asset_info *out)
{
    int rc=asset_owner();if(rc)return rc;if(!out)return JP_ASSET_ARGUMENT;
    struct jp_asset_info info=assets.info;info.resident_refs=resident_count();info.pending_refs=jp_pvm_loaded_count(&assets.pending);*out=info;return JP_ASSET_OK;
}
static int valid_relative(const char *name,size_t *length)
{
    if(!name)return 0;size_t n=0,start=0;
    for(;n<1024 && name[n];n++){
        unsigned c=(unsigned char)name[n];
        if(c=='/'||c=='\\'){
            if(n==start || name[n-1]=='.')return 0;start=n+1;
        }else if(!((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='_'||c=='-'||c=='.'))return 0;
    }
    if(!n||n==1024||n==start||name[n-1]=='.')return 0;
    *length=n;return 1;
}
int jp_assets_make_prs_path(const char *name,char *out,size_t bytes)
{
    int rc=asset_ready();if(rc)return rc;size_t n;
    if(!out||!valid_relative(name,&n)||n>SIZE_MAX-5||bytes<n+5)return asset_fail(JP_ASSET_ARGUMENT);
    memcpy(out,name,n);memcpy(out+n,".PRS",5);return JP_ASSET_OK;
}
int jp_assets_initialize(const wchar_t *root,const struct jp_asset_limits *limits)
{
    if(assets.initialized)return asset_owner()?asset_owner():asset_fail(JP_ASSET_STATE);
    if(!root||!limits||!limits->file_bytes||!limits->decoded_bytes||!limits->buffer_bytes||limits->resident_names>32767)return JP_ASSET_ARGUMENT;
    HANDLE dir=CreateFileW(root,FILE_READ_ATTRIBUTES,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
    if(dir==INVALID_HANDLE_VALUE)return JP_ASSET_OPEN;
    BY_HANDLE_FILE_INFORMATION fi;wchar_t final[ASSET_PATH];
    DWORD count=GetFinalPathNameByHandleW(dir,final,ASSET_PATH,FILE_NAME_NORMALIZED);
    int valid=GetFileInformationByHandle(dir,&fi) && (fi.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) && count && count<ASSET_PATH-2;CloseHandle(dir);
    if(!valid)return JP_ASSET_OPEN;
    if(final[count-1]!=L'\\')final[count++]=L'\\';final[count]=0;
    size_t root_bytes=((size_t)count+1)*sizeof(wchar_t),resident_bytes=(size_t)limits->resident_names*sizeof(NJS_TEXNAME),scratch_bytes=ASSET_SCRATCH*sizeof(NJS_TEXNAME),name_bytes=ASSET_SCRATCH*PVM_NAME_SIZE;
    size_t base=root_bytes+resident_bytes+scratch_bytes+name_bytes;if(base>limits->buffer_bytes)return JP_ASSET_LIMIT;
    wchar_t *owned_root=malloc(root_bytes);NJS_TEXNAME *resident=limits->resident_names?calloc(limits->resident_names,sizeof *resident):NULL,*scratch=calloc(ASSET_SCRATCH,sizeof *scratch);char *names=malloc(name_bytes);
    if(!owned_root||(limits->resident_names&&!resident)||!scratch||!names){free(owned_root);free(resident);free(scratch);free(names);return JP_ASSET_ALLOCATION;}
    memset(&assets,0,sizeof assets);memcpy(owned_root,final,root_bytes);assets.root=owned_root;assets.residents=resident;assets.scratch=scratch;assets.names=names;assets.limits=*limits;assets.info.owned_bytes=base;assets.owner=GetCurrentThreadId();assets.initialized=1;return JP_ASSET_OK;
}
int jp_assets_release_data(void)
{
    int rc=asset_owner();if(rc)return rc;if(assets.busy)return asset_fail(JP_ASSET_STATE);
    free(assets.data);assets.data=NULL;assets.info.owned_bytes-=assets.info.decoded_bytes;assets.info.decoded_bytes=0;return JP_ASSET_OK;
}
int jp_assets_read_prs(const char *relative,const void **out,size_t *bytes)
{
    int rc=asset_ready();if(rc)return rc;
    if(assets.busy||assets.data||jp_pvm_loaded_count(&assets.pending))return asset_fail(JP_ASSET_STATE);
    size_t length;if(!out||!bytes||!valid_relative(relative,&length))return asset_fail(JP_ASSET_ARGUMENT);
    wchar_t path[ASSET_PATH],actual[ASSET_PATH];size_t root=(size_t)lstrlenW(assets.root);
    if(length>=ASSET_PATH-root)return asset_fail(JP_ASSET_LIMIT);
    memcpy(path,assets.root,root*sizeof(wchar_t));for(size_t i=0;i<length;i++)path[root+i]=relative[i]=='/'?L'\\':(wchar_t)(unsigned char)relative[i];path[root+length]=0;
    HANDLE file=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    if(file==INVALID_HANDLE_VALUE)return os_fail(JP_ASSET_OPEN);
    DWORD final=GetFinalPathNameByHandleW(file,actual,ASSET_PATH,FILE_NAME_NORMALIZED);LARGE_INTEGER size;
    if(!final||final>=ASSET_PATH || final<=root || CompareStringOrdinal(actual,(int)root,assets.root,(int)root,TRUE)!=CSTR_EQUAL){CloseHandle(file);return asset_fail(JP_ASSET_ARGUMENT);}
    if(!GetFileSizeEx(file,&size)){assets.info.os_error=GetLastError();CloseHandle(file);return asset_fail(JP_ASSET_READ);}
    if(size.QuadPart<=0 || (uint64_t)size.QuadPart>assets.limits.file_bytes || (uint64_t)size.QuadPart>SIZE_MAX || (size_t)size.QuadPart>assets.limits.buffer_bytes-assets.info.owned_bytes){CloseHandle(file);return asset_fail(JP_ASSET_LIMIT);}
    size_t input_bytes=(size_t)size.QuadPart;unsigned char *input=malloc(input_bytes);if(!input){CloseHandle(file);return asset_fail(JP_ASSET_ALLOCATION);}
    assets.busy=1;size_t read=0;
    while(read<input_bytes){DWORD got=0,part=(DWORD)((input_bytes-read)>(1u<<20)?(1u<<20):input_bytes-read);if(!ReadFile(file,input+read,part,&got,NULL)||!got){rc=os_fail(JP_ASSET_READ);break;}read+=got;}
    LARGE_INTEGER after;if(!rc && (!GetFileSizeEx(file,&after)||after.QuadPart!=size.QuadPart))rc=os_fail(JP_ASSET_READ);CloseHandle(file);
    if(rc){free(input);assets.busy=0;return rc;}
    assets.info.files_read++;assets.info.bytes_read+=input_bytes;
    size_t decoded=0,at=0;int prs=prskit_decompress(input,input_bytes,NULL,0,&decoded,&at);assets.info.prs_error=(uint32_t)prs;
    if(prs!=PRSKIT_OK && prs!=PRSKIT_TRAILING){free(input);assets.busy=0;return asset_fail(JP_ASSET_PRS);}
    if(!decoded||decoded>assets.limits.decoded_bytes||decoded>assets.limits.buffer_bytes-assets.info.owned_bytes-input_bytes){free(input);assets.busy=0;return asset_fail(JP_ASSET_LIMIT);}
    void *result=malloc(decoded);if(!result){free(input);assets.busy=0;return asset_fail(JP_ASSET_ALLOCATION);}
    size_t written=0;prs=prskit_decompress(input,input_bytes,result,decoded,&written,&at);free(input);assets.busy=0;assets.info.prs_error=(uint32_t)prs;
    if((prs!=PRSKIT_OK && prs!=PRSKIT_TRAILING)||written!=decoded){free(result);return asset_fail(JP_ASSET_PRS);}
    assets.data=result;assets.info.decoded_bytes=decoded;assets.info.owned_bytes+=decoded;assets.info.decodes++;assets.info.bytes_decoded+=decoded;*out=result;*bytes=decoded;return JP_ASSET_OK;
}
static void pending(NJS_TEXLIST *list)
{if(list && list->nbTexture<=32767 && jp_pvm_loaded_count(list))assets.pending=*list;}
int jp_assets_load_prs(const char *relative,NJS_TEXLIST *out,uint32_t capacity,size_t filename_bytes)
{
    int rc=asset_ready();if(rc)return -1;
    if(!out||(capacity&&!out->textures)||capacity>32767 || assets.pending.nbTexture)return asset_fail(JP_ASSET_ARGUMENT),-1;
    for(uint32_t i=0;i<capacity;i++)if(jp_memory_owner_bound(&out->textures[i].texaddr))return asset_fail(JP_ASSET_STATE),-1;
    const void *data;size_t bytes;if(jp_assets_read_prs(relative,&data,&bytes))return -1;
    int result=jp_pvm_load_memory(out,capacity,filename_bytes,data,bytes);jp_assets_release_data();
    if(result<0){pending(out);asset_fail(JP_ASSET_PVM);}else assets.info.pvm_loads++;
    return result;
}
int jp_assets_prepare_game(NJS_TEXLIST *out,NJS_TEXNAME *scratch,uint32_t capacity)
{
    int rc=asset_ready();if(rc)return rc;
    if(!out||!scratch||capacity>32767||out->nbTexture>capacity||(out->nbTexture&&!out->textures)||assets.pending.nbTexture)return asset_fail(JP_ASSET_ARGUMENT);
    uintptr_t a=(uintptr_t)scratch,b=(uintptr_t)out->textures,c=(uintptr_t)out;
    size_t na=(size_t)capacity*sizeof *scratch,nb=(size_t)out->nbTexture*sizeof *scratch;
    if(na>UINTPTR_MAX-a||nb>UINTPTR_MAX-b||sizeof *out>UINTPTR_MAX-c || (nb && a<b+nb && b<a+na) || (c<a+na && a<c+sizeof *out) || (nb && c<b+nb && b<c+sizeof *out))return asset_fail(JP_ASSET_ARGUMENT);
    for(uint32_t i=0;i<capacity;i++)if(jp_memory_owner_bound(&scratch[i].texaddr))return asset_fail(JP_ASSET_STATE);
    for(uint32_t i=0;i<out->nbTexture;i++)if(jp_memory_owner_bound(&out->textures[i].texaddr))return asset_fail(JP_ASSET_STATE);
    for(uint32_t i=0;i<capacity;i++){scratch[i].attr=0;scratch[i].texaddr=0;}return JP_ASSET_OK;
}
static NJS_TEXNAME *resident_for(NJS_TEXMEMLIST *record)
{for(uint32_t i=0;i<assets.limits.resident_names;i++)if(jp_memory_owner_bound(&assets.residents[i].texaddr)==record)return &assets.residents[i];return NULL;}
int jp_assets_commit_game(NJS_TEXLIST *scratch,NJS_TEXLIST *out)
{
    int rc=asset_ready();if(rc)return -1;
    if(!scratch||!out||scratch->nbTexture>32767||out->nbTexture>scratch->nbTexture||(scratch->nbTexture&&!scratch->textures))return pending(scratch),asset_fail(JP_ASSET_ARGUMENT),-1;
    if(jp_texture_validate_move(scratch->textures,scratch->textures,scratch->nbTexture))return pending(scratch),asset_fail(JP_ASSET_TEXTURE),-1;
    if(jp_texture_validate_move(scratch->textures,out->textures,out->nbTexture))return pending(scratch),asset_fail(JP_ASSET_TEXTURE),-1;
    uint32_t need=0,available=assets.limits.resident_names-resident_count();
    for(Uint32 i=out->nbTexture;i<scratch->nbTexture;i++){
        NJS_TEXMEMLIST *record=jp_memory_owner_bound(&scratch->textures[i].texaddr);if(!record)return pending(scratch),asset_fail(JP_ASSET_TEXTURE),-1;
        if(!resident_for(record)){
            int duplicate=0;for(Uint32 j=out->nbTexture;j<i;j++)if(jp_memory_owner_bound(&scratch->textures[j].texaddr)==record){duplicate=1;break;}
            if(!duplicate)need++;
        }
    }
    if(need>available)return pending(scratch),asset_fail(JP_ASSET_LIMIT),-1;
    if(jp_texture_move_names(scratch->textures,out->textures,out->nbTexture))return pending(scratch),asset_fail(JP_ASSET_TEXTURE),-1;
    for(Uint32 i=out->nbTexture;i<scratch->nbTexture;i++){
        NJS_TEXNAME *name=&scratch->textures[i];NJS_TEXMEMLIST *record=jp_memory_owner_bound(&name->texaddr);
        if(resident_for(record)){
            Uint32 index=record->globalIndex;if(name->attr&NJD_TEXATTR_PALGLOBALINDEX)index|=(record->bank&63)<<NJD_TEXBANK_SHIFT;
            if(njReleaseTextureLN(name->texaddr)<0)return pending(scratch),asset_fail(JP_ASSET_TEXTURE),-1;
            jp_texture_forget_field(&name->texaddr);name->texaddr=index;
        }else{
            uint32_t slot=0;while(slot<assets.limits.resident_names && jp_memory_owner_bound(&assets.residents[slot].texaddr))slot++;
            if(slot==assets.limits.resident_names || jp_texture_move_names(name,&assets.residents[slot],1))return pending(scratch),asset_fail(JP_ASSET_TEXTURE),-1;
        }
    }
    assets.info.transfers++;return 1;
}
int jp_assets_load_game_file(const char *relative,NJS_TEXLIST *out)
{
    if(jp_assets_prepare_game(out,assets.scratch,ASSET_SCRATCH))return -1;
    NJS_TEXLIST loaded={0};njSetPvmTextureList(&loaded,assets.scratch,assets.names,ASSET_SCRATCH);
    if(jp_assets_load_prs(relative,&loaded,ASSET_SCRATCH,PVM_NAME_SIZE)<0)return -1;
    return jp_assets_commit_game(&loaded,out);
}
int jp_assets_recover_pending(void)
{
    int rc=asset_ready();if(rc)return rc;
    if(assets.pending.nbTexture && jp_pvm_release_loaded(&assets.pending)<0)return asset_fail(JP_ASSET_TEXTURE);
    memset(&assets.pending,0,sizeof assets.pending);return JP_ASSET_OK;
}
int jp_assets_release_residents(void)
{
    int rc=asset_ready();if(rc)return rc;
    NJS_TEXLIST list={assets.residents,assets.limits.resident_names};
    if(jp_pvm_release_loaded(&list)<0)return asset_fail(JP_ASSET_TEXTURE);return JP_ASSET_OK;
}
int jp_assets_shutdown(void)
{
    int rc=asset_ready();if(rc)return rc;
    if(assets.busy||assets.data||jp_pvm_loaded_count(&assets.pending))return asset_fail(JP_ASSET_STATE);
    if(jp_assets_release_residents())return (int)assets.info.status;
    free(assets.root);free(assets.residents);free(assets.scratch);free(assets.names);memset(&assets,0,sizeof assets);return JP_ASSET_OK;
}
