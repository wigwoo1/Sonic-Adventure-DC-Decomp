/* Authored native implementation from the pinned Japanese SDK contract.
 * Original SH-4 assembly remains an external reference. The numerical model
 * retains its existing third-party MIT notice. One game thread owns state. */
#include "jp_ninja_multi_matrix.h"
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"

void njMultiMatrix(jp_matrix *destination, jp_matrix *source)
{
    float *target = destination ? *destination : jp_ninja_current_matrix_storage();
    uint32_t left[16], right[16], product[16];
    memcpy(left, source, sizeof left);
    memcpy(right, target, sizeof right);

    if (!destination) {
        /* Original implicit-current path: transform each source row by XMTRX.
         * FTRV recodes the destination matrix operand in the multiplier model. */
        for (int row = 0; row < 4; ++row) {
            uint32_t v[4];
            memcpy(v, left + 4 * row, sizeof v);
            sh4_ftrv(right, v, 0);
            memcpy(product + 4 * row, v, sizeof v);
        }
    } else {
        /* Original stored path: FIPR FV8,FV0 dots a destination column into
         * a source row. Reversing those operands changes low result bits. */
        for (int column = 0; column < 4; ++column) {
            uint32_t v[4];
            for (int i = 0; i < 4; ++i) v[i] = right[4 * i + column];
            for (int row = 0; row < 4; ++row)
                product[4 * row + column] = sh4_fipr(v, left + 4 * row, 0);
        }
    }
    memcpy(target, product, sizeof product);
}
