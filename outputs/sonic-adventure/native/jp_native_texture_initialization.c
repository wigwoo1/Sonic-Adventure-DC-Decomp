/* REPLACE the existing texture-move owner object; do not link both.
 * Include every previously qualified authored owner operation unchanged.
 * Only record-table lifetime/initialization gains caller-owned storage.
 * A game thread supplies a valid, writable count-element native-width table.
 */
#include "jp_native_texture_initialization.h"
#include "jp_native_texture_upload.h"
#include "jp_native_assets.h"
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <limits.h>
static NJS_TEXMEMLIST *borrowed_records,*preserve_records;
static uint32_t owned_record_capacity;
static uint32_t sdk_bindings=16384;
static size_t sdk_budget=64u*1024u*1024u;
static void jp_texture_table_aware_free(void *pointer)
{
    if(pointer!=borrowed_records && pointer!=preserve_records)free(pointer);
}
/* Headers above retain the original public declarations. Internal names are
 * not alternative exported owners. The original allocation/error paths stay
 * intact, including their calls to the real njInitTexMemList implementation.
 */
#define free jp_texture_table_aware_free
#define jp_textures_shutdown jp_textures_storage_shutdown
#define jp_textures_initialize jp_textures_storage_initialize
#include "jp_native_texture_move_owner.c"
#undef jp_textures_initialize
#undef jp_textures_shutdown
#undef free
char _nj_current_texture_path[256];
NJS_TEXSURFACE _nj_tex_err_surface;
void jp_textures_shutdown(void)
{
    jp_textures_storage_shutdown();
    borrowed_records=preserve_records=NULL;owned_record_capacity=0;
}
int jp_textures_initialize(uint32_t count,uint32_t names,size_t budget)
{
    int result=jp_textures_storage_initialize(count,names,budget);
    if(!result){borrowed_records=NULL;owned_record_capacity=count;}
    return result;
}
int jp_texture_sdk_limits(uint32_t names,size_t budget)
{
    if(!names || !budget || initializing)return fail(JP_TEXTURE_ARGUMENT);
    sdk_bindings=names;sdk_budget=budget;return JP_TEXTURE_OK;
}
int jp_texture_begin_sdk_init(NJS_TEXMEMLIST *records,Uint32 count)
{
    if(initializing){fail(JP_TEXTURE_STATE);return 0;}
    size_t record_bytes=(size_t)count*sizeof *records;
    uintptr_t address=(uintptr_t)records;
    if(count>INT_MAX || (count && (!records || address%_Alignof(NJS_TEXMEMLIST))) ||
       record_bytes>UINTPTR_MAX-address){fail(JP_TEXTURE_ARGUMENT);return 0;}
    int keep_owned=initialized && records==_nj_texmemlist && borrowed_records!=records;
    uint32_t previous_capacity=owned_record_capacity;
    if(initialized && _nj_texmemlist){
        uintptr_t old=(uintptr_t)_nj_texmemlist;
        size_t span=(size_t)(borrowed_records==_nj_texmemlist?_nj_texmemsize:owned_record_capacity)*sizeof *records;
        if(records!=_nj_texmemlist && address<old+span && old<address+record_bytes){fail(JP_TEXTURE_ARGUMENT);return 0;}
        if(keep_owned && count>owned_record_capacity){fail(JP_TEXTURE_ARGUMENT);return 0;}
    }
    /* Account for the complete logical record table, including borrowed
     * storage. Retain the previous owner's transient replacement budget rule.
     * Submitted frame resources have their separate existing frame budget.
     */
    /* Shrinking a reused heap table does not shrink its allocation. Charge
     * the retained physical capacity as well as the new live sidecars.
     */
    size_t record_cost=(size_t)(keep_owned?owned_record_capacity:count)*sizeof *records;
    size_t cost=record_cost+(size_t)count*(sizeof(Uint32)+sizeof(struct surface));
    if(sdk_bindings>(SIZE_MAX-cost)/sizeof(struct binding)){fail(JP_TEXTURE_ARGUMENT);return 0;}
    cost+=(size_t)sdk_bindings*sizeof(struct binding);
    if(cost>sdk_budget || allocated>sdk_budget-cost){fail(JP_TEXTURE_BUDGET);return 0;}
    Uint32 *new_tokens=count?calloc(count,sizeof *new_tokens):NULL;
    struct surface *new_surfaces=count?calloc(count,sizeof *new_surfaces):NULL;
    struct binding *new_bindings=calloc(sdk_bindings,sizeof *new_bindings);
    if((count && (!new_tokens || !new_surfaces)) || !new_bindings){
        free(new_tokens);free(new_surfaces);free(new_bindings);
        fail(JP_TEXTURE_ALLOCATION);return 0;
    }
    /* njInitTexture does not reset these SDK values. Retiring native handles
     * must not add writes to its value/memory contract. Any retained stale
     * handles still fail the existing resolver instead of aliasing new ones.
     */
    NJS_TEXLIST *selected=_nj_current_texlist;
    Uint32 texture=_nj_current_texture,bank=_nj_current_texture_bank;
    NJS_TEXERRSTAT error=_nj_texerr_;
    preserve_records=records;jp_textures_storage_shutdown();preserve_records=NULL;
    borrowed_records=keep_owned?NULL:records;owned_record_capacity=keep_owned?previous_capacity:0;
    _nj_texmemlist=records;_nj_texmemsize=count;tokens=new_tokens;surfaces=new_surfaces;
    bindings=new_bindings;binding_limit=sdk_bindings;byte_budget=sdk_budget;allocated=cost;
    initialized=initializing=1;status=JP_TEXTURE_OK;
    _nj_current_texlist=selected;_nj_current_texture=texture;_nj_current_texture_bank=bank;_nj_texerr_=error;
    return 1;
}
void jp_texture_finish_sdk_init(void)
{
    if(!initialized || !initializing){fail(JP_TEXTURE_STATE);return;}
    initializing=0;
}
