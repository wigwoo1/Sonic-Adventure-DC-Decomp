/* Compile IN PLACE OF the frozen plain/intensity queue, never alongside it.
 * Preserve one owner. Public SDK starts retain texture identity before any
 * header is written; end commits the reference and discard rolls it back.
 */
#include "jp_native_frames_internal.h"
#define jp_queue_initialize jp_unretained_queue_initialize
#define jp_queue_shutdown jp_unretained_queue_shutdown
#define jp_queue_reset_frame jp_unretained_queue_reset_frame
#define jp_queue_discard_active jp_unretained_queue_discard_active
#define njEndVertex jp_unretained_end_vertex
#define njStartVertexNonTexOpaque jp_unretained_nontex_opaque
#define njStartVertexNonTexTrans jp_unretained_nontex_trans
#define njStartVertexTexOpaque jp_unretained_tex_opaque
#define njStartVertexTexTrans jp_unretained_tex_trans
#define njStartVertexNonTexOpaqueIntens jp_unretained_nontex_opaque_intens
#define njStartVertexNonTexTransIntens jp_unretained_nontex_trans_intens
#define njStartVertexTexOpaqueIntens jp_unretained_tex_opaque_intens
#define njStartVertexTexTransIntens jp_unretained_tex_trans_intens
#define njStartVertexTexOpaqueIntensH jp_unretained_tex_opaque_intens_h
#define njStartVertexTexTransIntensH jp_unretained_tex_trans_intens_h
#define njStartVertexTexPunch jp_unretained_tex_punch
#define njStartVertexTexPunchIntens jp_unretained_tex_punch_intens
#define njStartVertexTexPunchIntensH jp_unretained_tex_punch_intens_h
#include "jp_native_queue_intensity.c"
#undef jp_queue_initialize
#undef jp_queue_shutdown
#undef jp_queue_reset_frame
#undef jp_queue_discard_active
#undef njEndVertex
#undef njStartVertexNonTexOpaque
#undef njStartVertexNonTexTrans
#undef njStartVertexTexOpaque
#undef njStartVertexTexTrans
#undef njStartVertexNonTexOpaqueIntens
#undef njStartVertexNonTexTransIntens
#undef njStartVertexTexOpaqueIntens
#undef njStartVertexTexTransIntens
#undef njStartVertexTexOpaqueIntensH
#undef njStartVertexTexTransIntensH
#undef njStartVertexTexPunch
#undef njStartVertexTexPunchIntens
#undef njStartVertexTexPunchIntensH

int jp_frame_queue_idle(void){return active<0;}
int jp_frame_queue_empty(void)
{
    if(active>=0 || dirty)return 0;
    for(unsigned i=0;i<JP_QUEUE_LISTS;i++)if(pools[i].used)return 0;
    return 1;
}
JP_QUEUE_API int jp_queue_initialize(size_t limit)
{
    int result=jp_unretained_queue_initialize(limit);
    if(!result)jp_frames_record_discard();return result;
}
JP_QUEUE_API void jp_queue_shutdown(void)
{jp_unretained_queue_shutdown();jp_frames_record_discard();}
JP_QUEUE_API int jp_queue_reset_frame(void)
{
    int result=jp_unretained_queue_reset_frame();
    if(!result)jp_frames_record_discard();return result;
}
JP_QUEUE_API int jp_queue_discard_active(void)
{
    int result=jp_unretained_queue_discard_active();
    if(!result)jp_frames_record_abort();return result;
}
void njEndVertex(Uint32 *end)
{
    int was_active=active>=0;jp_unretained_end_vertex(end);
    if(was_active && active<0)jp_frames_record_commit();
}
static int retain(int textured)
{
    if(!ready())return 0;
    if(active>=0){fail(JP_QUEUE_STATE);return 0;}
    if(jp_frames_record_begin(textured,_nj_parameterkm_.TexturePARAMBUFFER&0x001fffffu)){
        fail(JP_QUEUE_STATE);return 0;
    }
    return 1;
}
static void *retained_plain(unsigned list,int textured,int opaque)
{
    if(!retain(textured))return NULL;
    void *out=begin(list,textured,opaque);if(!out)jp_frames_record_abort();return out;
}
static void *retained_intensity(unsigned list,int textured,int opaque,Uint32 *color,Uint32 *offset_color,int format)
{
    if(!retain(textured))return NULL;
    void *out=begin_intensity(list,textured,opaque,color,offset_color,format);if(!out)jp_frames_record_abort();return out;
}
void *njStartVertexNonTexOpaque(void){return retained_plain(JP_QUEUE_OPAQUE,0,1);}
void *njStartVertexNonTexTrans(void){return retained_plain(JP_QUEUE_TRANSLUCENT,0,0);}
void *njStartVertexTexOpaque(void){return retained_plain(JP_QUEUE_OPAQUE,1,1);}
void *njStartVertexTexTrans(void){return retained_plain(JP_QUEUE_TRANSLUCENT,1,0);}
void *njStartVertexNonTexOpaqueIntens(Uint32 *c){return retained_intensity(JP_QUEUE_OPAQUE,0,1,c,NULL,1);}
void *njStartVertexNonTexTransIntens(Uint32 *c){return retained_intensity(JP_QUEUE_TRANSLUCENT,0,0,c,NULL,1);}
void *njStartVertexTexOpaqueIntens(Uint32 *c){return retained_intensity(JP_QUEUE_OPAQUE,1,1,c,NULL,1);}
void *njStartVertexTexTransIntens(Uint32 *c){return retained_intensity(JP_QUEUE_TRANSLUCENT,1,0,c,NULL,1);}
void *njStartVertexTexOpaqueIntensH(Uint32 *c,Uint32 *o){return retained_intensity(JP_QUEUE_OPAQUE,1,1,c,o,2);}
void *njStartVertexTexTransIntensH(Uint32 *c,Uint32 *o){return retained_intensity(JP_QUEUE_TRANSLUCENT,1,0,c,o,2);}
void *njStartVertexTexPunch(void){return retained_intensity(JP_QUEUE_PUNCH,1,2,NULL,NULL,0);}
void *njStartVertexTexPunchIntens(Uint32 *c){return retained_intensity(JP_QUEUE_PUNCH,1,2,c,NULL,1);}
void *njStartVertexTexPunchIntensH(Uint32 *c,Uint32 *o){return retained_intensity(JP_QUEUE_PUNCH,1,2,c,o,2);}
int jp_frames_submit(uint64_t *id)
{
    if(!ready() || active>=0 || dirty)return jp_frames_report_error(JP_FRAME_STATE);
    const void *data[5];size_t sizes[5];
    for(unsigned i=0;i<5;i++){
        if(!guards(&pools[i]))return jp_frames_report_error(JP_FRAME_GUARD);
        data[i]=pools[i].data;sizes[i]=pools[i].used;
    }
    int result=jp_frames_capture_lists(data,sizes,id);
    /* All reset preconditions were checked above; one game thread, no callback
     * runs during capture. This reset cannot fail or allocate. */
    if(!result)jp_unretained_queue_reset_frame();return result;
}
