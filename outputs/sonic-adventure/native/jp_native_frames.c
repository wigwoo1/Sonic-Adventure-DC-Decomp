#include "jp_native_frames_internal.h"
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
struct resource { const void *pixels;size_t bytes;uint32_t word,refs;int orphaned; };
struct frame {uint64_t id;unsigned char *allocation,*data;size_t cost,offsets[5],sizes[5];uint32_t count,*resources;};
static struct resource *resources;
static struct frame *frames;
static uint32_t *indices,*recording,frame_capacity,resource_capacity,recorded,live;
static uint32_t pending_index;static int pending_open,pending_new,initialized,status=JP_FRAME_NOT_INITIALIZED;
static size_t budget,metadata_bytes,snapshot_bytes,pinned_bytes,orphaned_bytes;
/* Never recycled, including across successful shutdown/reinitialization. */
static uint64_t next_id=1;
static int fail(int code){status=code;return code;}
int jp_frames_report_error(int code){return fail(code);}
static int ready(void){if(!initialized){fail(JP_FRAME_NOT_INITIALIZED);return 0;}return status==JP_FRAME_OK;}
static size_t accounted(void){return metadata_bytes+snapshot_bytes+pinned_bytes;}
static int room(size_t bytes){size_t now=accounted();return now<=budget && bytes<=budget-now;}
static void unpin(uint32_t index)
{
    struct resource *r=&resources[index];
    if(--r->refs)return;
    pinned_bytes-=r->bytes;
    if(r->orphaned){orphaned_bytes-=r->bytes;_aligned_free((void *)r->pixels);}
    memset(r,0,sizeof *r);
}
void jp_frames_record_abort(void)
{
    if(!initialized || !pending_open)return;
    if(pending_new)unpin(pending_index);
    pending_open=pending_new=0;
}
void jp_frames_record_discard(void)
{
    if(!initialized)return;
    jp_frames_record_abort();while(recorded)unpin(recording[--recorded]);
}
static void drop(struct frame *f)
{
    for(uint32_t i=0;i<f->count;i++)unpin(f->resources[i]);
    _aligned_free(f->allocation);snapshot_bytes-=f->cost;f->allocation=f->data=NULL;f->cost=0;f->count=0;f->id=0;--live;
}
int jp_frames_shutdown(void)
{
    if(!jp_frame_queue_empty())return fail(JP_FRAME_STATE);
    if(initialized){jp_frames_record_discard();for(uint32_t i=0;i<frame_capacity;i++)if(frames[i].id)drop(&frames[i]);}
    free(resources);free(frames);free(indices);resources=NULL;frames=NULL;indices=recording=NULL;
    frame_capacity=resource_capacity=recorded=live=0;pending_open=pending_new=initialized=0;
    budget=metadata_bytes=snapshot_bytes=pinned_bytes=orphaned_bytes=0;status=JP_FRAME_NOT_INITIALIZED;return JP_FRAME_OK;
}
int jp_frames_initialize(uint32_t count,uint32_t capacity,size_t limit)
{
    if(!count || !capacity || count==UINT32_MAX)return fail(JP_FRAME_ARGUMENT);
    if(!jp_frame_queue_empty())return fail(JP_FRAME_STATE);
    size_t vector_count=(size_t)count+1;
    if(vector_count>SIZE_MAX/capacity || vector_count*capacity>SIZE_MAX/sizeof(uint32_t))return fail(JP_FRAME_ARGUMENT);
    size_t icost=vector_count*capacity*sizeof(uint32_t);
    size_t fcost=(size_t)count*sizeof(struct frame),rcost=(size_t)capacity*sizeof(struct resource);
    if(fcost/count!=sizeof(struct frame) || rcost/capacity!=sizeof(struct resource))return fail(JP_FRAME_ARGUMENT);
    if(fcost>SIZE_MAX-rcost || icost>SIZE_MAX-fcost-rcost)return fail(JP_FRAME_ARGUMENT);
    size_t cost=fcost+rcost+icost;
    if(cost>limit || accounted()>limit-cost)return fail(JP_FRAME_BUDGET);
    struct frame *nf=calloc(count,sizeof *nf);struct resource *nr=calloc(capacity,sizeof *nr);uint32_t *ni=calloc(vector_count*capacity,sizeof *ni);
    if(!nf || !nr || !ni){free(nf);free(nr);free(ni);return fail(JP_FRAME_ALLOCATION);}
    /* All allocations precede retiring the previous usable owner. */
    int result=jp_frames_shutdown();if(result){free(nf);free(nr);free(ni);return result;}
    frames=nf;resources=nr;indices=ni;recording=ni+(size_t)count*capacity;
    for(uint32_t i=0;i<count;i++)frames[i].resources=ni+(size_t)i*capacity;
    frame_capacity=count;resource_capacity=capacity;budget=limit;metadata_bytes=cost;initialized=1;status=JP_FRAME_OK;return status;
}
int jp_frames_status(void){return status;}
void jp_frames_clear_error(void){status=initialized?JP_FRAME_OK:JP_FRAME_NOT_INITIALIZED;}
size_t jp_frames_accounted_bytes(void){return accounted();}
size_t jp_frames_pinned_bytes(void){return pinned_bytes;}
size_t jp_frames_orphaned_bytes(void){return orphaned_bytes;}
uint32_t jp_frames_live_count(void){return live;}
int jp_frames_record_begin(int textured,uint32_t word)
{
    if(!initialized)return JP_FRAME_OK; /* Preserved legacy mode, no frames. */
    if(!ready())return status;
    if(pending_open)return fail(JP_FRAME_STATE);
    pending_new=0;
    if(!textured){pending_open=1;return JP_FRAME_OK;}
    const void *pixels;size_t bytes;
    if(!jp_texture_live_surface_view(word,&pixels,&bytes))return fail(JP_FRAME_RESOURCE);
    for(uint32_t i=0;i<recorded;i++)if(resources[recording[i]].word==word){pending_index=recording[i];pending_open=1;return JP_FRAME_OK;}
    uint32_t slot=resource_capacity,free_slot=resource_capacity;
    for(uint32_t i=0;i<resource_capacity;i++){
        if(resources[i].refs && resources[i].word==word){slot=i;break;}
        if(!resources[i].refs)free_slot=i;
    }
    if(slot==resource_capacity){
        if(free_slot==resource_capacity || recorded==resource_capacity)return fail(JP_FRAME_CAPACITY);
        if(!room(bytes))return fail(JP_FRAME_BUDGET);
        slot=free_slot;resources[slot]=(struct resource){pixels,bytes,word,0,0};pinned_bytes+=bytes;
    }else if(resources[slot].pixels!=pixels || resources[slot].bytes!=bytes || resources[slot].orphaned)return fail(JP_FRAME_RESOURCE);
    if(resources[slot].refs==UINT32_MAX)return fail(JP_FRAME_CAPACITY);
    ++resources[slot].refs;pending_index=slot;pending_new=1;pending_open=1;return JP_FRAME_OK;
}
void jp_frames_record_commit(void)
{
    if(!initialized || !pending_open)return;
    if(pending_new)recording[recorded++]=pending_index;
    pending_open=pending_new=0;
}
int jp_frames_prepare_texture(uint32_t word)
{return jp_frames_prepare_textures(&word,1);}
int jp_frames_prepare_textures(const uint32_t *words,uint32_t count)
{
    if(!ready())return status;
    if(!jp_frame_queue_idle() || pending_open)return fail(JP_FRAME_STATE);
    if(count && !words)return fail(JP_FRAME_ARGUMENT);
    uint32_t before=recorded;
    for(uint32_t i=0;i<count;i++){
        int result=jp_frames_record_begin(1,words[i]);
        if(result){while(recorded>before)unpin(recording[--recorded]);return result;}
        jp_frames_record_commit();
    }
    return JP_FRAME_OK;
}
int jp_frames_defer_surface_free(void *pixels)
{
    if(!initialized || !pixels)return 0;
    for(uint32_t i=0;i<resource_capacity;i++)if(resources[i].refs && resources[i].pixels==pixels){
        if(!resources[i].orphaned){resources[i].orphaned=1;orphaned_bytes+=resources[i].bytes;}return 1;
    }
    return 0;
}
static struct frame *find(uint64_t id)
{
    if(!initialized){fail(JP_FRAME_NOT_INITIALIZED);return NULL;}
    for(uint32_t i=0;i<frame_capacity;i++)if(frames[i].id==id && id)return &frames[i];
    fail(JP_FRAME_STALE);return NULL;
}
static int guard(const struct frame *f)
{
    if(!f->allocation)return 1;
    for(size_t i=0;i<32;i++)if(f->allocation[i]!=0xa5 || f->allocation[f->cost-32+i]!=0x5a)return 0;
    return 1;
}
int jp_frames_capture_lists(const void *const data[5],const size_t sizes[5],uint64_t *out)
{
    if(!ready())return status;
    if(!out || !data || !sizes)return fail(JP_FRAME_ARGUMENT);
    if(pending_open || !jp_frame_queue_idle())return fail(JP_FRAME_STATE);
    struct frame *f=NULL;for(uint32_t i=0;i<frame_capacity;i++)if(!frames[i].id){f=&frames[i];break;}
    if(!f || !next_id)return fail(JP_FRAME_CAPACITY);
    size_t total=0;
    for(unsigned i=0;i<5;i++){
        if((sizes[i] && !data[i]) || sizes[i]%32 || sizes[i]>SIZE_MAX-total)return fail(JP_FRAME_ARGUMENT);
        total+=sizes[i];
    }
    if(total>SIZE_MAX-64)return fail(JP_FRAME_ARGUMENT);
    size_t cost=total?total+64:0;if(!room(cost))return fail(JP_FRAME_BUDGET);
    unsigned char *allocation=total?_aligned_malloc(cost,32):NULL;
    if(total && !allocation)return fail(JP_FRAME_ALLOCATION);
    unsigned char *bytes=allocation?allocation+32:NULL;
    if(allocation){memset(allocation,0xa5,32);memset(bytes+total,0x5a,32);}
    size_t offset=0;
    for(unsigned i=0;i<5;i++){
        f->offsets[i]=offset;f->sizes[i]=sizes[i];if(sizes[i])memcpy(bytes+offset,data[i],sizes[i]);offset+=sizes[i];
    }
    memcpy(f->resources,recording,(size_t)recorded*sizeof(uint32_t));f->count=recorded;recorded=0;
    f->allocation=allocation;f->data=bytes;f->cost=cost;f->id=next_id++;snapshot_bytes+=cost;++live;*out=f->id;return JP_FRAME_OK;
}
int jp_frame_release(uint64_t id)
{
    struct frame *f=find(id);if(!f)return status;
    if(!guard(f))return fail(JP_FRAME_GUARD);
    drop(f);return JP_FRAME_OK; /* Cleanup also works after a sticky error. */
}
const void *jp_frame_list(uint64_t id,unsigned list,size_t *bytes)
{
    if(!bytes || list>=5){fail(JP_FRAME_ARGUMENT);return NULL;}
    struct frame *f=find(id);if(!f)return NULL;
    if(!guard(f)){fail(JP_FRAME_GUARD);return NULL;}
    *bytes=f->sizes[list];return f->sizes[list]?f->data+f->offsets[list]:NULL;
}
const void *jp_frame_surface(uint64_t id,uint32_t word,size_t *bytes)
{
    if(!bytes){fail(JP_FRAME_ARGUMENT);return NULL;}
    struct frame *f=find(id);if(!f)return NULL;
    if(!guard(f)){fail(JP_FRAME_GUARD);return NULL;}
    for(uint32_t i=0;i<f->count;i++){
        const struct resource *r=&resources[f->resources[i]];if(r->word==word){*bytes=r->bytes;return r->pixels;}
    }
    fail(JP_FRAME_RESOURCE);return NULL;
}
uint32_t jp_frame_surface_count(uint64_t id)
{struct frame *f=find(id);if(!f)return 0;if(!guard(f)){fail(JP_FRAME_GUARD);return 0;}return f->count;}
