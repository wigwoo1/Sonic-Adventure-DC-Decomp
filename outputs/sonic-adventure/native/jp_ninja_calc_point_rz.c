/* Complete original current/stored point contract, including stored-output
 * alias ordering: X is stored after loading column Y, before loading Z. */
#include <ninja.h>
#include <stdint.h>
#include <string.h>
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"
extern float *jp_ninja_current_matrix_storage(void);
static void column(uint32_t c[4],const float *m,int i)
{for(int k=0;k<4;k++)memcpy(c+k,m+i+4*k,4);}
void njCalcPoint(NJS_MATRIX *m,NJS_POINT3 *from,NJS_POINT3 *to)
{
    uint32_t v[4],c[4],x,y,z;memcpy(v,from,12);v[3]=0x3f800000u;
    if(!m){uint32_t a[16];memcpy(a,jp_ninja_current_matrix_storage(),64);sh4_ftrv(a,v,1);memcpy(to,v,12);return;}
    const float *a=(const float *)m;
    column(c,a,0);x=sh4_fipr(v,c,1);
    column(c,a,1);y=sh4_fipr(v,c,1);
    memcpy(&to->x,&x,4);
    column(c,a,2);memcpy(&to->y,&y,4);z=sh4_fipr(v,c,1);memcpy(&to->z,&z,4);
}
