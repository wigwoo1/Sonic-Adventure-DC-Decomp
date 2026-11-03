/* Authored native replacement of store-queue addressing and buffer ownership.
 * Header fields follow the pinned SDK's four plain polygon-start contracts.
 * Unwritten header words are deterministically zero. No MMIO/cache alias is
 * emulated: complete batches become host-readable streams at njEndVertex.
 * This transports geometry; it does not rasterize or supply missing services.
 */
#include "jp_native_queue.h"
/* The external SDK defines its own equivalent NULL macro unconditionally. */
#undef NULL
#include <kamui.h>
#include <ninja.h>
#include "sdk/ninja/njkamui.h"
#include <malloc.h>
#include <string.h>

_Static_assert(sizeof(Uint32)==4 && sizeof(void *)==8,"Windows x64 SDK ABI");
struct nj_parameterkm _nj_parameterkm_;
KMVERTEXCONTEXT _nj_context_[NJD_CONTEXT_MAX];
KMVERTEXCONTEXT *_nj_current_ctx_ = &_nj_context_[0];
Uint32 _nj_ctx_color_mode_[NJD_CONTEXT_MAX];
Uint32 *_nj_color_mode_ = &_nj_ctx_color_mode_[0];
/* The SDK descriptor points to a separate five-entry cursor table. */
static PKMDWORD descriptor_cursors[JP_QUEUE_LISTS];
KMVERTEXBUFFDESC _nj_vertex_buffer_ = { .pCurrentPtr=descriptor_cursors };

struct pool { unsigned char *allocation, *data; size_t capacity, used, limit; };
static struct pool pools[JP_QUEUE_LISTS];
static size_t memory_budget, allocated;
static int initialized, status=JP_QUEUE_NOT_INITIALIZED, active=-1, dirty;
static unsigned char *active_start, *active_limit;
static uint64_t cache_hints;
static const void *last_hint;
static const unsigned descriptor_indices[JP_QUEUE_LISTS] = {
    KM_OPAQUE_POLYGON,KM_TRANS_POLYGON,KM_PUNCHTHROUGH_POLYGON,
    KM_OPAQUE_MODIFIER,KM_TRANS_MODIFIER
};
static int fail(int error) { status=error; return error; }
static int ready(void)
{
    if (!initialized) { fail(JP_QUEUE_NOT_INITIALIZED); return 0; }
    return status==JP_QUEUE_OK;
}
static Uint32 **cursor(unsigned list)
{
    switch(list) {
    case JP_QUEUE_OPAQUE:return &_nj_parameterkm_.opaque_polygon;
    case JP_QUEUE_TRANSLUCENT:return &_nj_parameterkm_.trans_polygon;
    case JP_QUEUE_PUNCH:return &_nj_parameterkm_.punchthrough_polygon;
    case JP_QUEUE_OPAQUE_MODIFIER:return &_nj_parameterkm_.opaque_modifier;
    default:return &_nj_parameterkm_.trans_modifier;
    }
}
static int offset(const struct pool *p,const void *pointer,size_t *out)
{
    if (!p->data) { if(pointer)return 0;*out=0;return 1; }
    uintptr_t base=(uintptr_t)p->data, value=(uintptr_t)pointer;
    if (value<base || value-base>p->capacity || (value-base)%32)return 0;
    *out=(size_t)(value-base);return 1;
}
static int guards(const struct pool *p)
{
    if (!p->allocation)return 1;
    for(size_t i=0;i<32;i++)
        if(p->allocation[i]!=0xa5 || p->data[p->capacity+i]!=0x5a)return 0;
    return 1;
}
static int context_index(void)
{
    for(unsigned i=0;i<NJD_CONTEXT_MAX;i++)if(_nj_current_ctx_==&_nj_context_[i])return (int)i;
    return -1;
}
int jp_queue_initialize(size_t budget)
{
    if(!budget)return fail(JP_QUEUE_ARGUMENT);
    if(initialized && active>=0)return fail(JP_QUEUE_STATE);
    jp_queue_shutdown();memory_budget=budget;initialized=1;status=JP_QUEUE_OK;
    return status;
}
void jp_queue_shutdown(void)
{
    for(unsigned i=0;i<JP_QUEUE_LISTS;i++)_aligned_free(pools[i].allocation);
    memset(pools,0,sizeof pools);memset(&_nj_parameterkm_,0,sizeof _nj_parameterkm_);
    memset(&_nj_vertex_buffer_,0,sizeof _nj_vertex_buffer_);
    memset(descriptor_cursors,0,sizeof descriptor_cursors);
    _nj_vertex_buffer_.pCurrentPtr=descriptor_cursors;
    memset(_nj_context_,0,sizeof _nj_context_);memset(_nj_ctx_color_mode_,0,sizeof _nj_ctx_color_mode_);
    _nj_current_ctx_=&_nj_context_[0];_nj_color_mode_=&_nj_ctx_color_mode_[0];
    initialized=0;active=-1;active_start=active_limit=NULL;dirty=0;
    allocated=memory_budget=0;cache_hints=0;last_hint=NULL;status=JP_QUEUE_NOT_INITIALIZED;
}
int jp_queue_reset_frame(void)
{
    if(!ready())return status;
    if(active>=0)return fail(JP_QUEUE_STATE);
    for(unsigned i=0;i<JP_QUEUE_LISTS;i++)if(!guards(&pools[i]))return fail(JP_QUEUE_GUARD);
    for(unsigned i=0;i<JP_QUEUE_LISTS;i++) {
        struct pool *p=&pools[i];
        if(p->data)memset(p->data,0,p->used);
        p->used=p->limit=0;*cursor(i)=(Uint32 *)p->data;
        _nj_vertex_buffer_.pCurrentPtr[descriptor_indices[i]]=(Uint32 *)p->data;
    }
    dirty=0;return status;
}
int jp_queue_reserve(unsigned list,uint32_t blocks)
{
    if(!ready())return status;
    if(list>=JP_QUEUE_LISTS)return fail(JP_QUEUE_ARGUMENT);
    if(active>=0)return fail(JP_QUEUE_STATE);
    struct pool *p=&pools[list];size_t position,descriptor_position;
    if(!guards(p))return fail(JP_QUEUE_GUARD);
    if(!offset(p,*cursor(list),&position) ||
       !offset(p,_nj_vertex_buffer_.pCurrentPtr[descriptor_indices[list]],&descriptor_position))return fail(JP_QUEUE_BOUNDS);
    if(blocks>(SIZE_MAX-position-64)/32)return fail(JP_QUEUE_BOUNDS);
    size_t need=position+(size_t)blocks*32;
    if(need>p->capacity) {
        size_t old_cost=p->allocation?p->capacity+64:0,new_cost=need+64;
        /* Preserve the old stream until allocation succeeds. Budget both
         * allocations during growth, including their guard storage. */
        if(allocated>memory_budget || new_cost>memory_budget-allocated)return fail(JP_QUEUE_BUDGET);
        unsigned char *allocation=_aligned_malloc(new_cost,32);
        if(!allocation)return fail(JP_QUEUE_ALLOCATION);
        unsigned char *data=allocation+32;
        memset(allocation,0xa5,32);memset(data,0,need);memset(data+need,0x5a,32);
        if(p->data)memcpy(data,p->data,p->used);
        _aligned_free(p->allocation);p->allocation=allocation;p->data=data;p->capacity=need;
        allocated=allocated-old_cost+new_cost;
        *cursor(list)=(Uint32 *)(data+position);
        _nj_vertex_buffer_.pCurrentPtr[descriptor_indices[list]]=(Uint32 *)(data+descriptor_position);
    }
    p->limit=need;return status;
}
int jp_queue_status(void) { return status; }
void jp_queue_clear_error(void) { status=initialized?JP_QUEUE_OK:JP_QUEUE_NOT_INITIALIZED; }
size_t jp_queue_allocated_bytes(void) { return allocated; }
size_t jp_queue_committed_bytes(unsigned list) { return list<JP_QUEUE_LISTS?pools[list].used:0; }
const void *jp_queue_data(unsigned list) { return list<JP_QUEUE_LISTS?pools[list].data:NULL; }
size_t jp_queue_active_capacity_blocks(void)
{
    return active>=0?(size_t)(active_limit-active_start)/32:0;
}
unsigned jp_queue_context_count(void) { return NJD_CONTEXT_MAX; }
int jp_queue_set_context(unsigned index,const uint32_t words[4])
{
    if(!ready())return status;
    if(index>=NJD_CONTEXT_MAX || !words)return fail(JP_QUEUE_ARGUMENT);
    if(active>=0)return fail(JP_QUEUE_STATE);
    KMVERTEXCONTEXT *c=&_nj_context_[index];
    c->GLOBALPARAMBUFFER=words[0];c->ISPPARAMBUFFER=words[1];c->TSPPARAMBUFFER=words[2];c->TexturePARAMBUFFER=words[3];
    return status;
}
int jp_queue_select_context(unsigned index)
{
    if(!ready())return status;
    if(index>=NJD_CONTEXT_MAX)return fail(JP_QUEUE_ARGUMENT);
    if(active>=0 || dirty)return fail(JP_QUEUE_STATE);
    _nj_current_ctx_=&_nj_context_[index];_nj_color_mode_=&_nj_ctx_color_mode_[index];return status;
}
int jp_queue_get_context(unsigned index,uint32_t words[4])
{
    if(index>=NJD_CONTEXT_MAX || !words)return fail(JP_QUEUE_ARGUMENT);
    KMVERTEXCONTEXT *c=&_nj_context_[index];
    words[0]=c->GLOBALPARAMBUFFER;words[1]=c->ISPPARAMBUFFER;words[2]=c->TSPPARAMBUFFER;words[3]=c->TexturePARAMBUFFER;
    return JP_QUEUE_OK;
}
int jp_queue_get_parameter(uint32_t words[4])
{
    if(!words)return fail(JP_QUEUE_ARGUMENT);
    words[0]=_nj_parameterkm_.GLOBALPARAMBUFFER;words[1]=_nj_parameterkm_.ISPPARAMBUFFER;
    words[2]=_nj_parameterkm_.TSPPARAMBUFFER;words[3]=_nj_parameterkm_.TexturePARAMBUFFER;return JP_QUEUE_OK;
}
void *jp_queue_parameter_cursor(unsigned list) { return list<JP_QUEUE_LISTS?*cursor(list):NULL; }
void *jp_queue_descriptor_cursor(unsigned list) { return list<JP_QUEUE_LISTS?_nj_vertex_buffer_.pCurrentPtr[descriptor_indices[list]]:NULL; }
uint64_t jp_queue_cache_hint_count(void) { return cache_hints; }
const void *jp_queue_last_cache_hint(void) { return last_hint; }

void njGetkmPointer(void)
{
    if(!ready())return;
    if(active>=0 || dirty || context_index()<0) {fail(JP_QUEUE_STATE);return;}
    for(unsigned i=0;i<JP_QUEUE_LISTS;i++) {
        size_t ignored;
        if(!guards(&pools[i])) {fail(JP_QUEUE_GUARD);return;}
        if(!offset(&pools[i],_nj_vertex_buffer_.pCurrentPtr[descriptor_indices[i]],&ignored)) {fail(JP_QUEUE_BOUNDS);return;}
    }
    _nj_parameterkm_.GLOBALPARAMBUFFER=_nj_current_ctx_->GLOBALPARAMBUFFER;
    _nj_parameterkm_.ISPPARAMBUFFER=_nj_current_ctx_->ISPPARAMBUFFER;
    _nj_parameterkm_.TSPPARAMBUFFER=_nj_current_ctx_->TSPPARAMBUFFER;
    _nj_parameterkm_.TexturePARAMBUFFER=_nj_current_ctx_->TexturePARAMBUFFER;
    for(unsigned i=0;i<JP_QUEUE_LISTS;i++)*cursor(i)=_nj_vertex_buffer_.pCurrentPtr[descriptor_indices[i]];
}
void njSetkmPointer(void)
{
    if(!ready())return;
    if(active>=0 || context_index()<0) {fail(JP_QUEUE_STATE);return;}
    for(unsigned i=0;i<JP_QUEUE_LISTS;i++) {
        size_t ignored;
        if(!guards(&pools[i])) {fail(JP_QUEUE_GUARD);return;}
        if(!offset(&pools[i],*cursor(i),&ignored)) {fail(JP_QUEUE_BOUNDS);return;}
    }
    _nj_current_ctx_->GLOBALPARAMBUFFER=_nj_parameterkm_.GLOBALPARAMBUFFER;
    _nj_current_ctx_->ISPPARAMBUFFER=_nj_parameterkm_.ISPPARAMBUFFER;
    _nj_current_ctx_->TSPPARAMBUFFER=_nj_parameterkm_.TSPPARAMBUFFER;
    _nj_current_ctx_->TexturePARAMBUFFER=_nj_parameterkm_.TexturePARAMBUFFER;
    for(unsigned i=0;i<JP_QUEUE_LISTS;i++)_nj_vertex_buffer_.pCurrentPtr[descriptor_indices[i]]=*cursor(i);
    dirty=0;
}
void _builtin_prefetch(void *pointer)
{
    cache_hints++;last_hint=pointer;__builtin_prefetch(pointer);
}
static void *begin(unsigned list,int textured,int opaque)
{
    if(!ready())return NULL;
    if(active>=0) {fail(JP_QUEUE_STATE);return NULL;}
    struct pool *p=&pools[list];size_t position;
    if(!guards(p)) {fail(JP_QUEUE_GUARD);return NULL;}
    if(!offset(p,*cursor(list),&position) || position>p->limit || p->limit-position<32) {fail(JP_QUEUE_BOUNDS);return NULL;}
    Uint32 *header=(Uint32 *)(p->data+position);memset(header,0,32);
    header[0]=_nj_parameterkm_.GLOBALPARAMBUFFER;header[1]=_nj_parameterkm_.ISPPARAMBUFFER;
    Uint32 tsp=_nj_parameterkm_.TSPPARAMBUFFER;
    header[2]=opaque?((tsp&~0xfc100000UL)|0x20080000UL):tsp;
    if(textured)header[3]=_nj_parameterkm_.TexturePARAMBUFFER;
    _builtin_prefetch(header);active=(int)list;
    active_start=(unsigned char *)(header+8);active_limit=p->data+p->limit;
    return active_start;
}
void *njStartVertexNonTexOpaque(void) { return begin(JP_QUEUE_OPAQUE,0,1); }
void *njStartVertexNonTexTrans(void) { return begin(JP_QUEUE_TRANSLUCENT,0,0); }
void *njStartVertexTexOpaque(void) { return begin(JP_QUEUE_OPAQUE,1,1); }
void *njStartVertexTexTrans(void) { return begin(JP_QUEUE_TRANSLUCENT,1,0); }
void njEndVertex(Uint32 *end)
{
    if(!ready())return;
    if(active<0) {fail(JP_QUEUE_STATE);return;}
    uintptr_t value=(uintptr_t)end,first=(uintptr_t)active_start,last=(uintptr_t)active_limit;
    if(value<first || value>last || (value-first)%32) {fail(JP_QUEUE_BOUNDS);return;}
    struct pool *p=&pools[active];
    if(!guards(p)) {fail(JP_QUEUE_GUARD);return;}
    p->used=(size_t)(value-(uintptr_t)p->data);*cursor((unsigned)active)=end;
    active=-1;active_start=active_limit=NULL;dirty=1;
}
void njSetkmModelAttr(Uint32 attributes)
{
    if(!ready())return;
    _nj_parameterkm_.TSPPARAMBUFFER=(_nj_parameterkm_.TSPPARAMBUFFER&~0xfc07ff00UL)|(attributes&0xfc07ff00UL);
}
