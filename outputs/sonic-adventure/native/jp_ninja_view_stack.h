/* Authored native view data ABI and real matrix-stack providers.
 * A valid current view and initialized nonempty stack are required by clear.
 * njSetView and the view-rotation functions require separate implementations. */
#ifndef JP_NINJA_VIEW_STACK_H
#define JP_NINJA_VIEW_STACK_H
#include "jp_ninja_shared_state.h"
typedef struct {
    jp_matrix m;
    float px,py,pz;
    float vx,vy,vz;
    int32_t roll;
    float apx,apy,apz;
    float avx,avy,avz;
    int32_t aroll;
} jp_view;
JP_MATH_API extern jp_view *_nj_view_;
JP_MATH_API extern jp_matrix _nj_view_matrix_;
JP_MATH_API void njInitView(jp_view *);
JP_MATH_API void njClearMatrix(void);
JP_MATH_API void njScale(jp_matrix *,float,float,float);
#endif
