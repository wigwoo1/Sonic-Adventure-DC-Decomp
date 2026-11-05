/* Native adaptation of Ninja's vertex initialization lifecycle.
 * Imports the real queue/texture owner and the separate real wait owner.
 * The build generates imports from the qualified shared list structure.
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>
#undef NULL
#include <ninja.h>
#include <kamui.h>
#include "jp_vertex_lifecycle_imports.h"
KMSYSTEMCONFIGSTRUCT _nj_SystemConfigStruct_;
Uint32 _nj_vertex_buffer_op_,_nj_vertex_buffer_om_,_nj_vertex_buffer_tp_;
Uint32 _nj_vertex_buffer_tm_,_nj_vertex_buffer_pt_,_nj_vertex_buffer_size_;
Int _nj_vertex_2v_type_;
static int lifecycle_status,busy;
/* Positive signed integer to binary32, truncating exactly as SH-4 RZ. */
static uint32_t integer_bits(uint32_t value)
{
    if(!value)return 0;
    unsigned bit=0;for(uint32_t n=value;n>>=1;)bit++;
    uint32_t significand=bit>23?value>>(bit-23):value<<(23-bit);
    return ((bit+127)<<23)|(significand&0x7fffff);
}
__declspec(dllexport) int jp_vertex_lifecycle_status(void){return lifecycle_status;}
__declspec(dllexport) int jp_vertex_lifecycle_initialize(int32_t op,int32_t om,
                                                       int32_t tp,int32_t tm,int32_t pt)
{
    if(busy)return 1;
    int32_t effective_op=op,effective_tp=tp,effective_pt=pt;
    if(effective_op<0)effective_op=100;
    else if(effective_tp<0 || effective_pt<0){effective_tp=100;effective_pt=0;}
    if(om<0 || tm<0 || effective_tp<0 || effective_pt<0)return lifecycle_status=2;
    uint64_t trans=(uint64_t)effective_tp+(uint64_t)effective_pt;
    uint64_t total=(uint64_t)effective_op+(uint64_t)om+trans+(uint64_t)tm;
    if(!total || total>INT_MAX-88)return lifecycle_status=2;
    KMUINT32 available,largest;
    if(kmGetFreeTextureMem(&available,&largest))return lifecycle_status=3;
    busy=1;
    if(jp_vblank_arm_ninja_blank()){busy=0;return lifecycle_status=4;}
    int result=jp_vblank_wait_ninja_blank();
    if(result){jp_vblank_cancel_ninja_blank();busy=0;return lifecycle_status=5;}
    /* A game callback may have changed texture ownership while waiting. */
    if(kmGetFreeTextureMem(&available,&largest)){jp_vblank_cancel_ninja_blank();busy=0;return lifecycle_status=3;}
    struct jp_vertex_lists lists;
    if(jp_vertex_lists_configure(op,om,tp,tm,pt,&lists)){jp_vblank_cancel_ninja_blank();busy=0;return lifecycle_status=6;}
    _nj_vertex_2v_type_=(Int)lists.model;
    _nj_vertex_buffer_op_=lists.words[0];_nj_vertex_buffer_om_=lists.words[1];
    _nj_vertex_buffer_tp_=lists.words[2];_nj_vertex_buffer_tm_=lists.words[3];
    _nj_vertex_buffer_pt_=0;_nj_vertex_buffer_size_=(Uint32)total;
    _nj_SystemConfigStruct_.flags=lists.model?KM_CONFIGFLAG_ENABLE_2V_LATENCY:
                                          KM_CONFIGFLAG_NOWAIT_FINISH_TEXTUREDMA;
    _nj_SystemConfigStruct_.nVbufModel=lists.model==1?KM_VERTEXBUFMODEL_NOBUF_OPAQUE:
        lists.model==2?KM_VERTEXBUFMODEL_NOBUF_TRANS:KM_VERTEXBUFMODEL_NORMAL;
    /* Host queues are separate actual allocations, not a console heap alias. */
    _nj_SystemConfigStruct_.pVertexBuffer=NULL;
    _nj_SystemConfigStruct_.nVertexBufferSize=(Uint32)total+88;
    for(unsigned i=0;i<5;i++){
        uint32_t bits=jp_rz_scalar32_divide(jp_rz_scalar32_multiply(
            integer_bits(lists.words[i]),0x42c80000),integer_bits((uint32_t)total));
        memcpy(&_nj_SystemConfigStruct_.fBufferSize[i],&bits,4);
    }
    jp_texture_cache_maximum(njCalcTexture(NJD_TEXMEM_FREESIZE));
    busy=0;return lifecycle_status=0;
}
void njInitVertexBuffer(Sint32 op,Sint32 om,Sint32 tp,Sint32 tm,Sint32 pt)
{(void)jp_vertex_lifecycle_initialize(op,om,tp,tm,pt);}
