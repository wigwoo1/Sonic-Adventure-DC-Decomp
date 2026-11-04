/* Authored from the pinned SDK's documented behavior. Game source stays external.
 * Uses the existing one active matrix and deferred stack, never a second owner.
 * Fixed toward-zero, DN=1, disabled-exception values; no host rounding switches. */
#include "jp_ninja_view_stack.h"
#include "jp_ninja_multi_matrix.h"
#include "jp_ninja_scalar32_rz.h"
#include <stddef.h>
#include <string.h>
_Static_assert(sizeof(jp_view)==120,"view data ABI");
_Static_assert(offsetof(jp_view,px)==64 && offsetof(jp_view,vx)==76,"view position/direction ABI");
_Static_assert(offsetof(jp_view,roll)==88 && offsetof(jp_view,avx)==104 && offsetof(jp_view,aroll)==116,"view angle/absolute ABI");
jp_view *_nj_view_;
_Alignas(32) jp_matrix _nj_view_matrix_;

void njInitView(jp_view *view)
{
    memset(view,0,sizeof *view);
    memcpy(view->m,_unit_matrix_,sizeof view->m);
    view->vz=-1.0f;
    view->avz=-1.0f;
}

void njScale(jp_matrix *matrix,float sx,float sy,float sz)
{
    float *target=matrix ? *matrix : jp_ninja_current_matrix_storage();
    uint32_t input[16],scale[3],result[12];
    memcpy(input,target,sizeof input);
    memcpy(&scale[0],&sx,4);memcpy(&scale[1],&sy,4);memcpy(&scale[2],&sz,4);
    for (int row=0;row<3;++row)
        for (int column=0;column<4;++column)
            result[4*row+column]=jp_rz_scalar32_multiply(input[4*row+column],scale[row]);
    memcpy(target,result,sizeof result);
}

void njClearMatrix(void)
{
    jp_matrix *base=_nj_base_matrix_ptr_;
    /* The original first loads the old base into XMTRX, then rewrites base. */
    njSetMatrix(NULL,base);
    _stack_size_=1;
    _nj_current_matrix_ptr_=base;
    njUnitMatrix(base);
    njScale(base,-1.0f,-1.0f,1.0f);
    /* Stored FIPR multiplication matters, even though the base is reflected. */
    njMultiMatrix(base,&_nj_view_->m);
    memcpy(_nj_view_matrix_,*base,sizeof _nj_view_matrix_);
    njSetMatrix(NULL,base);
}
