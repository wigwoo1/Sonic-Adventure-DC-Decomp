#include "jp_native_primitive_numeric.h"
#include "jp_ninja_camera_math.h"
#include <string.h>
float jp_primitive_sqrt(float x)
{
    uint32_t u;memcpy(&u,&x,4);u=jp_rz_camera_sqrt(u);memcpy(&x,&u,4);return x;
}
