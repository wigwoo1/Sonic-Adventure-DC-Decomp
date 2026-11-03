/* Authored native determinant/inverse ABI; one existing active matrix owner. */
#ifndef JP_NINJA_INVERSE_MATRIX_H
#define JP_NINJA_INVERSE_MATRIX_H
#include "jp_ninja_shared_state.h"
JP_MATH_API float njDetMatrix(jp_matrix *matrix);
/* In-place inverse; null selects active state without saving the stack.
 * Exact zero determinant fills all sixteen words with 3.4e38 and returns 0.
 * All other determinants take the cofactor path and return 1. */
JP_MATH_API int njInvertMatrix(jp_matrix *matrix);
#endif
