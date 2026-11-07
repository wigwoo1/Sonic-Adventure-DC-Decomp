/* Bridge the original pointer-only SDK entry to caller-owned model extents. */
#include "jp_native_simple_calc.h"

struct simple_mesh_span { size_t indices, uvs; };
struct simple_model_span {
    NJS_MODEL *model;
    size_t points, normals, materials;
    const struct simple_mesh_span *spans;
    size_t meshsets;
};

int jp_simple_draw_model_bounded(NJS_MODEL *,size_t,size_t,size_t,
                                const struct simple_mesh_span *,size_t,int32_t *);

/* One game thread owns the scope. The caller keeps descriptors and model
 * storage alive for the full traversal, including its motion callbacks. */
static const struct simple_model_span *registered_models;
static size_t registered_count;
static int first_error;

__declspec(dllexport) int jp_simple_model_scope_begin(const struct simple_model_span *entries,size_t count)
{
    if(registered_models || !entries || !count || count>65535)return 103;
    for(size_t i=0;i<count;i++){
        if(!entries[i].model || !entries[i].spans || !entries[i].meshsets ||
           !entries[i].points || !entries[i].materials)return 100;
    }
    registered_models=entries;
    registered_count=count;
    first_error=0;
    return 0;
}

__declspec(dllexport) int jp_simple_model_scope_end(void)
{
    if(!registered_models)return 103;
    registered_models=NULL;
    registered_count=0;
    return first_error;
}

__declspec(dllexport) int jp_simple_model_scope_status(void)
{
    return first_error;
}

void njSimpleDrawModel(NJS_MODEL *model)
{
    if(first_error)return;
    for(size_t i=0;i<registered_count;i++){
        const struct simple_model_span *entry=registered_models+i;
        if(entry->model!=model)continue;
        int32_t projection;
        first_error=jp_simple_draw_model_bounded(model,entry->points,entry->normals,
                       entry->materials,entry->spans,entry->meshsets,&projection);
        return;
    }
    first_error=104; /* No provenance for this SDK model pointer. */
}
