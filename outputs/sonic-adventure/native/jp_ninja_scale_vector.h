#ifndef JP_NINJA_SCALE_VECTOR_H
#define JP_NINJA_SCALE_VECTOR_H
#include "jp_ninja_view_stack.h"
/* Same current-matrix owner and toward-zero/DN1 arithmetic as njScale.
 * v addresses three readable floats and may alias the destination matrix.
 * A null matrix selects the active matrix. v itself must be nonnull. */
JP_MATH_API void njScaleV(jp_matrix *,const jp_vector *);
#endif
