/* Authored host implementations from the documented Japanese SDK behavior.
 * SH-4 assembly is an external reference, not copied into this source.
 * Numerical instruction models retain their existing third-party MIT notice.
 * Policy: nearest rounding, denormals flushed, exceptions disabled. This is
 * value compatibility, not a host emulation of the SH-4 FPSCR or register ABI.
 * The current matrix is intentionally separate from its deferred stack copy.
 * One game thread owns this global state, as in the original SDK.
 */
#include "jp_ninja_math.h"
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"

_Static_assert(sizeof(float) == 4 && sizeof(jp_matrix) == 64, "matrix ABI");
_Static_assert(sizeof(jp_vector) == 12, "vector ABI");
jp_matrix _unit_matrix_ = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
jp_matrix *_nj_current_matrix_ptr_, *_nj_base_matrix_ptr_;
int32_t _stack_size_, _max_stack_size_, _nj_matrix_flag_;
static jp_matrix current;

static uint32_t bits(float f) { uint32_t u; memcpy(&u, &f, 4); return u; }
static float number(uint32_t u) { float f; memcpy(&f, &u, 4); return f; }
static void copy(jp_matrix *to, const jp_matrix *from)
{
    /* Temporary also permits exact self-aliasing. */
    jp_matrix t;
    for (int i = 0; i < 16; ++i) t[i] = (*from)[i];
    for (int i = 0; i < 16; ++i) (*to)[i] = t[i];
}
static void transform(const jp_matrix *m, const float in[4], float out[4])
{
    uint32_t matrix[16], vector[4];
    for (int i = 0; i < 16; ++i) matrix[i] = bits((*m)[i]);
    for (int i = 0; i < 4; ++i) vector[i] = bits(in[i]);
    sh4_ftrv(matrix, vector, 0);
    for (int i = 0; i < 4; ++i) out[i] = number(vector[i]);
}
void njInitMatrix(jp_matrix *stack, int32_t capacity, int32_t normalize)
{
    _nj_base_matrix_ptr_ = _nj_current_matrix_ptr_ = stack;
    _max_stack_size_ = capacity;
    _stack_size_ = 1;
    _nj_matrix_flag_ = normalize;
    copy(&current, &_unit_matrix_);
    current[0] = -current[0]; current[5] = -current[5];
    copy(stack, &current);
}
void njGetMatrix(jp_matrix *out) { copy(out, &current); }
void njSetMatrix(jp_matrix *out, jp_matrix *in)
{
    copy(out ? out : &current, in);
}
void njUnitMatrix(jp_matrix *out)
{
    copy(out ? out : &current, &_unit_matrix_);
}
int njPushMatrix(jp_matrix *replacement)
{
    if (_stack_size_ >= _max_stack_size_) return 0;
    copy(_nj_current_matrix_ptr_, &current);
    ++_nj_current_matrix_ptr_; ++_stack_size_;
    if (replacement) copy(&current, replacement);
    else copy(_nj_current_matrix_ptr_, &current);
    return 1;
}
int njPopMatrix(uint32_t count)
{
    /* Valid original calls keep at least one matrix. Reject the original's
     * unsigned-underflow path rather than form a pointer outside the arena. */
    if (count >= (uint32_t)_stack_size_) return 0;
    _stack_size_ -= (int32_t)count;
    _nj_current_matrix_ptr_ -= count;
    copy(&current, _nj_current_matrix_ptr_);
    return 1;
}
void njTranslateV(jp_matrix *m, jp_vector *v)
{
    jp_matrix *target = m ? m : &current;
    float input[4] = {v->x, v->y, v->z, 1.0f}, out[4];
    transform(target, input, out);
    for (int i = 0; i < 4; ++i) (*target)[12 + i] = out[i];
}
static void rotate(jp_matrix *m, int32_t angle, int axis)
{
    jp_matrix *target = m ? m : &current;
    uint32_t sb, cb;
    sh4_fsca((uint32_t)angle, 0, &sb, &cb);
    float s = number(sb), c = number(cb);
    float a[4] = {0,0,0,0}, b[4] = {0,0,0,0}, x[4], y[4];
    int row_a, row_b;
    if (axis == 0) { row_a = 1; row_b = 2; a[1] = c; a[2] = s; b[1] = -s; b[2] = c; }
    else if (axis == 1) { row_a = 0; row_b = 2; a[0] = c; a[2] = -s; b[0] = s; b[2] = c; }
    else { row_a = 0; row_b = 1; a[0] = c; a[1] = s; b[0] = -s; b[1] = c; }
    transform(target, a, x); transform(target, b, y);
    for (int i = 0; i < 4; ++i) { (*target)[4*row_a+i] = x[i]; (*target)[4*row_b+i] = y[i]; }
}
void njRotateX(jp_matrix *m, int32_t angle) { rotate(m, angle, 0); }
void njRotateY(jp_matrix *m, int32_t angle) { rotate(m, angle, 1); }
void njRotateZ(jp_matrix *m, int32_t angle) { rotate(m, angle, 2); }
void njCalcVector(jp_matrix *m, jp_vector *in, jp_vector *out)
{
    float input[4] = {in->x, in->y, in->z, 0}, result[4];
    transform(m ? m : &current, input, result);
    if (_nj_matrix_flag_) {
        uint32_t v[4] = {bits(result[0]), bits(result[1]), bits(result[2]), 0};
        float reciprocal = number(sh4_fsrra(sh4_fipr(v, v, 0), 0));
        for (int i = 0; i < 3; ++i) result[i] *= reciprocal;
    }
    out->x = result[0]; out->y = result[1]; out->z = result[2];
}
void njAddVector(jp_vector *out, jp_vector *in)
{
    float x = out->x + in->x, y = out->y + in->y, z = out->z + in->z;
    out->x = x; out->y = y; out->z = z;
}
