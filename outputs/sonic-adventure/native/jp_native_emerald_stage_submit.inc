/* Convert the already validated native mesh view into the triangle form
 * used by the bounded terrain renderer. Preserve winding and UV corners. */
static int submit_stage_mesh(NJS_MODEL *model, unsigned index,
                             const struct simple_mesh_span *span)
{
    NJS_MESHSET mesh = model->meshsets[index];
    unsigned kind = mesh.type_matId & NJD_MESHSET_MASK;
    NJS_MATERIAL material = model->mats[mesh.type_matId & 0x3fff];
    int textured = !!(material.attrflags & NJD_FLAG_USE_TEXTURE);
    int environment = !!(material.attrflags & NJD_FLAG_USE_ENV);
    if (textured && !mesh.vertuv) return -1;
    const Sint16 *indices = mesh.meshes;
    size_t input = 0, triangles = 0;
    for (unsigned i = 0; i < mesh.nbMesh; ++i) {
        if (input >= span->indices) return -1;
        unsigned n = kind == NJD_MESHSET_TRIMESH ? (Uint16)indices[input++] & 0x7fff
                                                : kind == NJD_MESHSET_4 ? 4 : 3;
        if (n < 3 || n > span->indices-input) return -1;
        triangles += n-2;
        input += n;
    }
    if (!triangles || triangles > UINT16_MAX) return -1;
    Sint16 *order = malloc(triangles * 3 * sizeof *order);
    NJS_COLOR *uv = textured ? malloc(triangles * 3 * sizeof *uv) : NULL;
    if (!order || (textured && !uv)) {
        free(uv);
        free(order);
        return -1;
    }
    input = 0;
    size_t corner = 0, written = 0;
    for (unsigned i = 0; i < mesh.nbMesh; ++i) {
        unsigned header = kind == NJD_MESHSET_TRIMESH ? (Uint16)indices[input++]
                          : kind == NJD_MESHSET_4 ? 4 : 3;
        unsigned n = header & 0x7fff;
        for (unsigned j = 0; j < n-2; ++j) {
            unsigned first = kind == NJD_MESHSET_4 ? 0 : j;
            unsigned next = j+1, last = j+2;
            if (kind == NJD_MESHSET_TRIMESH && (((header >> 15)+j)&1)) {
                next = j+2;
                last = j+1;
            }
            unsigned vertices[3] = {first, next, last};
            for (unsigned k = 0; k < 3; ++k) {
                unsigned vertex = vertices[k];
                order[written] = indices[input+vertex];
                if (uv) {
                    const unsigned char *source_uv = (const unsigned char *)mesh.vertuv;
                    memcpy(&uv[written], source_uv + (corner+vertex)*sizeof *uv, sizeof *uv);
                }
                ++written;
            }
        }
        corner += n;
        input += n;
    }
    mesh.type_matId = NJD_MESHSET_3;
    mesh.nbMesh = (Uint16)triangles;
    mesh.meshes = order;
    mesh.vertuv = uv;
    material.attrflags |= NJD_FLAG_IGNORE_LIGHT;
    NJS_MODEL draw = *model;
    draw.normals = environment ? model->normals : NULL;
    draw.meshsets = &mesh;
    draw.mats = &material;
    draw.nbMeshset = draw.nbMat = 1;
    draw.center = (NJS_POINT3){0};
    draw.r = 1024;
    struct simple_mesh_span converted = {written, uv ? written : 0};
    int32_t projection;
    int result = jp_simple_draw_model_bounded(&draw, draw.nbPoint,
        draw.normals ? draw.nbPoint : 0, 1, &converted, 1, &projection);
    free(uv);
    free(order);
    return result;
}

__declspec(dllexport) int jp_emerald_stage_mesh_submit(NJS_MODEL *model)
{
    const struct simple_mesh_span *spans;
    int count = jp_emerald_stage_mesh_bounds(model, &spans);
    if (count < 0) return -1;
    for (int i = 0; i < count; ++i) {
        int result = submit_stage_mesh(model, (unsigned)i, spans+i);
        if (result) return result;
    }
    return 0;
}
