/* Authored native host interface; implementation comes from external SH4ZAM. */
#include <sh4zam/shz_xmtrx.h>
#include <sh4zam/shz_matrix.h>
#define EXPORT __declspec(dllexport)
_Static_assert(sizeof(shz_mat4x4_t) == 64, "matrix data ABI");
_Static_assert(sizeof(shz_vec4_t) == 16, "vector data ABI");
EXPORT void jp_shz_load(const float *matrix)
{
    shz_xmtrx_load_unaligned_4x4(matrix);
}
EXPORT void jp_shz_store(float *matrix)
{
    shz_xmtrx_store_unaligned_4x4(matrix);
}
EXPORT void jp_shz_transform(const float *input, float *output)
{
    shz_vec4_t v = shz_vec4_init(input[0], input[1], input[2], input[3]);
    shz_vec4_t r = shz_xmtrx_transform_vec4(v);
    output[0] = r.x; output[1] = r.y; output[2] = r.z; output[3] = r.w;
}
EXPORT unsigned jp_shz_matrix_alignment(void) { return _Alignof(shz_mat4x4_t); }
EXPORT unsigned jp_shz_tls_model(void) { return SHZ_TLS_MODEL; }
