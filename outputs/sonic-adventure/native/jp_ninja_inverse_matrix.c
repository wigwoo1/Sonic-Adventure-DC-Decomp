/* Authored cofactor implementation from the documented pinned SDK behavior.
 * Original assembly is external. Each scalar operation rounds separately;
 * parenthesization, independent reversed minors and signed zeros are retained.
 */
#include "jp_ninja_inverse_matrix.h"
#include "jp_ninja_scalar32.h"
#include <string.h>
#define ADD jp_scalar32_add
#define SUB jp_scalar32_subtract
#define MUL jp_scalar32_multiply
static uint32_t determinant(const uint32_t m[16])
{
    uint32_t a = SUB(MUL(m[10], m[15]), MUL(m[11], m[14]));
    uint32_t b = SUB(MUL(m[9], m[14]), MUL(m[10], m[13]));
    uint32_t c = SUB(MUL(m[11], m[12]), MUL(m[8], m[15]));
    uint32_t d = SUB(MUL(m[8], m[13]), MUL(m[9], m[12]));
    uint32_t e = SUB(MUL(m[13], m[11]), MUL(m[15], m[9]));
    uint32_t f = SUB(MUL(m[14], m[8]), MUL(m[12], m[10]));
    uint32_t reverse_e = SUB(MUL(m[15], m[9]), MUL(m[13], m[11]));
    uint32_t reverse_f = SUB(MUL(m[12], m[10]), MUL(m[14], m[8]));
    uint32_t p = ADD(ADD(MUL(e, m[6]), MUL(a, m[5])), MUL(b, m[7]));
    uint32_t q = ADD(ADD(MUL(m[4], a), MUL(c, m[6])), MUL(f, m[7]));
    uint32_t s = ADD(MUL(m[7], d), ADD(MUL(reverse_e, m[4]), MUL(m[5], c)));
    uint32_t t = ADD(ADD(MUL(m[5], reverse_f), MUL(m[4], b)), MUL(m[6], d));
    return ADD(SUB(MUL(s, m[2]), MUL(t, m[3])), SUB(MUL(p, m[0]), MUL(q, m[1])));
}
float njDetMatrix(jp_matrix *matrix)
{
    uint32_t words[16];
    memcpy(words, matrix ? (const void *)matrix : jp_ninja_current_matrix_storage(), sizeof words);
    uint32_t word = determinant(words);
    float value;
    memcpy(&value, &word, sizeof value);
    return value;
}

/* For each adjugate entry: six rounded triple products, with the first pair
 * multiplied before the remaining factor. Signs follow the cofactor pattern.
 * Two entries associate their first three terms to the right; diagonal entry
 * five adds its final negated term. These preserve the SDK's evaluation order.
 */
static const uint8_t terms[16][6][3] = {
    {{10,15,5},{13,11,6},{9,14,7},{10,13,7},{9,15,6},{11,14,5}},
    {{9,14,3},{13,11,2},{10,15,1},{10,13,3},{9,15,2},{11,14,1}},
    {{1,6,15},{7,2,13},{5,3,14},{3,6,13},{2,5,15},{1,7,14}},
    {{1,6,11},{7,2,9},{5,3,10},{3,6,9},{2,5,11},{1,7,10}},
    {{10,15,4},{11,12,6},{14,8,7},{10,12,7},{8,15,6},{11,14,4}},
    {{10,15,0},{11,12,2},{14,8,3},{10,12,3},{8,15,2},{11,14,0}},
    {{7,2,12},{6,0,15},{3,4,14},{3,6,12},{2,4,15},{0,7,14}},
    {{7,2,8},{6,0,11},{3,4,10},{3,6,8},{2,4,11},{0,7,10}},
    {{9,15,4},{11,12,5},{8,13,7},{9,12,7},{15,8,5},{13,11,4}},
    {{9,15,0},{11,12,1},{8,13,3},{9,12,3},{15,8,1},{13,11,0}},
    {{1,7,12},{0,5,15},{3,4,13},{3,5,12},{4,1,15},{0,7,13}},
    {{1,7,8},{0,5,11},{3,4,9},{3,5,8},{4,1,11},{0,7,9}},
    {{12,10,5},{9,14,4},{8,13,6},{9,12,6},{8,14,5},{10,13,4}},
    {{8,13,2},{9,14,0},{12,10,1},{9,12,2},{8,14,1},{10,13,0}},
    {{1,6,12},{0,5,14},{4,2,13},{2,5,12},{4,1,14},{0,6,13}},
    {{0,5,10},{1,6,8},{4,2,9},{2,5,8},{4,1,10},{0,6,9}}
};
int njInvertMatrix(jp_matrix *matrix)
{
    void *target = matrix ? (void *)matrix : jp_ninja_current_matrix_storage();
    uint32_t m[16], out[16];
    memcpy(m, target, sizeof m);
    uint32_t det = determinant(m);
    if ((det & 0x7fffffffu) == 0) {
        for (int i = 0; i < 16; ++i) out[i] = 0x7f7fc99eu;
        memcpy(target, out, sizeof out);
        return 0;
    }
    uint32_t reciprocal = jp_scalar32_divide(0x3f800000u, det);
    for (int i = 0; i < 16; ++i) {
        uint32_t t[6];
        for (int j = 0; j < 6; ++j) {
            const uint8_t *index = terms[i][j];
            t[j] = MUL(MUL(m[index[0]], m[index[1]]), m[index[2]]);
        }
        uint32_t value = (i == 1 || i == 13) ? ADD(t[0], ADD(t[1], t[2])) : ADD(ADD(t[0], t[1]), t[2]);
        value = SUB(SUB(value, t[3]), t[4]);
        value = i == 5 ? ADD(t[5] ^ 0x80000000u, value) : SUB(value, t[5]);
        if (((i / 4 + i % 4) & 1) != 0) value ^= 0x80000000u;
        out[i] = MUL(value, reciprocal);
    }
    memcpy(target, out, sizeof out);
    return 1;
}
