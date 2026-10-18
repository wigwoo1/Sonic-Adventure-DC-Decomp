/* Reuse the byte-proven game body with the extended scalar support call.
 * No game instruction/order changes. Prior body/source and DLLs are preserved.
 */
#define sa_sh4_multiply sa_sh4_multiply_extended
#define sa_model_direction_transform sa_model_direction_transform_extended
#include "model_direction_transform.c"
