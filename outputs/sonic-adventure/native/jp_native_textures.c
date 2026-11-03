/* Native record handles and real owned payload storage. Hardware addresses are
 * replaced by nonrecycled resource IDs in the texture word's address field.
 * Payload interpretation and frame retention are separate renderer work. */
#include "jp_native_textures.h"
#include <kamui.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
_Static_assert(sizeof(Uint32)==4 && sizeof(void *)==8,"native texture ABI");
_Static_assert(sizeof(NJS_TEXSURFACE)==sizeof(KMSURFACEDESC),"surface ABI size");
_Static_assert(offsetof(NJS_TEXSURFACE,pSurface)==offsetof(KMSURFACEDESC,pSurface),"surface pointer ABI");
_Static_assert(offsetof(NJS_TEXSURFACE,pVirtual)==offsetof(KMSURFACEDESC,pVirtual),"virtual pointer ABI");
_Static_assert(offsetof(NJS_TEXSURFACE,pPhysical)==offsetof(KMSURFACEDESC,pPhysical),"physical pointer ABI");
NJS_TEXMEMLIST *_nj_texmemlist;
Uint32 _nj_texmemsize;
Sint32 _nj_tex_count;
NJS_TEXLIST *_nj_current_texlist;
Uint32 _nj_current_texture=UINT32_MAX,_nj_current_texture_bank=UINT32_MAX;
NJS_TEXERRSTAT _nj_texerr_;
struct binding { Uint32 *field; Uint32 handle; };
struct surface { void *pixels; size_t bytes; Uint32 word; };
static Uint32 *tokens;
static struct binding *bindings;
static struct surface *surfaces;
static uint32_t binding_limit,live_surfaces;
static size_t byte_budget,allocated;
/* Kept across shutdown so stale values cannot alias a later runtime's record. */
static uint64_t next_token=1,next_surface=1;
static int initialized,initializing,status=JP_TEXTURE_NOT_INITIALIZED;
static int fail(int value) {status=value;return value;}
static int ready(void)
{
    if(!initialized) {fail(JP_TEXTURE_NOT_INITIALIZED);return 0;}
    return status==JP_TEXTURE_OK;
}
int jp_textures_status(void) {return status;}
void jp_textures_clear_error(void) {status=initialized?JP_TEXTURE_OK:JP_TEXTURE_NOT_INITIALIZED;}
size_t jp_textures_allocated_bytes(void) {return allocated;}
uint32_t jp_textures_live_surfaces(void) {return live_surfaces;}
void jp_textures_shutdown(void)
{
    if(surfaces)for(uint32_t i=0;i<_nj_texmemsize;i++)_aligned_free(surfaces[i].pixels);
    free(surfaces);free(bindings);free(tokens);free(_nj_texmemlist);
    surfaces=NULL;bindings=NULL;tokens=NULL;_nj_texmemlist=NULL;_nj_texmemsize=0;
    _nj_tex_count=0;_nj_current_texlist=NULL;
    _nj_current_texture=_nj_current_texture_bank=UINT32_MAX;
    memset(&_nj_texerr_,0,sizeof _nj_texerr_);
    initialized=initializing=0;binding_limit=live_surfaces=0;byte_budget=allocated=0;
    status=JP_TEXTURE_NOT_INITIALIZED;
}
int jp_textures_initialize(uint32_t count,uint32_t names,size_t budget)
{
    if(!count || !names || count>INT_MAX)return fail(JP_TEXTURE_ARGUMENT);
    size_t cost=(size_t)count*(sizeof(NJS_TEXMEMLIST)+sizeof(Uint32)+sizeof(struct surface));
    if(names>(SIZE_MAX-cost)/sizeof(struct binding))return fail(JP_TEXTURE_ARGUMENT);
    cost+=(size_t)names*sizeof(struct binding);
    if(cost>budget)return fail(JP_TEXTURE_BUDGET);
    /* Allocate a full replacement first. Reinitialization never destroys the
     * prior usable runtime merely because the new allocation fails. */
    if(initialized && (allocated>budget || cost>budget-allocated))return fail(JP_TEXTURE_BUDGET);
    NJS_TEXMEMLIST *records=calloc(count,sizeof *records);
    Uint32 *new_tokens=calloc(count,sizeof *new_tokens);
    struct binding *new_bindings=calloc(names,sizeof *new_bindings);
    struct surface *new_surfaces=calloc(count,sizeof *new_surfaces);
    if(!records || !new_tokens || !new_bindings || !new_surfaces) {
        free(records);free(new_tokens);free(new_bindings);free(new_surfaces);
        return fail(JP_TEXTURE_ALLOCATION);
    }
    jp_textures_shutdown();_nj_texmemlist=records;_nj_texmemsize=count;
    tokens=new_tokens;bindings=new_bindings;surfaces=new_surfaces;binding_limit=names;
    byte_budget=budget;allocated=cost;initialized=initializing=1;status=JP_TEXTURE_OK;
    njInitTexMemList(records,count);initializing=0;
    return status;
}
int jp_textures_accept_sdk_init(NJS_TEXMEMLIST *records,Uint32 count)
{
    if(!ready())return 0;
    if(!initializing || records!=_nj_texmemlist || count!=_nj_texmemsize) {fail(JP_TEXTURE_STATE);return 0;}
    return 1;
}
intptr_t jp_texture_record_index(const NJS_TEXMEMLIST *record)
{
    uintptr_t base=(uintptr_t)_nj_texmemlist,value=(uintptr_t)record;
    if(!initialized || !record || value<base || (value-base)%sizeof *record ||
       (value-base)/sizeof *record>=_nj_texmemsize) {fail(JP_TEXTURE_ARGUMENT);return -1;}
    return (intptr_t)((value-base)/sizeof *record);
}
NJS_TEXMEMLIST *jp_textures_record(uint32_t index)
{
    if(!ready())return NULL;
    if(index>=_nj_texmemsize) {fail(JP_TEXTURE_ARGUMENT);return NULL;}
    return &_nj_texmemlist[index];
}
static struct binding *binding_for(Uint32 *field,int vacant)
{
    struct binding *empty=NULL;
    for(uint32_t i=0;i<binding_limit;i++) {
        if(bindings[i].field==field)return &bindings[i];
        if(!bindings[i].field && !empty)empty=&bindings[i];
    }
    return vacant?empty:NULL;
}
int jp_texture_free_slot_available(void)
{
    if(!ready())return 0;
    for(uint32_t i=0;i<_nj_texmemsize;i++)if(_nj_texmemlist[i].globalIndex==UINT32_MAX)return 1;
    fail(JP_TEXTURE_CAPACITY);return 0;
}
int jp_texture_can_enter(Uint32 *field,const NJS_TEXINFO *info)
{
    if(!ready())return 0;
    if(!field || !info) {fail(JP_TEXTURE_ARGUMENT);return 0;}
    if(!binding_for(field,1)) {fail(JP_TEXTURE_CAPACITY);return 0;}
    if(next_token>=UINT32_MAX) {fail(JP_TEXTURE_EXHAUSTED);return 0;}
    return jp_texture_free_slot_available();
}
int jp_texture_store_handle(Uint32 *field,NJS_TEXMEMLIST *record)
{
    if(!ready())return status;
    if(!field)return fail(JP_TEXTURE_ARGUMENT);
    intptr_t index=jp_texture_record_index(record);if(index<0)return status;
    struct binding *b=binding_for(field,1);
    if(!b)return fail(JP_TEXTURE_CAPACITY);
    if(!tokens[index]) {
        if(next_token>=UINT32_MAX)return fail(JP_TEXTURE_EXHAUSTED);
        tokens[index]=(Uint32)next_token++;
    }
    b->field=field;b->handle=tokens[index];*field=b->handle;return status;
}
void jp_texture_forget_field(Uint32 *field)
{
    if(!initialized)return;
    struct binding *b=binding_for(field,0);if(b)memset(b,0,sizeof *b);
}
NJS_TEXMEMLIST *jp_texture_resolve(uint32_t handle)
{
    if(!ready())return NULL;
    if(handle && handle!=UINT32_MAX)for(uint32_t i=0;i<_nj_texmemsize;i++)
        if(tokens[i]==handle && _nj_texmemlist[i].globalIndex!=UINT32_MAX)return &_nj_texmemlist[i];
    fail(JP_TEXTURE_STALE);return NULL;
}
int jp_texture_retire_record(NJS_TEXMEMLIST *record)
{
    if(!ready())return 0;
    intptr_t index=jp_texture_record_index(record);if(index<0)return 0;
    Uint32 token=tokens[index];
    for(uint32_t i=0;i<binding_limit;i++)if(token && bindings[i].handle==token)memset(&bindings[i],0,sizeof bindings[i]);
    tokens[index]=0;return 1;
}
NJS_TEXMEMLIST *jp_texture_resolve_field(Uint32 *field)
{
    if(!ready())return NULL;
    if(!field) {fail(JP_TEXTURE_ARGUMENT);return NULL;}
    struct binding *b=binding_for(field,0);
    if(!b || *field!=b->handle) {fail(JP_TEXTURE_STALE);return NULL;}
    NJS_TEXMEMLIST *record=jp_texture_resolve(*field);
    return record;
}
const NJS_TEXMEMLIST *jp_renderer_texture_lookup(const NJS_TEXLIST *list,Uint32 index)
{
    if(!ready())return NULL;
    if(!list || !list->textures || index>=list->nbTexture) {fail(JP_TEXTURE_ARGUMENT);return NULL;}
    NJS_TEXMEMLIST *record=jp_texture_resolve_field(&list->textures[index].texaddr);
    if(!record)return NULL;
    if(!(record->texaddr&TEXMEM_LOADED) || !jp_texture_surface_owned(&record->texinfo.texsurface)) {
        fail(JP_TEXTURE_STATE);return NULL;
    }
    return record;
}
int jp_textures_validate_list(const NJS_TEXLIST *list)
{
    if(!ready())return 0;
    if(!list || (list->nbTexture && !list->textures)) {fail(JP_TEXTURE_ARGUMENT);return 0;}
    for(Uint32 i=0;i<list->nbTexture;i++)if(!jp_renderer_texture_lookup(list,i))return 0;
    return 1;
}
static struct surface *surface_at(const void *pixels)
{
    if(pixels)for(uint32_t i=0;i<_nj_texmemsize;i++)if(surfaces[i].pixels==pixels)return &surfaces[i];
    return NULL;
}
int jp_texture_surface_owned(const NJS_TEXSURFACE *surface)
{
    if(!ready())return 0;
    if(!surface || !surface_at(surface->pSurface)) {fail(JP_TEXTURE_STALE);return 0;}
    return 1;
}
int jp_texture_copy_surface(NJS_TEXINFO *info,const void *bytes,size_t count)
{
    if(!ready())return status;
    if(!info || !bytes || !count || count!=info->texsurface.TextureSize ||
       !info->texsurface.nWidth || !info->texsurface.nHeight || info->texsurface.pSurface)return fail(JP_TEXTURE_ARGUMENT);
    if(count>byte_budget-allocated)return fail(JP_TEXTURE_BUDGET);
    if(next_surface>0x001fffff)return fail(JP_TEXTURE_EXHAUSTED);
    struct surface *slot=NULL;
    for(uint32_t i=0;i<_nj_texmemsize;i++)if(!surfaces[i].pixels) {slot=&surfaces[i];break;}
    if(!slot)return fail(JP_TEXTURE_CAPACITY);
    void *pixels=_aligned_malloc(count,32);if(!pixels)return fail(JP_TEXTURE_ALLOCATION);
    memcpy(pixels,bytes,count);slot->pixels=pixels;slot->bytes=count;slot->word=(Uint32)next_surface++;
    allocated+=count;live_surfaces++;
    info->texsurface.pSurface=info->texsurface.pVirtual=info->texsurface.pPhysical=pixels;
    return status;
}
uint32_t jp_texture_surface_word(const NJS_TEXSURFACE *surface)
{
    if(!jp_texture_surface_owned(surface))return 0;
    return surface_at(surface->pSurface)->word;
}
const void *jp_texture_surface_data(uint32_t word)
{
    if(!ready())return NULL;
    for(uint32_t i=0;i<_nj_texmemsize;i++)if(surfaces[i].pixels && surfaces[i].word==word)return surfaces[i].pixels;
    fail(JP_TEXTURE_STALE);return NULL;
}
size_t jp_texture_surface_bytes(uint32_t word)
{
    const void *pixels=jp_texture_surface_data(word);if(!pixels)return 0;
    return surface_at(pixels)->bytes;
}
KMSTATUS kmFreeTexture(PKMSURFACEDESC descriptor)
{
    if(!ready())return KMSTATUS_INVALID_SURFACE;
    struct surface *s=descriptor?surface_at(descriptor->pSurface):NULL;
    if(!s) {fail(JP_TEXTURE_STALE);return KMSTATUS_INVALID_SURFACE;}
    _aligned_free(s->pixels);allocated-=s->bytes;live_surfaces--;memset(s,0,sizeof *s);
    descriptor->pSurface=descriptor->pVirtual=descriptor->pPhysical=NULL;
    return KMSTATUS_SUCCESS;
}
