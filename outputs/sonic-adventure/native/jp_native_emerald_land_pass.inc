#include <float.h>
#include <math.h>
#include "system/util.h"

int jp_land_task_frame(const NJS_POINT3 *, Angle, Angle, Sint32, Bool, Sint32);

/* Use the same active rigid world-to-view matrix for visibility and model
 * submission. The caller owns the camera matrix and the open terrain task. */
__declspec(dllexport) int jp_emerald_draw_land(Sint32 blocks, Bool shown, Sint32 step)
{
    NJS_MATRIX matrix;
    njGetMatrix(&matrix);
    for (unsigned i = 0; i < 16; ++i)
        if (!(matrix[i] >= -FLT_MAX && matrix[i] <= FLT_MAX)) return -1;

    NJS_POINT3 eye;
    eye.x = -(matrix[0]*(double)matrix[12] + matrix[1]*(double)matrix[13] + matrix[2]*(double)matrix[14]);
    eye.y = -(matrix[4]*(double)matrix[12] + matrix[5]*(double)matrix[13] + matrix[6]*(double)matrix[14]);
    eye.z = -(matrix[8]*(double)matrix[12] + matrix[9]*(double)matrix[13] + matrix[10]*(double)matrix[14]);
    double x = matrix[2], y = matrix[6], z = matrix[10];
    double horizontal = sqrt(x*x + z*z);
    if (!(horizontal > 0) && y == 0) return -1;
    Angle yaw = (Angle)(atan2(-x, -z) * ANGLE_PER_RADIAN);
    Angle pitch = (Angle)(atan2(y, horizontal) * ANGLE_PER_RADIAN);
    return jp_land_task_frame(&eye, yaw, pitch, blocks, shown, step);
}
