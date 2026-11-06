#include "jp_ninja_scale_vector.h"
void njScaleV(jp_matrix *matrix,const jp_vector *vector)
{
    float x=vector->x,y=vector->y,z=vector->z;
    njScale(matrix,x,y,z);
}
