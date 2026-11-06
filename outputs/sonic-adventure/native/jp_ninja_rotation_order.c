#include "jp_ninja_rotation_order.h"
void njRotateXYZ(jp_matrix *matrix,int32_t x,int32_t y,int32_t z)
{
    if(z)njRotateZ(matrix,z);
    if(y)njRotateY(matrix,y);
    if(x)njRotateX(matrix,x);
}
void njRotateZXY(jp_matrix *matrix,int32_t x,int32_t y,int32_t z)
{
    if(y)njRotateY(matrix,y);
    if(x)njRotateX(matrix,x);
    if(z)njRotateZ(matrix,z);
}
