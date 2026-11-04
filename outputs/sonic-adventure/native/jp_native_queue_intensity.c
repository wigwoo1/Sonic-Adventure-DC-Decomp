/* New queue version built around the preserved authored plain-queue source.
 * Compile this translation unit IN PLACE OF jp_native_queue.c, never beside
 * it. Including the frozen implementation keeps exactly one state owner. */
#include "jp_native_queue_intensity.h"
#include "jp_native_queue.c"

static void *begin_intensity(unsigned list,int textured,int opaque,
                             const Uint32 *color,const Uint32 *offset_color,int format)
{
    if(!ready())return NULL;
    int high=format==2;
    if((format && !color) || (high && !offset_color)){fail(JP_QUEUE_ARGUMENT);return NULL;}
    if(active>=0){fail(JP_QUEUE_STATE);return NULL;}
    struct pool *p=&pools[list];size_t position,bytes=high?64:32;
    if(!guards(p)){fail(JP_QUEUE_GUARD);return NULL;}
    if(!offset(p,*cursor(list),&position) || !p->data || position>p->limit || p->limit-position<bytes){fail(JP_QUEUE_BOUNDS);return NULL;}
    /* Preserve raw words, including NaN payloads. Inputs must stay valid
     * through the call; they may share storage with each other. */
    Uint32 colors[4],offsets[4];if(format)memcpy(colors,color,16);
    if(high)memcpy(offsets,offset_color,16);
    Uint32 *header=(Uint32 *)(p->data+position);memset(header,0,bytes);
    header[0]=_nj_parameterkm_.GLOBALPARAMBUFFER;header[1]=_nj_parameterkm_.ISPPARAMBUFFER;
    if(opaque==2)header[0]=(header[0]&~0x07000000u)|0x04000000u;
    Uint32 tsp=_nj_parameterkm_.TSPPARAMBUFFER;
    header[2]=opaque==2?((tsp&~0xfc100000u)|0x94000000u):opaque?((tsp&~0xfc100000u)|0x20080000u):tsp;
    if(textured)header[3]=_nj_parameterkm_.TexturePARAMBUFFER;
    if(format)memcpy(header+(high?8:4),colors,16);
    if(high)memcpy(header+12,offsets,16);
    _builtin_prefetch(header);if(high)_builtin_prefetch(header+8);
    active=(int)list;active_start=(unsigned char *)header+bytes;active_limit=p->data+p->limit;
    return active_start;
}
void *njStartVertexNonTexOpaqueIntens(Uint32 *color) { return begin_intensity(JP_QUEUE_OPAQUE,0,1,color,NULL,1); }
void *njStartVertexNonTexTransIntens(Uint32 *color) { return begin_intensity(JP_QUEUE_TRANSLUCENT,0,0,color,NULL,1); }
void *njStartVertexTexOpaqueIntens(Uint32 *color) { return begin_intensity(JP_QUEUE_OPAQUE,1,1,color,NULL,1); }
void *njStartVertexTexTransIntens(Uint32 *color) { return begin_intensity(JP_QUEUE_TRANSLUCENT,1,0,color,NULL,1); }
void *njStartVertexTexOpaqueIntensH(Uint32 *color,Uint32 *offset) { return begin_intensity(JP_QUEUE_OPAQUE,1,1,color,offset,2); }
void *njStartVertexTexTransIntensH(Uint32 *color,Uint32 *offset) { return begin_intensity(JP_QUEUE_TRANSLUCENT,1,0,color,offset,2); }
void *njStartVertexTexPunch(void) { return begin_intensity(JP_QUEUE_PUNCH,1,2,NULL,NULL,0); }
void *njStartVertexTexPunchIntens(Uint32 *color) { return begin_intensity(JP_QUEUE_PUNCH,1,2,color,NULL,1); }
void *njStartVertexTexPunchIntensH(Uint32 *color,Uint32 *offset) { return begin_intensity(JP_QUEUE_PUNCH,1,2,color,offset,2); }

int jp_queue_discard_active(void)
{
    if(!ready())return status;
    if(active<0)return fail(JP_QUEUE_STATE);
    struct pool *p=&pools[active];size_t position;
    if(!guards(p))return fail(JP_QUEUE_GUARD);
    if(!offset(p,*cursor((unsigned)active),&position) || position!=p->used)return fail(JP_QUEUE_BOUNDS);
    uintptr_t first=(uintptr_t)p->data+position,vertex=(uintptr_t)active_start,last=(uintptr_t)active_limit;
    if(vertex<first || (vertex-first!=32 && vertex-first!=64) || last<vertex || last>(uintptr_t)p->data+p->capacity)return fail(JP_QUEUE_BOUNDS);
    memset((void *)first,0,(size_t)(last-first));
    active=-1;active_start=active_limit=NULL;
    /* dirty describes earlier committed batches. Discard must not lose it. */
    return status;
}
