/* One real queue owner; compile in place of queue_allocation.obj. */
#include "jp_native_queue_allocation.c"
#include "jp_native_vertex_lists.h"
#include <limits.h>
int jp_vertex_lists_configure(int32_t op,int32_t om,int32_t tp,int32_t tm,
                            int32_t pt,struct jp_vertex_lists *out)
{
    if(!ready())return status;
    if(!out)return fail(JP_QUEUE_ARGUMENT);
    struct jp_vertex_lists value={0};
    /* Match original selector precedence before checking effective counts. */
    if(op<0){op=100;value.model=1;}
    else if(tp<0 || pt<0){tp=100;pt=0;value.model=2;}
    if(op<0 || om<0 || tp<0 || tm<0 || pt<0)
        return fail(JP_QUEUE_ARGUMENT);
    uint64_t trans=(uint64_t)tp+(uint64_t)pt;
    uint64_t total=(uint64_t)op+(uint64_t)om+trans+(uint64_t)tm;
    /* Original scalar sum plus its 88-word console overhead must fit. */
    if(!total || total>INT_MAX-88)return fail(JP_QUEUE_ARGUMENT);
    value.words[0]=(uint32_t)op;value.words[1]=(uint32_t)om;
    value.words[2]=(uint32_t)trans;value.words[3]=(uint32_t)tm;
    const unsigned order[5]={0,2,4,1,3};
    for(unsigned i=0;i<5;i++){
        size_t bytes=(size_t)value.words[order[i]]*4;
        value.minimum_bytes[i]=(bytes+31)&~(size_t)31;
    }
    /* The existing allocator validates all lists, accounts transient cost,
     * and commits all or none. No dummy contiguous console heap is made. */
    int result=jp_queue_preallocate(value.minimum_bytes);
    if(result)return result;
    *out=value;return JP_QUEUE_OK;
}
