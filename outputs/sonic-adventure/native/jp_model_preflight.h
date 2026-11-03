/* Caller supplies truthful accessible array lengths; this validates contents
 * and a worst-case queue bound before entering the pinned unchecked drawer. */
#ifndef JP_MODEL_PREFLIGHT_H
#define JP_MODEL_PREFLIGHT_H
#include "jp_native_render_state.h"
struct jp_mesh_span { size_t indices,uvs; };
struct jp_model_bound { uint32_t opaque_blocks,translucent_blocks,vertices,meshsets; };
JP_RENDER_API int jp_model_preflight(const NJS_MODEL *model,size_t point_count,
    size_t normal_count,size_t meshset_count,size_t material_count,
    const struct jp_mesh_span *spans,struct jp_model_bound *out);
JP_RENDER_API int jp_model_reserve(const struct jp_model_bound *bound);
#endif
