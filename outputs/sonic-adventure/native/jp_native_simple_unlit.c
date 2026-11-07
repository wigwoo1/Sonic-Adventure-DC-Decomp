/* Bounded entry for the original unclipped, untextured unlit triangle drawer. */
#include "jp_native_simple_calc.h"
#include "jp_native_queue.h"
#include <limits.h>

typedef int (*jp_simple_consumer)(NJS_MODEL *,const void *,size_t,void *);
int jp_simple_calc_with_records(NJS_MODEL *,size_t,size_t,NJS_VECTOR *,Float,
                               int32_t *,jp_simple_consumer,void *);
void jp_simple_material_stage(NJS_MATERIAL *,NJS_LIGHT_MATERIAL *,Uint32,
                             Uint32,Uint32,NJS_ARGB,Uint32 *);
Float *jp_simple_emit_unlit_triangles(NJS_MESHSET *,Float *);

static int consume_unlit(NJS_MODEL *model,const void *records,size_t count,void *context)
{
    (void)records;(void)count;
    Uint32 *dispatch=context;
    uint32_t projection[6];
    jp_simple_calc_context_snapshot(projection);
    if(projection[0])return 101; /* requires the original clipped drawer */
    unsigned lane=dispatch[4]==4?JP_QUEUE_TRANSLUCENT:JP_QUEUE_OPAQUE;
    int status=jp_queue_reserve(lane,1u+3u*model->meshsets->nbMesh);
    if(status)return status;
    njGetkmPointer();
    if((status=jp_queue_status()))return status;
    njSetkmModelAttr(dispatch[0]);
    njSetkmColorMode(dispatch[3]);
    Float *cursor=lane==JP_QUEUE_TRANSLUCENT?njStartVertexNonTexTrans():njStartVertexNonTexOpaque();
    if(!cursor)return jp_queue_status();
    cursor=jp_simple_emit_unlit_triangles(model->meshsets,cursor);
    njEndVertex((Uint32 *)cursor);
    njSetkmPointer();
    return jp_queue_status();
}

__declspec(dllexport) int jp_simple_draw_unlit_triangles(
    NJS_MODEL *model,size_t points,size_t normals,size_t indices,size_t materials,
    NJS_VECTOR *light,Float scale,int32_t *projection)
{
    if(!model || !light || !projection || model->nbPoint<0 ||
       model->nbMeshset!=1 || !model->meshsets || !model->mats)return 100;
    NJS_MESHSET *mesh=model->meshsets;
    unsigned material=mesh->type_matId&0x3fff;
    if((mesh->type_matId&NJD_MESHSET_MASK)!=NJD_MESHSET_3 ||
       material>=model->nbMat || material>=materials || !mesh->meshes ||
       !mesh->nbMesh || (size_t)mesh->nbMesh*3>indices)return 100;
    unsigned corners=3u*mesh->nbMesh;
    for(unsigned i=0;i<corners;i++)
        if((Uint16)mesh->meshes[i]>=(unsigned)model->nbPoint)return 100;
    if(_nj_calc_polygon_>INT_MAX-(int)corners || _nj_draw_polygon_>INT_MAX-(int)corners)return 100;
    NJS_LIGHT_MATERIAL saved=_nj_light_material_,prepared=saved;
    Uint32 dispatch[5];
    jp_simple_material_stage(model->mats+material,&prepared,_nj_control_3d_flag_,
                             _nj_constant_attr_and_,_nj_constant_attr_or_,_nj_constant_material_,dispatch);
    if(dispatch[1]!=NJD_FLAG_IGNORE_LIGHT &&
       dispatch[1]!=(NJD_FLAG_IGNORE_LIGHT|NJD_FLAG_USE_FLAT))return 100;
    _nj_light_material_=prepared;
    int rc=jp_simple_calc_with_records(model,points,normals,light,scale,projection,consume_unlit,dispatch);
    _nj_light_material_=saved;
    return rc;
}
