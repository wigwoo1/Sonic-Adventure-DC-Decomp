/* Authored native replacement for the hardware-bound memory loader. Original
 * list dispatch, texture parameter building, record entry/release and bitmap
 * queue bodies are compiled separately from the pinned external dependency.
 * No disc file/cache provider or silent format conversion is supplied here. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#undef TRUE
#undef FALSE
#include "jp_native_textures.h"
#include "jp_native_texture_upload.h"
#include "jp_native_video_state.h"
#include <kamui.h>

static const unsigned char *source;
static size_t source_bytes;
static DWORD owner;
static int error;
Uint32 _nj_user_globalIndex=0xffffffef;
static int fail(int value){if(!error)error=value;return value;}
static int thread_ok(void){return !owner || owner==GetCurrentThreadId();}
int jp_memory_status(void){return thread_ok()?error:JP_MEMORY_THREAD;}
void jp_memory_clear_error(void){if(thread_ok())error=JP_MEMORY_OK;}
int jp_memory_bind_source(const void *pixels,size_t bytes)
{
    if(!thread_ok())return JP_MEMORY_THREAD;
    if(error)return error;
    if(source)return fail(JP_MEMORY_STATE);
    if(!pixels || !bytes || bytes>UINTPTR_MAX-(uintptr_t)pixels)return fail(JP_MEMORY_ARGUMENT);
    owner=GetCurrentThreadId();source=pixels;source_bytes=bytes;return JP_MEMORY_OK;
}
int jp_memory_unbind_source(void)
{
    if(!thread_ok())return JP_MEMORY_THREAD;
    source=NULL;source_bytes=0;return JP_MEMORY_OK;
}
int jp_memory_shutdown(void)
{
    if(!thread_ok())return JP_MEMORY_THREAD;
    if(source)return fail(JP_MEMORY_STATE);
    owner=0;error=JP_MEMORY_OK;return JP_MEMORY_OK;
}
int jp_memory_texture_error(void)
{return thread_ok()?fail(JP_MEMORY_TEXTURE):JP_MEMORY_THREAD;}
static int contains(const void *p,size_t n)
{
    uintptr_t a=(uintptr_t)source,b=(uintptr_t)p;
    return source && b>=a && b-a<=source_bytes && n<=source_bytes-(size_t)(b-a);
}
static uint32_t read32(const void *p){uint32_t v;memcpy(&v,p,4);return v;}
static uint16_t read16(const void *p){uint16_t v;memcpy(&v,p,2);return v;}
static unsigned code(unsigned n){unsigned c=0;while(n>8){n>>=1;++c;}return c;}
static int side(unsigned n){return n>=8 && n<=1024 && !(n&(n-1));}
static int describe(unsigned type,unsigned w,unsigned h,NJS_TEXINFO *out,size_t *input,size_t *prefix)
{
    static const unsigned off16[11]={6,8,16,48,176,688,2736,10928,43696,174768,699056};
    static const unsigned off8[11]={3,4,8,24,88,344,1368,5464,21848,87384,349528};
    static const unsigned off4[11]={1,2,4,12,44,172,684,2732,10924,43692,174764};
    static const unsigned vqend[11]={1,2,6,22,86,342,1366,5462,21846,87382,349526};
    if(!side(w)||!side(h)||type>0xffff)return JP_MEMORY_ARGUMENT;
    unsigned kind=type&0xff00,fmt=type&255,flags=0,pixel=fmt,log=code(w)+3;
    size_t count=(size_t)w*h,bytes=0,pad=0;
    if(kind!=0x500 && kind!=0x600 && kind!=0x700 && kind!=0x800 && fmt>2)return JP_MEMORY_UNSUPPORTED;
    switch(kind){
    case 0x100:case 0xd00:flags=4;bytes=count*2;break;
    case 0x900:bytes=count*2;break;
    case 0x200:flags=5;bytes=off16[log]+count*2;pad=4;break;
    case 0x300:flags=12;bytes=2048+count/4;break;
    case 0x400:flags=13;bytes=2048+vqend[log-1]+count/4;break;
    case 0x500:flags=36;pixel=5;bytes=count/2;break;
    case 0x600:flags=37;pixel=5;bytes=off4[log]+count/2;break;
    case 0x700:flags=36;pixel=6;bytes=count;break;
    case 0x800:flags=37;pixel=6;bytes=off8[log]+count;break;
    default:return JP_MEMORY_UNSUPPORTED;
    }
    if(w!=h && kind!=0x900 && kind!=0xd00 && kind!=0x500 && kind!=0x700)return JP_MEMORY_ARGUMENT;
    NJS_TEXINFO v={0};v.texsurface.Type=(type<<16)|KM_SURFACETYPE_TEXTURE;
    v.texsurface.BitDepth=KMI_BITDEPTH_16;v.texsurface.PixelFormat=pixel<<27;
    v.texsurface.nWidth=w;v.texsurface.nHeight=h;v.texsurface.TextureSize=(Uint32)bytes;v.texsurface.fSurfaceFlags=flags;
    *out=v;*input=bytes-pad;*prefix=pad;return JP_MEMORY_OK;
}
int jp_memory_raw_size(unsigned type,unsigned w,unsigned h,size_t *bytes)
{
    if(!thread_ok())return JP_MEMORY_THREAD;
    if(!bytes)return fail(JP_MEMORY_ARGUMENT);
    NJS_TEXINFO v;size_t n,pad;int rc=describe(type,w,h,&v,&n,&pad);
    if(rc)return fail(rc);
    *bytes=n;return JP_MEMORY_OK;
}
static Sint32 rejected(unsigned n,unsigned gid,int reason)
{
    fail(reason);_nj_texerr_.n=n;_nj_texerr_.globalIndex=gid;_nj_texerr_.texerr=NJD_TEXERR_OTHER;return -1;
}
/* Bounded chunk walk. A PVRT size includes its eight-byte type/dimensions
 * payload header. Unknown chunks and GBIX must fit completely before skipping.
 * Explicit global indices also permit raw pixels (the original bitmap path).
 */
static int parse(NJS_TEXINFO *info,unsigned explicit_index,unsigned *gid,NJS_TEXINFO *out,const void **payload,size_t *bytes,size_t *pad)
{
    if(!info || !contains(info->texaddr,4))return JP_MEMORY_SOURCE;
    const unsigned char *p=(const unsigned char *)info->texaddr;
    unsigned type=info->texsurface.Type>>16,w=info->texsurface.nWidth,h=info->texsurface.nHeight;
    uint32_t tag=read32(p);int indexed=explicit_index;
    if(!explicit_index || tag==0x58494247 || tag==0x54525650){
        for(;;){
            if(!contains(p,8))return JP_MEMORY_SOURCE;
            tag=read32(p);uint32_t length=read32(p+4);
            if(!contains(p+8,length))return JP_MEMORY_SOURCE;
            if(tag==0x54525650){
                if(length<8)return JP_MEMORY_SOURCE;
                type=read32(p+8);w=read16(p+12);h=read16(p+14);
                int rc=describe(type,w,h,out,bytes,pad);if(rc)return rc;
                if(*bytes>(size_t)length-8)return JP_MEMORY_SOURCE;
                *payload=p+16;break;
            }
            if(tag==0x58494247){
                if(length<4)return JP_MEMORY_SOURCE;
                if(!explicit_index){*gid=read32(p+8);indexed=1;}
            }
            p+=8+(size_t)length;
        }
    }else{
        int rc=describe(type,w,h,out,bytes,pad);if(rc)return rc;
        if(!contains(p,*bytes))return JP_MEMORY_SOURCE;
        *payload=p;
    }
    if(!indexed){
        while(njSearchTexList(_nj_texmemlist,_nj_texmemsize,_nj_user_globalIndex)>=0){
            if(!_nj_user_globalIndex)return JP_MEMORY_STATE;
            --_nj_user_globalIndex;
        }
        *gid=_nj_user_globalIndex;
    }
    return *gid==UINT32_MAX?JP_MEMORY_ARGUMENT:JP_MEMORY_OK;
}
static Sint32 share(NJS_TEXNAME *name,unsigned n,NJS_TEXMEMLIST *existing,unsigned bank)
{
    unsigned gid=existing->globalIndex;
    if(!(existing->texaddr&TEXMEM_LOADED) || (existing->texaddr&TEXMEM_CACHED) ||
        !jp_texture_surface_owned(&existing->texinfo.texsurface))return rejected(n,gid,JP_MEMORY_TEXTURE);
    unsigned kind=(existing->texinfo.texsurface.Type>>16)&0xff00;
    int pal=kind==0x500||kind==0x600||kind==0x700||kind==0x800;
    if(!pal)bank=UINT32_MAX;
    Sint32 same=pal?njSearchTexList3(_nj_texmemlist,_nj_texmemsize,gid,bank):(Sint32)jp_texture_record_index(existing);
    if(same>=0){
        NJS_TEXMEMLIST *record=&_nj_texmemlist[same];
        if(record->count==UINT16_MAX || !(record->texaddr&TEXMEM_LOADED) || (record->texaddr&TEXMEM_CACHED))return rejected(n,gid,JP_MEMORY_TEXTURE);
        if(jp_texture_store_handle(&name->texaddr,record))return rejected(n,gid,JP_MEMORY_TEXTURE);
        record->count++;return 1;
    }
    if(!jp_texture_can_enter(&name->texaddr,&existing->texinfo))return rejected(n,gid,JP_MEMORY_TEXTURE);
    njEntryTexMemList(&name->texaddr,&existing->texinfo,gid,TEXMEM_LOADED,bank);
    NJS_TEXMEMLIST *record=jp_memory_owner_bound(&name->texaddr);
    if(!record)return rejected(n,gid,JP_MEMORY_TEXTURE);
    record->dummy=0;njSetTextureParam(record);njSetPaletteBankLN(record,(Sint32)bank);return 1;
}
Sint32 njLoadMemoryTexture(NJS_TEXLIST *list,Uint32 n)
{
    if(!thread_ok())return -1;
    if(error || jp_textures_status() || !list || !list->textures || n>=list->nbTexture)return rejected(n,UINT32_MAX,JP_MEMORY_STATE);
    NJS_TEXNAME *name=&list->textures[n];unsigned attr=name->attr,gid=name->texaddr;
    if(!(attr&NJD_TEXATTR_TYPE_MEMORY) || (attr&(NJD_TEXATTR_READAREA_MASK|NJD_TEXATTR_AUTOMIPMAP)))return rejected(n,gid,JP_MEMORY_UNSUPPORTED);
    if(!jp_memory_owner_can_bind(&name->texaddr))return rejected(n,gid,JP_MEMORY_STATE);
    unsigned bank=(gid&NJD_TEXBANK_MASK)>>NJD_TEXBANK_SHIFT;
    if(attr&NJD_TEXATTR_TEXCONTINUE){
        NJS_TEXMEMLIST *previous=n?jp_memory_owner_bound(&list->textures[n-1].texaddr):NULL;
        return previous?share(name,n,previous,bank):rejected(n,gid,JP_MEMORY_STATE);
    }
    unsigned indexed=attr&(NJD_TEXATTR_GLOBALINDEX|NJD_TEXATTR_PALGLOBALINDEX);
    if(attr&NJD_TEXATTR_PALGLOBALINDEX)gid&=NJD_TEXPALGLOBAL_MASK;
    else if(attr&NJD_TEXATTR_GLOBALINDEX)bank=0;
    Sint32 index=indexed?njSearchTexList(_nj_texmemlist,_nj_texmemsize,gid):-1;
    if(gid==UINT32_MAX && indexed)return rejected(n,gid,JP_MEMORY_ARGUMENT);
    if(index>=0)return share(name,n,&_nj_texmemlist[index],bank);
    NJS_TEXINFO info;const void *payload;size_t bytes,pad;
    int rc=parse((NJS_TEXINFO *)name->filename,indexed,&gid,&info,&payload,&bytes,&pad);
    if(rc)return rejected(n,gid,rc);
    if(info.texsurface.fSurfaceFlags&KM_SURFACEFLAGS_PALETTIZED){
        uint32_t palette_mode;
        if(jp_video_read(0x108,&palette_mode))return rejected(n,gid,JP_MEMORY_TEXTURE);
        if(palette_mode==3)info.texsurface.BitDepth=KMI_BITDEPTH_32;
    }else bank=UINT32_MAX;
    index=njSearchTexList(_nj_texmemlist,_nj_texmemsize,gid);
    if(index>=0)return share(name,n,&_nj_texmemlist[index],bank);
    if(!jp_texture_can_enter(&name->texaddr,&info))return rejected(n,gid,JP_MEMORY_TEXTURE);
    /* Insert the four-byte surface prefix missing from 16-bit PVR mip chains.
     * Copy first, then enter the SDK record. Failure leaves no live binding. */
    void *normalized=NULL;const void *pixels=payload;
    if(pad){normalized=calloc(1,bytes+pad);if(!normalized)return rejected(n,gid,JP_MEMORY_TEXTURE);memcpy((unsigned char *)normalized+pad,payload,bytes);pixels=normalized;}
    rc=jp_texture_copy_surface(&info,pixels,bytes+pad);free(normalized);
    if(rc)return rejected(n,gid,JP_MEMORY_TEXTURE);
    njEntryTexMemList(&name->texaddr,&info,gid,TEXMEM_LOADED,bank);
    NJS_TEXMEMLIST *record=jp_memory_owner_bound(&name->texaddr);
    if(!record)return rejected(n,gid,JP_MEMORY_TEXTURE);
    record->dummy=0;njSetTextureParam(record);njSetPaletteBankLN(record,(Sint32)bank);return 1;
}
Sint32 njLoadTextureLN(NJS_TEXLIST *list,Uint32 n)
{
    if(!thread_ok())return -1;
    if(!list || !list->textures || n>=list->nbTexture)return rejected(n,UINT32_MAX,JP_MEMORY_ARGUMENT);
    if(!(list->textures[n].attr&NJD_TEXATTR_TYPE_MEMORY))return rejected(n,list->textures[n].texaddr,JP_MEMORY_UNSUPPORTED);
    return njLoadMemoryTexture(list,n);
}
int jp_memory_load_list(NJS_TEXLIST *list,const void *pixels,size_t bytes)
{
    if(jp_memory_bind_source(pixels,bytes))return -1;
    int result=njLoadTexture(list);jp_memory_unbind_source();return result;
}
