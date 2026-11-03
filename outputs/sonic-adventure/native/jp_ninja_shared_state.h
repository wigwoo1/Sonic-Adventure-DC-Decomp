/* Authored shared active-matrix ABI. One instance belongs to one game runtime.
 * The game thread owns this storage; no concurrent matrix API calls are valid. */
#ifndef JP_NINJA_SHARED_STATE_H
#define JP_NINJA_SHARED_STATE_H
#include "jp_ninja_math.h"
/* Sixteen floats, explicitly aligned to 64 bytes. Stack snapshots stay separate. */
JP_MATH_API float *jp_ninja_current_matrix_storage(void);
#endif
