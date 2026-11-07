#include "camera/camera.h"
#include "player/player.h"

_Static_assert(sizeof(struct camera_collision) == 64, "camera file record layout");

static int emerald_camera_act = -1;
static uint16_t emerald_camera_hits[CAMERA_COLLISIONS_MAX];
static unsigned emerald_camera_count;

/* Invert the rotation of the authored volume using its orthonormal axes. */
static NJS_POINT3 camera_local(const struct camera_collision *row, const NJS_POINT3 *p)
{
    NJS_MATRIX matrix = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    njRotateX(&matrix, row->col_ang_x);
    njRotateY(&matrix, row->col_ang_y);
    Float x = p->x - row->col_pos.x;
    Float y = p->y + TEST_HEIGHT - row->col_pos.y;
    Float z = p->z - row->col_pos.z;
    return (NJS_POINT3){
        x*matrix[0] + y*matrix[1] + z*matrix[2],
        x*matrix[4] + y*matrix[5] + z*matrix[6],
        x*matrix[8] + y*matrix[9] + z*matrix[10]
    };
}

static int camera_contact(const struct camera_collision *row, const NJS_POINT3 *now,
                          const NJS_POINT3 *previous, const NJS_POINT3 *older)
{
    Float x = row->col_pos.x - now->x;
    Float y = row->col_pos.y - now->y;
    Float z = row->col_pos.z - now->z;
    const NJS_POINT3 size = row->col_scl;
    if (row->type == CAMCOL_SPHERE) {
        y -= TEST_HEIGHT;
        return size.z > x*x + y*y + z*z;
    }
    if (row->type != CAMCOL_BLOCK && row->type != CAMCOL_PLANE) return 0;
    y += TEST_HEIGHT; /* The original coarse check adds height on this side. */
    Float range = size.x*size.x + size.y*size.y;
    if (row->type == CAMCOL_BLOCK) range += size.z*size.z;
    if (x*x + y*y + z*z > range) return 0;
    NJS_POINT3 point = camera_local(row, previous);
    if (row->type == CAMCOL_BLOCK)
        return point.x >= -size.x && point.x <= size.x &&
               point.y >= -size.y && point.y <= size.y &&
               point.z >= -size.z && point.z <= size.z;
    NJS_POINT3 before = camera_local(row, older);
    if ((point.z < 0 && before.z < 0) || (point.z > 0 && before.z > 0) ||
        point.z == before.z) return 0;
    Float t = -before.z / (point.z - before.z);
    x = before.x + t*(point.x-before.x);
    y = before.y + t*(point.y-before.y);
    return x >= -size.x && x <= size.x && y >= -size.y && y <= size.y;
}

/* Return contacts in authored order. Mode levels and transition priority
 * belong to the camera controller, which consumes this ordered stream. */
__declspec(dllexport) int jp_emerald_camera_contacts(unsigned act, const NJS_POINT3 *now,
    const NJS_POINT3 *previous, const NJS_POINT3 *older, uint16_t *out, size_t capacity)
{
    struct resource_view view;
    if (act >= 3 || !now || !previous || !older || (!out && capacity) ||
        jp_stage_resources_get(4 + act, &view) || view.bytes < 64) return -1;
    uint32_t count;
    memcpy(&count, view.data, sizeof count);
    if (count > CAMERA_COLLISIONS_MAX || count > view.bytes/64 - 1) return -1;
    const unsigned char *bytes = view.data;
    unsigned hits = 0;
    for (unsigned i = 0; i < count; ++i) {
        struct camera_collision row;
        memcpy(&row, bytes + (i+1)*64, sizeof row);
        if (camera_contact(&row, now, previous, older)) {
            if (hits < capacity) out[hits] = (uint16_t)i;
            ++hits;
        }
    }
    return (int)hits;
}

static void emerald_camera_sample(const NJS_POINT3 *now)
{
    NJS_POINT3 previous = *now, older = *now;
    player_get_history(0, 1, &previous, NULL);
    player_get_history(0, 2, &older, NULL);
    int count = jp_emerald_camera_contacts(emerald_camera_act, now, &previous, &older,
        emerald_camera_hits, CAMERA_COLLISIONS_MAX);
    emerald_camera_count = count < 0 ? 0 : (unsigned)count;
}

__declspec(dllexport) int jp_emerald_camera_last(uint16_t *out, size_t capacity)
{
    if (emerald_camera_act < 0 || (!out && capacity)) return -1;
    size_t count = capacity < emerald_camera_count ? capacity : emerald_camera_count;
    if (count) memcpy(out, emerald_camera_hits, count * sizeof *out);
    return (int)emerald_camera_count;
}
