/* Compile instead of jp_native_queue_direct.c: one real queue/cursor owner.
 * Native system configuration uses these pools, never an unused console heap.
 */
#include "jp_native_queue_direct.c"
#include "jp_native_queue_allocation.h"

/* Compile-time allocation seam for the private failure-path qualification.
 * Production uses the same CRT allocator as the existing queue reserve. */
#ifndef JP_QUEUE_PREALLOCATE_ALLOC
#define JP_QUEUE_PREALLOCATE_ALLOC _aligned_malloc
#endif

JP_QUEUE_API size_t jp_queue_capacity_bytes(unsigned list)
{return list<JP_QUEUE_LISTS?pools[list].capacity:0;}

JP_QUEUE_API int jp_queue_preallocate(const size_t minimum_bytes[JP_QUEUE_LISTS])
{
    if(!ready())return status;
    if(!minimum_bytes)return fail(JP_QUEUE_ARGUMENT);
    if(!jp_frame_queue_empty())return fail(JP_QUEUE_STATE);
    size_t minimum[JP_QUEUE_LISTS],extra=0;
    unsigned char *replacement[JP_QUEUE_LISTS]={0};
    /* Copy first: the caller's array may itself reside in an existing pool. */
    memcpy(minimum,minimum_bytes,sizeof minimum);
    if(allocated>memory_budget)return fail(JP_QUEUE_BUDGET);
    for(unsigned i=0;i<JP_QUEUE_LISTS;i++){
        struct pool *p=&pools[i];size_t a,b;
        if(!guards(p))return fail(JP_QUEUE_GUARD);
        if(!offset(p,*cursor(i),&a) ||
           !offset(p,descriptor_cursors[descriptor_indices[i]],&b))
            return fail(JP_QUEUE_BOUNDS);
        if(a || b)return fail(JP_QUEUE_STATE);
        if(minimum[i]%32 || minimum[i]>SIZE_MAX-64)
            return fail(JP_QUEUE_ARGUMENT);
        if(minimum[i]>p->capacity){
            size_t cost=minimum[i]+64;
            if(cost>memory_budget-allocated-extra)return fail(JP_QUEUE_BUDGET);
            extra+=cost;
        }
    }
    /* Stage every changed pool before freeing or changing any old owner. */
    for(unsigned i=0;i<JP_QUEUE_LISTS;i++)if(minimum[i]>pools[i].capacity){
        unsigned char *next=JP_QUEUE_PREALLOCATE_ALLOC(minimum[i]+64,32);
        if(!next){
            for(unsigned j=0;j<JP_QUEUE_LISTS;j++)_aligned_free(replacement[j]);
            return fail(JP_QUEUE_ALLOCATION);
        }
        replacement[i]=next;
        memset(next,0xa5,32);memset(next+32,0,minimum[i]);
        memset(next+32+minimum[i],0x5a,32);
    }
    for(unsigned i=0;i<JP_QUEUE_LISTS;i++)if(replacement[i]){
        struct pool *p=&pools[i];
        size_t old_cost=p->allocation?p->capacity+64:0;
        _aligned_free(p->allocation);
        p->allocation=replacement[i];p->data=replacement[i]+32;
        p->capacity=minimum[i];
        allocated=allocated-old_cost+p->capacity+64;
        *cursor(i)=(Uint32 *)p->data;
        descriptor_cursors[descriptor_indices[i]]=(Uint32 *)p->data;
    }
    return status;
}
