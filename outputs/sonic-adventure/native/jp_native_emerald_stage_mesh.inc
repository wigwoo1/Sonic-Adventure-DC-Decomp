/* Native headers point into the owned, decoded stage image. Only the
 * pointer-width-dependent headers and bounds need new allocations. */
struct stage_mesh {
    NJS_MODEL model;
    struct simple_mesh_span *spans;
    uint32_t address;
};

static struct stage_mesh stage_meshes[1024];
static unsigned stage_mesh_count;

static void clear_stage_meshes(void)
{
    while (stage_mesh_count) {
        struct stage_mesh *mesh = &stage_meshes[--stage_mesh_count];
        free(mesh->model.meshsets);
        free(mesh->spans);
        *mesh = (struct stage_mesh){0};
    }
}

static const unsigned char *stage_array(const struct resource_view *image,
    uint32_t address, size_t bytes, unsigned alignment)
{
    if (address & (alignment-1)) return NULL;
    return stage_module_at(image, address, bytes);
}

static int stage_mesh_read(const struct resource_view *image, NJS_MESHSET *mesh,
    struct simple_mesh_span *span, const unsigned char *row, unsigned points, unsigned materials)
{
    uint16_t header[2];
    uint32_t pointers[5];
    memcpy(header, row, sizeof header);
    memcpy(pointers, row+4, sizeof pointers);
    unsigned kind = header[0] & NJD_MESHSET_MASK;
    if ((header[0] & 0x3fff) >= materials || !header[1] ||
        pointers[1] || pointers[2] || pointers[3] || kind == NJD_MESHSET_N)
        return -1;
    size_t words = 0, corners = 0;
    const uint16_t *indices = (const uint16_t *)stage_array(image, pointers[0], 2, 2);
    if (!indices) return -1;
    size_t limit = (image->bytes - (pointers[0]-STAGE_MODULE_BASE)) / 2;
    for (unsigned i = 0; i < header[1]; ++i) {
        unsigned count = kind == NJD_MESHSET_3 ? 3 : 4;
        if (kind == NJD_MESHSET_TRIMESH) {
            if (words >= limit) return -1;
            count = indices[words++] & 0x7fff;
        }
        if (count < 3 || count > limit-words) return -1;
        for (unsigned j = 0; j < count; ++j)
            if (indices[words+j] >= points) return -1;
        words += count;
        corners += count;
    }
    mesh->type_matId = header[0];
    mesh->nbMesh = header[1];
    mesh->meshes = (Sint16 *)indices;
    if (pointers[4]) {
        mesh->vertuv = (NJS_COLOR *)stage_array(image, pointers[4], corners*4, 2);
        if (!mesh->vertuv) return -1;
    }
    *span = (struct simple_mesh_span){words, pointers[4] ? corners : 0};
    return 0;
}

static NJS_MODEL *resolve_stage_mesh(unsigned index, uint32_t address)
{
    for (unsigned i = 0; i < stage_mesh_count; ++i)
        if (stage_meshes[i].address == address) return &stage_meshes[i].model;
    struct resource_view image;
    if (stage_mesh_count == 1024 || jp_stage_resources_get(0, &image)) return NULL;
    const unsigned char *source = stage_module_at(&image, address, 40);
    if (!source) return NULL;
    uint32_t fields[5];
    uint16_t counts[2];
    memcpy(fields, source, sizeof fields);
    memcpy(counts, source+20, sizeof counts);
    if (!fields[2] || fields[2] > 65535 || !counts[0] || !counts[1]) return NULL;
    NJS_MODEL model = {0};
    model.points = (NJS_POINT3 *)stage_array(&image, fields[0], fields[2]*12, 4);
    if (fields[1]) {
        model.normals = (NJS_VECTOR *)stage_array(&image, fields[1], fields[2]*12, 4);
        if (!model.normals) return NULL;
    }
    model.mats = (NJS_MATERIAL *)stage_array(&image, fields[4], counts[1]*20, 4);
    const unsigned char *rows = stage_module_at(&image, fields[3], counts[0]*24);
    if (!model.points || !model.mats || !rows) return NULL;
    model.nbPoint = fields[2];
    model.nbMeshset = counts[0];
    model.nbMat = counts[1];
    memcpy(&model.center, source+24, 12);
    memcpy(&model.r, source+36, 4);
    model.meshsets = calloc(counts[0], sizeof *model.meshsets);
    struct simple_mesh_span *spans = calloc(counts[0], sizeof *spans);
    if (!model.meshsets || !spans) goto failed;
    for (unsigned i = 0; i < counts[0]; ++i)
        if (stage_mesh_read(&image, model.meshsets+i, spans+i, rows+i*24, fields[2], counts[1]))
            goto failed;
    struct stage_mesh *entry = &stage_meshes[stage_mesh_count++];
    *entry = (struct stage_mesh){model, spans, address};
    return &entry->model;
failed:
    free(model.meshsets);
    free(spans);
    return NULL;
}

__declspec(dllexport) int jp_emerald_stage_land_open_native(
    NJS_TEXLIST *textures, void (*consume)(NJS_MODEL *))
{
    if (stage_mesh_count) return -1;
    int result = jp_emerald_stage_land_open(textures, resolve_stage_mesh, consume);
    if (result) clear_stage_meshes();
    return result;
}

__declspec(dllexport) int jp_emerald_stage_mesh_bounds(
    const NJS_MODEL *model, const struct simple_mesh_span **spans)
{
    if (!model || !spans) return -1;
    for (unsigned i = 0; i < stage_mesh_count; ++i) {
        if (model == &stage_meshes[i].model) {
            *spans = stage_meshes[i].spans;
            return model->nbMeshset;
        }
    }
    return -1;
}
