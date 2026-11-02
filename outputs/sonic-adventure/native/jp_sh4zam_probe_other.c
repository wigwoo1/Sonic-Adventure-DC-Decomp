/* A separate translation unit tests shared portable XMTRX state. */
#include <sh4zam/shz_xmtrx.h>
__declspec(dllexport) void jp_shz_store_from_other_unit(float *matrix)
{
    shz_xmtrx_store_unaligned_4x4(matrix);
}
