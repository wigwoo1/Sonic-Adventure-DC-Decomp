/* Authored native ABI for the Japanese game's matrix/vector dependencies.
 * Runtime pointers are native; no on-disc structures are declared here. */
#ifndef JP_NINJA_MATH_H
#define JP_NINJA_MATH_H
#include <stdint.h>
#if defined(_WIN32)
#define JP_MATH_API __declspec(dllexport)
#else
#define JP_MATH_API __attribute__((visibility("default")))
#endif
typedef float jp_matrix[16];
typedef struct { float x, y, z; } jp_vector;
JP_MATH_API extern jp_matrix _unit_matrix_;
JP_MATH_API extern jp_matrix *_nj_current_matrix_ptr_;
JP_MATH_API extern jp_matrix *_nj_base_matrix_ptr_;
JP_MATH_API extern int32_t _stack_size_, _max_stack_size_, _nj_matrix_flag_;
JP_MATH_API void njInitMatrix(jp_matrix *, int32_t, int32_t);
JP_MATH_API void njGetMatrix(jp_matrix *);
JP_MATH_API void njSetMatrix(jp_matrix *, jp_matrix *);
JP_MATH_API void njUnitMatrix(jp_matrix *);
JP_MATH_API int njPushMatrix(jp_matrix *);
JP_MATH_API int njPopMatrix(uint32_t);
JP_MATH_API void njTranslateV(jp_matrix *, jp_vector *);
JP_MATH_API void njRotateX(jp_matrix *, int32_t);
JP_MATH_API void njRotateY(jp_matrix *, int32_t);
JP_MATH_API void njRotateZ(jp_matrix *, int32_t);
JP_MATH_API void njCalcVector(jp_matrix *, jp_vector *, jp_vector *);
JP_MATH_API void njAddVector(jp_vector *, jp_vector *);
#endif
