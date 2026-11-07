#include <stdlib.h>
#include "collision/world.h"

int jp_land_task_open(struct land_entry *, int, NJS_TEXLIST *, int, Float,
                      void (*)(NJS_MODEL *));
int jp_land_task_close(void);

static struct land_entry *stage_land_entries;
static NJS_OBJECT *stage_land_objects;

static int close_stage_land(void)
{
    if (!stage_land_entries) return 0;
    if (jp_land_task_close()) return -1;
    free(stage_land_objects);
    free(stage_land_entries);
    stage_land_objects = NULL;
    stage_land_entries = NULL;
    return 0;
}

static const unsigned char *stage_module_at(const struct resource_view *image,
                                            uint32_t address, size_t bytes)
{
    if (address < STAGE_MODULE_BASE) return NULL;
    size_t offset = address - STAGE_MODULE_BASE;
    if (offset > image->bytes || bytes > image->bytes - offset) return NULL;
    return (const unsigned char *)image->data + offset;
}

/* The stage owns relocated entry/object records and the scheduler task.
 * The renderer retains the model data and texture list through stage close. */
__declspec(dllexport) int jp_emerald_stage_land_open(NJS_TEXLIST *textures,
    NJS_MODEL *(*resolve)(unsigned, uint32_t), void (*consume)(NJS_MODEL *))
{
    struct resource_view image;
    if (!(stage_session & STAGE_FLOOR) || emerald_stage_stepping ||
        stage_land_entries || !resolve || !consume ||
        jp_stage_resources_get(0, &image)) return -1;
    const unsigned char *table = stage_module_at(&image, land_headers[emerald_camera_act], 16);
    if (!table) return -1;
    int16_t header[4];
    Float clip;
    uint32_t address;
    memcpy(header, table, sizeof header);
    memcpy(&clip, table+8, sizeof clip);
    memcpy(&address, table+12, sizeof address);
    int count = header[0], flags = header[2];
    if (count <= 0 || count > 1024 || header[1] || (flags & (LAND_MOTIONS | LAND_FILE)))
        return -1;
    const unsigned char *rows = stage_module_at(&image, address, (size_t)count*36);
    if (!rows) return -1;
    struct land_entry *entries = calloc(count, sizeof *entries);
    NJS_OBJECT *objects = calloc(count, sizeof *objects);
    int result = -1;
    emerald_stage_stepping = 1;
    if (!entries || !objects) goto finish;
    for (int i = 0; i < count; ++i) {
        const unsigned char *row = rows + i*36;
        struct land_entry *entry = &entries[i];
        memcpy(&entry->center, row, 12);
        memcpy(&entry->radius, row+12, 12);
        memcpy(&address, row+24, 4);
        memcpy(&entry->blocks, row+28, 4);
        memcpy(&entry->flags, row+32, 4);
        if (!(entry->flags & LAND_DRAWN)) continue;
        const unsigned char *original = stage_module_at(&image, address, 52);
        if (!original) goto finish;
        NJS_OBJECT *object = &objects[i];
        uint32_t model;
        memcpy(&model, original+4, 4);
        if (!stage_module_at(&image, model, 40)) goto finish;
        object->model = resolve((unsigned)i, model);
        if (!object->model) goto finish;
        memcpy(object->pos, original+8, 12);
        memcpy(object->ang, original+20, 12);
        memcpy(object->scl, original+32, 12);
        entry->object = object;
    }
    result = jp_land_task_open(entries, count, textures, flags, clip, consume);
finish:
    emerald_stage_stepping = 0;
    if (result) {
        free(objects);
        free(entries);
    } else {
        stage_land_objects = objects;
        stage_land_entries = entries;
    }
    return result;
}
