/* Authored test bridge; excluded from production object integration. */
#include "jp_ninja_inverse_matrix.h"
#include <string.h>
__declspec(dllexport) uint32_t jp_inverse_probe_det_bits(jp_matrix *matrix)
{
    float value = njDetMatrix(matrix);
    uint32_t word;
    memcpy(&word, &value, sizeof word);
    return word;
}
