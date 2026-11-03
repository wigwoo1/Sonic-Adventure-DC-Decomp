/* Authored extension to the shared native Ninja matrix ABI. */
#ifndef JP_NINJA_MULTI_MATRIX_H
#define JP_NINJA_MULTI_MATRIX_H
#include "jp_ninja_shared_state.h"
/* destination = source * destination, for row-vector matrices.
 * A null destination selects the single active matrix, without saving it to
 * the deferred stack. Source must address sixteen readable float words.
 * Inputs may overlap; the complete product precedes the first output store.
 * Numerical policy: nearest rounding, DN=1, exceptions disabled. */
JP_MATH_API void njMultiMatrix(jp_matrix *destination, jp_matrix *source);
#endif
