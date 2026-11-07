/* Vertical floor query over locally decoded static stage triangles. */
#include <stddef.h>
#include <stdint.h>

struct jp_stage_triangle {
    uint32_t entry, flags;
    float vertex[3][3];
};

struct jp_stage_floor {
    float height;
    uint32_t entry, flags;
};

_Static_assert(sizeof(struct jp_stage_triangle) == 44, "decoded triangle layout");

__declspec(dllexport) int jp_stage_floor_below(
    const struct jp_stage_triangle *triangles, size_t count,
    float x, float y, float z, struct jp_stage_floor *floor)
{
    if (!triangles || !floor)
        return 0;

    struct jp_stage_floor best = {0};
    double best_height = 0;
    int found = 0;
    for (size_t i = 0; i < count; i++) {
        const struct jp_stage_triangle *triangle = &triangles[i];
        const float *a = triangle->vertex[0];
        const float *b = triangle->vertex[1];
        const float *c = triangle->vertex[2];
        double ux = (double)b[0] - a[0];
        double uz = (double)b[2] - a[2];
        double vx = (double)c[0] - a[0];
        double vz = (double)c[2] - a[2];
        double det = ux * vz - uz * vx;
        if (det >= -1e-8)
            continue;
        double dx = (double)x - a[0];
        double dz = (double)z - a[2];
        double u = (dx * vz - dz * vx) / det;
        double v = (ux * dz - uz * dx) / det;
        if (u < -1e-7 || v < -1e-7 || u + v > 1 + 1e-7)
            continue;
        double height = a[1] + u * ((double)b[1] - a[1]) + v * ((double)c[1] - a[1]);
        if (height <= y && (!found || height > best_height ||
                            (height == best_height && triangle->entry > best.entry))) {
            best = (struct jp_stage_floor){(float)height, triangle->entry, triangle->flags};
            best_height = height;
            found = 1;
        }
    }
    if (found)
        *floor = best;
    return found;
}
