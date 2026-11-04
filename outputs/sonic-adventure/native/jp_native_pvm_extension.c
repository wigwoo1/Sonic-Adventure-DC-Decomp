/* Append to the frozen authored memory-upload translation unit. Its source
 * view, error/thread owner and texture decoder are shared, never duplicated.
 * PVM metadata helpers are separately compiled from the pinned external SDK. */
#include "jp_native_pvm.h"
#include "jp_native_queue.h"
extern KMVERTEXCONTEXT *_nj_current_ctx_;
/* Defined/exported by the pinned SDK setter provider; its public header lacks
 * this particular declaration. Keep the exact original typed signature. */
KMSTATUS kmSetPaletteBankData(KMINT32 entry,KMINT32 count,PKMPALETTEDATA colors);
static struct {
    NJS_TEXLIST *list;uint32_t capacity;size_t filename_bytes,start,cursor;
    int busy;
} pvm;
int jp_pvm_destination_active(void){return pvm.list!=NULL;}
int jp_pvm_bind_destination(NJS_TEXLIST *list,uint32_t capacity,size_t filename_bytes)
{
    if(!thread_ok())return JP_MEMORY_THREAD;
    if(error)return error;
    if(pvm.list||pvm.busy)return fail(JP_MEMORY_STATE);
    if(!list || (capacity && !list->textures) || capacity>INT16_MAX)return fail(JP_MEMORY_ARGUMENT);
    owner=GetCurrentThreadId();pvm.list=list;pvm.capacity=capacity;pvm.filename_bytes=filename_bytes;return JP_MEMORY_OK;
}
int jp_pvm_unbind_destination(void)
{
    if(!thread_ok())return JP_MEMORY_THREAD;
    if(pvm.busy)return fail(JP_MEMORY_STATE);
    memset(&pvm,0,sizeof pvm);return JP_MEMORY_OK;
}
void njMemOpen(Uint8 *addr)
{
    if(!thread_ok() || error)return;
    if(!contains(addr,0)){fail(JP_MEMORY_SOURCE);return;}
    pvm.start=pvm.cursor=(size_t)((uintptr_t)addr-(uintptr_t)source);
}
void njMemRead(void *out,Sint32 bytes)
{
    if(!thread_ok() || error)return;
    if(bytes<0 || !out || !source || pvm.cursor>source_bytes || (size_t)bytes>source_bytes-pvm.cursor){fail(JP_MEMORY_SOURCE);return;}
    memcpy(out,source+pvm.cursor,(size_t)bytes);pvm.cursor+=(size_t)bytes;
}
void njMemFseek(Sint32 offset,Sint32 origin)
{
    if(!thread_ok() || error)return;
    size_t base;
    if(origin==0)base=pvm.start;else if(origin==1)base=pvm.cursor;else{fail(JP_MEMORY_ARGUMENT);return;}
    int64_t delta=(int64_t)offset;
    if(base>source_bytes || (delta<0 && (uint64_t)-delta>base-pvm.start) || (delta>=0 && (uint64_t)delta>source_bytes-base)){fail(JP_MEMORY_SOURCE);return;}
    pvm.cursor=delta<0?base-(size_t)-delta:base+(size_t)delta;
}
static int pvm_overlap(const void *a,size_t na,const void *b,size_t nb)
{
    uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;
    if(!na||!nb)return 0;
    if(na>UINTPTR_MAX-x || nb>UINTPTR_MAX-y)return 1;
    return x<y+nb && y<x+na;
}
/* Validate a PVPL before reads/writes. Mode 6 is ARGB8888 and consumes four
 * bytes per color regardless of bank (the upstream GCC repair). */
static int pvm_palette(const unsigned char *chunk,size_t length,int apply)
{
    if(length<8)return JP_MEMORY_SOURCE;
    unsigned mode=read16(chunk+8);int bank=(int16_t)read16(chunk+10);
    int offset=(int16_t)read16(chunk+12),count=(int16_t)read16(chunk+14);
    if(mode!=0 && mode!=1 && mode!=2 && mode!=6)return JP_MEMORY_UNSUPPORTED;
    int first=bank*16+offset;unsigned width=mode==6?4:2;
    if(bank<0||bank>63||offset<0||count<0||first<0||first>1024||count>1024-first)return JP_MEMORY_ARGUMENT;
    if((size_t)count*width>length-8)return JP_MEMORY_SOURCE;
    if(!apply)return JP_MEMORY_OK;
    KMPALETTEDATA colors={0};
    for(int i=0;i<count;i++)colors.dwPaletteData[first+i]=width==4?read32(chunk+16+(size_t)i*4):read16(chunk+16+(size_t)i*2);
    if(kmSetPaletteMode(mode==6?3:(int)mode)!=KMSTATUS_SUCCESS || kmSetPaletteBankData(first,count,&colors)!=KMSTATUS_SUCCESS || jp_video_status())return JP_MEMORY_TEXTURE;
    /* Fix the original mode-as-bank store. Texture records separately carry
     * their own encoded banks; this is the actual shared Ninja context field. */
    if(!_nj_current_ctx_)return JP_MEMORY_STATE;
    _nj_current_ctx_->PaletteBank=(Uint32)bank;return JP_MEMORY_OK;
}
struct pvm_entry { const unsigned char *texture; };
static int pvm_preflight(const unsigned char *addr,struct jp_pvm_header *header,struct pvm_entry **entries)
{
    if(!contains(addr,12) || read32(addr)!=0x484d5650)return JP_MEMORY_SOURCE;
    header->size=read32(addr+4);header->status=read16(addr+8);header->count=(Sint16)read16(addr+10);
    int count=header->count;Sint32 stride=njCalcPvmStatusSize(header->status);
    if(count<0 || (unsigned)count>pvm.capacity || stride<2 || stride>40)return JP_MEMORY_ARGUMENT;
    if(header->size<4 || !contains(addr+8,header->size) || (size_t)count*(size_t)stride>(size_t)header->size-4)return JP_MEMORY_SOURCE;
    NJS_TEXLIST *list=pvm.list;size_t names_bytes=(size_t)count*sizeof(NJS_TEXNAME);
    if(pvm_overlap(addr,source_bytes-(size_t)(addr-source),list,sizeof *list) || pvm_overlap(addr,source_bytes-(size_t)(addr-source),list->textures,names_bytes) || pvm_overlap(list,sizeof *list,list->textures,names_bytes))return JP_MEMORY_ARGUMENT;
    uint8_t *seen=count?calloc((size_t)count,1):NULL;
    struct pvm_entry *records=count?calloc((size_t)count,sizeof *records):NULL;
    if(count && (!seen||!records)){free(seen);free(records);return JP_MEMORY_TEXTURE;}
    int rc=JP_MEMORY_OK;
    for(int i=0;i<count;i++){
        const unsigned char *entry=addr+12+(size_t)i*(size_t)stride;
        unsigned number=read16(entry);
        if(number>=(unsigned)count || seen[number]){rc=JP_MEMORY_SOURCE;break;}seen[number]=1;
        NJS_TEXNAME *name=&list->textures[i];
        if((name->attr&(NJD_TEXATTR_READAREA_MASK|NJD_TEXATTR_AUTOMIPMAP))){rc=JP_MEMORY_UNSUPPORTED;break;}
        if(!jp_memory_owner_can_bind(&name->texaddr)){rc=JP_MEMORY_STATE;break;}
        if(header->status&1){
            if(pvm.filename_bytes<PVM_NAME_SIZE || !name->filename || pvm_overlap(name->filename,pvm.filename_bytes,source,source_bytes) || pvm_overlap(name->filename,pvm.filename_bytes,list,sizeof *list) || pvm_overlap(name->filename,pvm.filename_bytes,list->textures,names_bytes)){rc=JP_MEMORY_ARGUMENT;break;}
            for(int j=0;j<i;j++)if(pvm_overlap(name->filename,pvm.filename_bytes,list->textures[j].filename,pvm.filename_bytes)){rc=JP_MEMORY_ARGUMENT;break;}
            if(rc)break;
        }
        unsigned gid=(header->status&8)?read32(entry+(size_t)stride-4):name->texaddr;
        int banked=((header->status&0x400) && (header->status&8)) || (name->attr&NJD_TEXATTR_PALGLOBALINDEX);
        if((banked?gid&NJD_TEXPALGLOBAL_MASK:gid)==UINT32_MAX){rc=JP_MEMORY_ARGUMENT;break;}
    }
    free(seen);
    size_t position=(size_t)(addr-source)+(size_t)header->size+8;
    int texture=0;
    while(!rc && texture<count){
        if(position>source_bytes || source_bytes-position<8){rc=JP_MEMORY_SOURCE;break;}
        const unsigned char *chunk=source+position;uint32_t length=read32(chunk+4),tag=read32(chunk);
        if((size_t)length>source_bytes-position-8){rc=JP_MEMORY_SOURCE;break;}
        if(tag==0x4c505650)rc=pvm_palette(chunk,length,0);
        else if(tag==0x54525650){
            if(length<8){rc=JP_MEMORY_SOURCE;break;}
            NJS_TEXINFO info;size_t input,prefix;
            rc=describe(read32(chunk+8),read16(chunk+12),read16(chunk+14),&info,&input,&prefix);
            if(!rc && input>(size_t)length-8)rc=JP_MEMORY_SOURCE;
            records[texture++].texture=chunk;
        }
        position+=8+(size_t)length;
    }
    if(rc){free(records);return rc;}
    *entries=records;return JP_MEMORY_OK;
}
Sint32 njLoadTexturePvmMemory(Uint8 *addr,NJS_TEXLIST *list)
{
    if(!thread_ok())return -1;
    if(error || pvm.busy || !pvm.list || pvm.list!=list || jp_textures_status() || jp_video_status())return rejected(UINT32_MAX,UINT32_MAX,JP_MEMORY_STATE);
    struct jp_pvm_header header;struct pvm_entry *records=NULL;
    int rc=pvm_preflight(addr,&header,&records);
    if(rc)return rejected(UINT32_MAX,UINT32_MAX,rc);
    pvm.busy=1;njInitTextureErrStat();list->nbTexture=(Uint32)header.count;
    njMemOpen(addr);njMemFseek(12,0);njSetTexListToPvmM(list,njCalcPvmStatusSize(header.status),&header);
    size_t position=(size_t)((uintptr_t)addr-(uintptr_t)source)+(size_t)header.size+8;
    int texture=0,result=1;
    while(texture<header.count){
        const unsigned char *chunk=source+position;uint32_t length=read32(chunk+4),tag=read32(chunk);position+=8+(size_t)length;
        if(tag==0x4c505650){rc=pvm_palette(chunk,length,1);if(rc){result=rejected(UINT32_MAX,UINT32_MAX,rc);break;}}
        else if(tag==0x54525650){
            Sint32 n=njSearchEntryCount(list,(Uint32)texture);NJS_TEXNAME *name=&list->textures[n];
            void *filename=name->filename;Uint32 attr=name->attr,index=name->texaddr;
            NJS_TEXINFO info={0};info.texaddr=(void *)records[texture].texture;
            Uint32 native_attr=attr|NJD_TEXATTR_TYPE_MEMORY|((attr&NJD_TEXATTR_PALGLOBALINDEX)?0:NJD_TEXATTR_GLOBALINDEX);
            /* PVM entries are independently numbered, not TEXCONTINUE names. */
            native_attr&=~NJD_TEXATTR_TEXCONTINUE;
            njSetTextureName(name,&info,index,native_attr);
            result=njLoadMemoryTexture(list,(Uint32)n);name->filename=filename;name->attr=attr;
            if(result<0)break;
            ++texture;
        }
    }
    free(records);pvm.busy=0;return result;
}
int jp_pvm_load_memory(NJS_TEXLIST *list,uint32_t capacity,size_t filename_bytes,const void *data,size_t bytes)
{
    if(jp_memory_bind_source(data,bytes))return -1;
    if(jp_pvm_bind_destination(list,capacity,filename_bytes)){jp_memory_unbind_source();return -1;}
    int result=njLoadTexturePvmMemory((Uint8 *)data,list);
    jp_pvm_unbind_destination();jp_memory_unbind_source();return result;
}
uint32_t jp_pvm_loaded_count(const NJS_TEXLIST *list)
{
    if(!thread_ok() || !list || (list->nbTexture&&!list->textures) || list->nbTexture>INT16_MAX)return 0;
    uint32_t count=0;for(Uint32 i=0;i<list->nbTexture;i++)if(jp_memory_owner_bound(&list->textures[i].texaddr))++count;return count;
}
int jp_pvm_release_loaded(NJS_TEXLIST *list)
{
    if(!thread_ok())return -1;
    if(error||jp_textures_status()||pvm.busy||!list||(list->nbTexture&&!list->textures)||list->nbTexture>INT16_MAX)return rejected(UINT32_MAX,UINT32_MAX,JP_MEMORY_STATE);
    for(Uint32 i=0;i<list->nbTexture;i++){
        NJS_TEXNAME *name=&list->textures[i];
        NJS_TEXMEMLIST *record=jp_memory_owner_bound(&name->texaddr);
        if(record){
            Uint32 index=record->globalIndex;
            if(name->attr&NJD_TEXATTR_PALGLOBALINDEX)index|=(record->bank&63)<<NJD_TEXBANK_SHIFT;
            if(njReleaseTextureLN(name->texaddr)<0)return rejected(i,UINT32_MAX,JP_MEMORY_TEXTURE);
            jp_texture_forget_field(&name->texaddr);name->texaddr=index;
        }
    }
    return 1;
}
