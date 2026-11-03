/* Authored state adapter. SH4ZAM and Ninja use the same active matrix.
 * Do not link the dependency's separate shz_xmtrx_sw.c state implementation
 * into this runtime. Its other out-of-line helpers need separate integration. */
#include "jp_ninja_shared_state.h"
#include "jp_sh4zam_host_config.h"
#include <sh4zam/shz_xmtrx.h>
_Static_assert(sizeof(shz_xmtrx__t) == sizeof(jp_matrix), "matrix storage size");
_Static_assert(_Alignof(shz_xmtrx__t) <= 64, "matrix storage alignment");
_Static_assert(__builtin_offsetof(shz_xmtrx__t, elem) == 0, "matrix word layout");
shz_xmtrx__t *shz_xmtrx_state_(void)
{
    return (shz_xmtrx__t *)(void *)jp_ninja_current_matrix_storage();
}
