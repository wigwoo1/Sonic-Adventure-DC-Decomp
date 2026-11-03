#include "jp_native_textures.h"
#include "jp_model_preflight.h"
#include "jp_native_queue.h"
#include <limits.h>
static int finite_vector(const NJS_VECTOR *v)
{
    return __builtin_isfinite(v->x) && __builtin_isfinite(v->y) && __builtin_isfinite(v->z);
}
int jp_model_preflight(const NJS_MODEL *model,size_t points,size_t normals,
    size_t meshsets,size_t materials,const struct jp_mesh_span *spans,struct jp_model_bound *out)
{
    if(jp_render_state_check_guards()!=JP_RENDER_OK)return jp_render_state_status();
    if(!model || !out || !spans || !model->points || !model->normals ||
       !model->meshsets || !model->mats || model->nbPoint<1 ||
       (uint32_t)model->nbPoint>jp_render_state_capacity() ||
       (size_t)model->nbPoint>points || (size_t)model->nbPoint>normals ||
       !model->nbMeshset || !model->nbMat || model->nbMeshset>meshsets || model->nbMat>materials ||
       !finite_vector(&model->center) || !__builtin_isfinite(model->r) || model->r<0)return JP_RENDER_ARGUMENT;
    for(Int i=0;i<model->nbPoint;i++)if(!finite_vector(&model->points[i]) || !finite_vector(&model->normals[i]))return JP_RENDER_ARGUMENT;
    uint64_t blocks[2]={0,0};
    for(Uint32 i=0;i<model->nbMeshset;i++) {
        const NJS_MESHSET *mesh=&model->meshsets[i];
        Uint32 material=mesh->type_matId&0x3fff;
        if(material>=model->nbMat || (mesh->nbMesh && !mesh->meshes))return JP_RENDER_ARGUMENT;
        const NJS_MATERIAL *mat=&model->mats[material];Uint32 own=mat->attrflags,drawn=own;
        if(_nj_control_3d_flag_&NJD_CONTROL_3D_CONSTANT_ATTR)drawn=(drawn&_nj_constant_attr_and_)|_nj_constant_attr_or_;
        if((drawn&NJD_FLAG_USE_TEXTURE) && !jp_renderer_texture_lookup(_nj_current_texlist,mat->attr_texId&0x3fffffff))return JP_RENDER_STATE;
        size_t cursor=0,uvs=0;uint64_t bound=1; /* one header even for no polygons */
        unsigned kind=(mesh->type_matId>>14)&3;
        for(Uint32 polygon=0;polygon<mesh->nbMesh;polygon++) {
            size_t size;
            if(kind<2)size=kind==0?3:4;
            else {
                if(cursor>=spans[i].indices)return JP_RENDER_ARGUMENT;
                size=(Uint16)mesh->meshes[cursor++]&0x7fff;
            }
            if(size<3 || cursor>spans[i].indices || size>spans[i].indices-cursor)return JP_RENDER_ARGUMENT;
            for(size_t j=0;j<size;j++)if(mesh->meshes[cursor+j]<0 || mesh->meshes[cursor+j]>=model->nbPoint)return JP_RENDER_ARGUMENT;
            cursor+=size;
            if(uvs>SIZE_MAX-size)return JP_RENDER_ARGUMENT;
            uvs+=size;
            /* Pinned send_clipped_strips handles size-2 triangles. Each loop
             * emits at most two strip-closing vertices and four clipped ones,
             * with at most one final vertex. Eight per triangle plus three is
             * deliberately conservative and also exceeds the unclipped size. */
            bound+=(uint64_t)(size-2)*8+3;
            if(bound>UINT32_MAX)return JP_RENDER_BUDGET;
        }
        if((own&NJD_FLAG_USE_TEXTURE) && !(own&NJD_FLAG_USE_ENV) &&
           (uvs>spans[i].uvs || (uvs && !mesh->vertuv)))return JP_RENDER_ARGUMENT;
        unsigned list=(drawn&NJD_FLAG_USE_ALPHA)!=0;
        blocks[list]+=bound;if(blocks[list]>UINT32_MAX)return JP_RENDER_BUDGET;
    }
    struct jp_model_bound result={(uint32_t)blocks[0],(uint32_t)blocks[1],(uint32_t)model->nbPoint,model->nbMeshset};
    *out=result;return JP_RENDER_OK;
}
static int queue_result(int code)
{
    switch(code) {
    case JP_QUEUE_OK:return JP_RENDER_OK;
    case JP_QUEUE_BUDGET:return JP_RENDER_BUDGET;
    case JP_QUEUE_ALLOCATION:return JP_RENDER_ALLOCATION;
    case JP_QUEUE_GUARD:return JP_RENDER_GUARD;
    case JP_QUEUE_ARGUMENT:case JP_QUEUE_BOUNDS:return JP_RENDER_ARGUMENT;
    case JP_QUEUE_NOT_INITIALIZED:return JP_RENDER_NOT_INITIALIZED;
    default:return JP_RENDER_STATE;
    }
}
int jp_model_reserve(const struct jp_model_bound *bound)
{
    if(!bound || !bound->vertices || bound->vertices>jp_render_state_capacity())return JP_RENDER_ARGUMENT;
    if(jp_render_state_check_guards()!=JP_RENDER_OK)return jp_render_state_status();
    int result=jp_queue_reserve(JP_QUEUE_OPAQUE,bound->opaque_blocks);
    if(result!=JP_QUEUE_OK)return queue_result(result);
    result=jp_queue_reserve(JP_QUEUE_TRANSLUCENT,bound->translucent_blocks);
    return queue_result(result);
}
