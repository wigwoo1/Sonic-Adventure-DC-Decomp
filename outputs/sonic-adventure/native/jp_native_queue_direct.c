/* One queue, cursor table and frame recorder. PRINT_OPEN/PRINT_CLOSE bypass
 * Ninja's cached parameters: only the translucent descriptor advances.
 * Header padding is zero; defined words and vertex blocks are copied exactly.
 */
#define jp_queue_reserve jp_direct_parameter_reserve
#include "jp_native_queue_frames.c"
#undef jp_queue_reserve
#include "jp_native_queue_direct.h"

JP_QUEUE_API int jp_queue_reserve(unsigned list,uint32_t blocks)
{
    if(!ready())return status;
    if(list>=JP_QUEUE_LISTS || active>=0)
        return jp_direct_parameter_reserve(list,blocks);
    struct pool *p=&pools[list];size_t parameter_position,descriptor_position;
    if(!guards(p))return fail(JP_QUEUE_GUARD);
    if(!offset(p,*cursor(list),&parameter_position) ||
       !offset(p,descriptor_cursors[descriptor_indices[list]],&descriptor_position))
        return fail(JP_QUEUE_BOUNDS);
    /* Existing producers reserve before njGetkmPointer. A direct SDK print
     * has advanced only the descriptor; reserve from that newer end, while
     * retaining the cached parameter cursor's original relative position. */
    if(descriptor_position<=parameter_position)
        return jp_direct_parameter_reserve(list,blocks);
    *cursor(list)=descriptor_cursors[descriptor_indices[list]];
    int result=jp_direct_parameter_reserve(list,blocks);
    *cursor(list)=p->data?(Uint32 *)(p->data+parameter_position):NULL;
    return result;
}

int jp_queue_append_direct_translucent(const uint32_t words[4],
                                      const void *vertices,uint32_t blocks)
{
    if(!ready())return status;
    if(!words || (blocks && !vertices) || blocks==UINT32_MAX)
        return fail(JP_QUEUE_ARGUMENT);
    if(active>=0 || dirty)return fail(JP_QUEUE_STATE);
    const unsigned list=JP_QUEUE_TRANSLUCENT;
    struct pool *p=&pools[list];size_t saved,position;
    if(!guards(p))return fail(JP_QUEUE_GUARD);
    if(!offset(p,*cursor(list),&saved) ||
       !offset(p,descriptor_cursors[KM_TRANS_POLYGON],&position) || position!=p->used)
        return fail(JP_QUEUE_BOUNDS);
    if((size_t)blocks>(SIZE_MAX-32)/32)return fail(JP_QUEUE_BOUNDS);
    size_t bytes=(size_t)blocks*32;
    /* Reject aliasing before a reserve can invalidate either input. */
    uintptr_t a=(uintptr_t)vertices,h=(uintptr_t)words;
    if(bytes>UINTPTR_MAX-a || 16>UINTPTR_MAX-h)return fail(JP_QUEUE_ARGUMENT);
    for(unsigned i=0;i<JP_QUEUE_LISTS;i++)if(pools[i].allocation){
        uintptr_t lo=(uintptr_t)pools[i].allocation,hi=lo+pools[i].capacity+64;
        if((bytes && a<hi && lo<a+bytes) || (h<hi && lo<h+16))
            return fail(JP_QUEUE_ARGUMENT);
    }
    uint32_t header_words[4];memcpy(header_words,words,sizeof header_words);
    /* Pin before changing the stream. Failed growth cancels only this
     * pending reference; previously committed batches stay recorded. */
    if(jp_frames_record_begin(1,header_words[3]&0x001fffffu))
        return fail(JP_QUEUE_STATE);
    *cursor(list)=descriptor_cursors[KM_TRANS_POLYGON];
    int result=jp_queue_reserve(list,blocks+1);
    *cursor(list)=p->data?(Uint32 *)(p->data+saved):NULL;
    if(result){jp_frames_record_abort();return result;}
    unsigned char *header=p->data+position;
    memset(header,0,32);memcpy(header,header_words,16);
    if(bytes)memcpy(header+32,vertices,bytes);
    p->used=position+32+bytes;
    descriptor_cursors[KM_TRANS_POLYGON]=(Uint32 *)(p->data+p->used);
    jp_frames_record_commit();return status;
}
