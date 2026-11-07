/* Bounded multi-mesh entry for the available original Simple drawers. */
#include "jp_native_simple_calc.h"
#include "jp_native_queue.h"
#include "jp_native_textures.h"
#include <limits.h>
#include <string.h>

typedef int (*simple_consumer)(NJS_MODEL *,const void *,size_t,void *);
int jp_simple_calc_with_records(NJS_MODEL *,size_t,size_t,NJS_VECTOR *,Float,
                                int32_t *,simple_consumer,void *);
int jp_simple_prepare_base_light(NJS_VECTOR *,Float *,Float *);
void jp_simple_material_stage(NJS_MATERIAL *,NJS_LIGHT_MATERIAL *,Uint32,
                              Uint32,Uint32,NJS_ARGB,Uint32 *);

Float *jp_simple_emit_unlit_triangles(NJS_MESHSET *,Float *);
Float *jp_simple_emit_unlit_triangles_clip(NJS_MESHSET *,Float *,Float,Float);
Float *simple_draw_unlit_strips_cull(NJS_MESHSET *,Float *);
Float *simple_draw_unlit_strips_near(NJS_MESHSET *,Float *,Float,Float);
Float *jp_simple_emit_unlit_tex(NJS_MESHSET *,Float *);
Float *jp_simple_emit_unlit_tex_clip(NJS_MESHSET *,Float *,Float,Float);
Float *simple_draw_unlit_tex_strips_cull(NJS_MESHSET *,Float *);
Float *simple_draw_unlit_tex_strips_near(NJS_MESHSET *,Float *,Float,Float);
Float *jp_simple_emit_smooth_triangles(NJS_MESHSET *,Float *,Float);
Float *jp_simple_emit_smooth_triangles_clip(NJS_MESHSET *,Float *,Float,Float,Float);
Float *jp_simple_emit_smooth_tex(NJS_MESHSET *,Float *,Float);
Float *jp_simple_emit_smooth_tex_clip(NJS_MESHSET *,Float *,Float,Float,Float);
Float *simple_draw_smooth_tex_strips_cull(NJS_MESHSET *,Float *,Float);
Float *simple_draw_smooth_tex_strips_near(NJS_MESHSET *,Float *,Float,Float,Float);

struct simple_mesh_span { size_t indices, uvs; };
struct simple_model_context { uint32_t blocks[2]; Float ambient; };
enum { SIMPLE_INVALID=100, SIMPLE_UNSUPPORTED=101, SIMPLE_TEXTURE=102 };

static void material_stage(NJS_MATERIAL *material,NJS_LIGHT_MATERIAL *light,Uint32 dispatch[5])
{
    jp_simple_material_stage(material,light,_nj_control_3d_flag_,_nj_constant_attr_and_,
                             _nj_constant_attr_or_,_nj_constant_material_,dispatch);
}

static int measure(const NJS_MESHSET *mesh,const struct simple_mesh_span *span,
                   unsigned points,uint32_t *corners,uint32_t *bound)
{
    size_t pos=0;
    int strip=(mesh->type_matId&NJD_MESHSET_MASK)==NJD_MESHSET_TRIMESH;
    if(!mesh->meshes || !mesh->nbMesh)return SIMPLE_INVALID;
    for(unsigned j=0;j<mesh->nbMesh;j++){
        unsigned n=3;
        if(strip){
            if(pos>=span->indices)return SIMPLE_INVALID;
            n=(Uint16)mesh->meshes[pos++]&0x7fff;
        }
        if(n<3 || n>span->indices-pos || n>(unsigned)INT_MAX-*corners ||
           4u*(n-2u)>UINT32_MAX/32u-*bound)return SIMPLE_INVALID;
        for(unsigned i=0;i<n;i++)
            if((Uint16)mesh->meshes[pos+i]>=points)return SIMPLE_INVALID;
        pos+=n;
        *corners+=n;
        *bound+=4u*(n-2u);
    }
    return 0;
}

static Float *emit(NJS_MESHSET *mesh,Float *cursor,Uint32 flags,int clipped,
                   const Float scales[2],Float ambient)
{
    int strip=(mesh->type_matId&NJD_MESHSET_MASK)==NJD_MESHSET_TRIMESH;
    int unlit=!!(flags&NJD_FLAG_IGNORE_LIGHT);
    int textured=!!(flags&NJD_FLAG_USE_TEXTURE);
    if(strip){
        if(!textured)
            return clipped?simple_draw_unlit_strips_near(mesh,cursor,scales[0],scales[1]):
                           simple_draw_unlit_strips_cull(mesh,cursor);
        if(unlit)
            return clipped?simple_draw_unlit_tex_strips_near(mesh,cursor,scales[0],scales[1]):
                           simple_draw_unlit_tex_strips_cull(mesh,cursor);
        return clipped?simple_draw_smooth_tex_strips_near(mesh,cursor,scales[0],scales[1],ambient):
                       simple_draw_smooth_tex_strips_cull(mesh,cursor,ambient);
    }
    if(unlit && !textured)
        return clipped?jp_simple_emit_unlit_triangles_clip(mesh,cursor,scales[0],scales[1]):
                       jp_simple_emit_unlit_triangles(mesh,cursor);
    if(unlit)
        return clipped?jp_simple_emit_unlit_tex_clip(mesh,cursor,scales[0],scales[1]):
                       jp_simple_emit_unlit_tex(mesh,cursor);
    if(textured)
        return clipped?jp_simple_emit_smooth_tex_clip(mesh,cursor,scales[0],scales[1],ambient):
                       jp_simple_emit_smooth_tex(mesh,cursor,ambient);
    return clipped?jp_simple_emit_smooth_triangles_clip(mesh,cursor,scales[0],scales[1],ambient):
                   jp_simple_emit_smooth_triangles(mesh,cursor,ambient);
}

static int consume_model(NJS_MODEL *model,const void *records,size_t count,void *context)
{
    (void)records;
    (void)count;
    const struct simple_model_context *draw=context;
    uint32_t projection[6];
    Float scales[2];
    jp_simple_calc_context_snapshot(projection);
    memcpy(scales,projection+4,sizeof scales);
    for(unsigned lane=0;lane<2;lane++)
        if(draw->blocks[lane] && jp_queue_reserve(lane,draw->blocks[lane]))return jp_queue_status();
    njGetkmPointer();
    if(jp_queue_status())return jp_queue_status();
    Uint32 dispatch[5];
    unsigned previous=UINT_MAX;
    for(unsigned i=0;i<model->nbMeshset;i++){
        NJS_MESHSET *mesh=model->meshsets+i;
        unsigned id=mesh->type_matId&0x3fff;
        if(id!=previous){
            material_stage(model->mats+id,&_nj_light_material_,dispatch);
            if(dispatch[4]<=2 && njSetkmTextureNum(dispatch[2])!=1)return SIMPLE_TEXTURE;
            njSetkmModelAttr(dispatch[0]);
            njSetkmColorMode(dispatch[3]);
            previous=id;
        }
        Uint32 *color=(Uint32 *)&_nj_light_material_.diffuse;
        Float *cursor;
        switch(dispatch[4]){
        case 1: cursor=njStartVertexTexOpaqueIntens(color);break;
        case 2: cursor=njStartVertexTexTransIntens(color);break;
        case 3: cursor=njStartVertexNonTexOpaque();break;
        default: cursor=njStartVertexNonTexTrans();break;
        }
        if(!cursor)return jp_queue_status();
        cursor=emit(mesh,cursor,dispatch[1],projection[0]!=0,scales,draw->ambient);
        njEndVertex((Uint32 *)cursor);
        njSetkmPointer();
        if(jp_queue_status())return jp_queue_status();
    }
    return 0;
}

__declspec(dllexport) int jp_simple_draw_model_bounded(
    NJS_MODEL *model,size_t points,size_t normals,size_t materials,
    const struct simple_mesh_span *spans,size_t meshsets,int32_t *projection)
{
    if(!model || !projection || model->nbPoint<0 || !model->meshsets || !model->mats ||
       !model->nbMeshset || model->nbMeshset>meshsets || !spans)return SIMPLE_INVALID;
    struct simple_model_context draw={{0,0},0};
    uint64_t total_corners=0,total_draw=0;
    NJS_LIGHT_MATERIAL saved=_nj_light_material_,prepared=saved;
    for(unsigned i=0;i<model->nbMeshset;i++){
        const NJS_MESHSET *mesh=model->meshsets+i;
        unsigned id=mesh->type_matId&0x3fff;
        unsigned kind=mesh->type_matId&NJD_MESHSET_MASK;
        if(id>=model->nbMat || id>=materials)return SIMPLE_INVALID;
        if(kind!=NJD_MESHSET_3 && kind!=NJD_MESHSET_TRIMESH)return SIMPLE_UNSUPPORTED;
        Uint32 dispatch[5];
        material_stage(model->mats+id,&prepared,dispatch);
        Uint32 flags=dispatch[1];
        int unlit=!!(flags&NJD_FLAG_IGNORE_LIGHT);
        int textured=!!(flags&NJD_FLAG_USE_TEXTURE);
        if((flags&NJD_FLAG_USE_ENV) || (!unlit && (flags&NJD_FLAG_USE_FLAT)) ||
           (!unlit && !textured && kind==NJD_MESHSET_TRIMESH))return SIMPLE_UNSUPPORTED;
        if(!unlit && !model->normals)return SIMPLE_INVALID;
        uint32_t corners=0,bound=0;
        int status=measure(mesh,spans+i,(unsigned)model->nbPoint,&corners,&bound);
        if(status)return status;
        if(textured){
            if(!mesh->vertuv || corners>spans[i].uvs)return SIMPLE_INVALID;
            const NJS_TEXMEMLIST *texture=jp_renderer_texture_lookup(_nj_current_texlist,dispatch[2]);
            if(!texture || (texture->texparambuffer&0x001fffffu)>0x00100000u)return SIMPLE_TEXTURE;
        }
        unsigned lane=(dispatch[4]==2 || dispatch[4]==4)?1:0;
        if(bound>=UINT32_MAX-draw.blocks[lane])return SIMPLE_INVALID;
        draw.blocks[lane]+=1+bound;
        total_corners+=corners;
        total_draw+=bound+(kind==NJD_MESHSET_TRIMESH?corners:0);
    }
    if(total_corners>INT_MAX || total_draw>INT_MAX ||
       _nj_calc_polygon_>INT_MAX-(int)total_corners || _nj_draw_polygon_>INT_MAX-(int)total_draw)
        return SIMPLE_INVALID;
    NJS_VECTOR direction;
    Float scale;
    if(jp_simple_prepare_base_light(&direction,&scale,&draw.ambient))return SIMPLE_INVALID;
    int status=jp_simple_calc_with_records(model,points,normals,&direction,scale,projection,consume_model,&draw);
    _nj_light_material_=saved;
    return status;
}
