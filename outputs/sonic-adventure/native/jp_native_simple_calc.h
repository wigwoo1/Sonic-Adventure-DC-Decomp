#ifndef JP_NATIVE_SIMPLE_CALC_H
#define JP_NATIVE_SIMPLE_CALC_H
#include "jp_native_simple_projection.h"
/* One game-thread owner for32-byte Simple records, distinct from game scratch. */
__declspec(dllexport) void SimpleCalcModelMatrix(NJS_POINT3 *,NJS_POINT3 *);
__declspec(dllexport) void SimpleCalcModelLight(NJS_VECTOR *,NJS_VECTOR *,Float);
__declspec(dllexport) int jp_simple_calc_initialize(uint32_t,size_t);
__declspec(dllexport) int jp_simple_calc_shutdown(void);
__declspec(dllexport) int jp_simple_calc_guards(void);
__declspec(dllexport) void *jp_simple_calc_records(void);
__declspec(dllexport) struct jp_simple_projection_queues *jp_simple_calc_queues(void);
__declspec(dllexport) int jp_simple_calc_model(NJS_MODEL *,size_t,size_t,NJS_VECTOR *,Float,int32_t *);
/* Internal connection to the genuine full projection body. */
void jp_simple_calc_loop(struct jp_simple_calc_work *,NJS_MATRIX *,unsigned);
void jp_simple_calc_context_light(NJS_VECTOR *,Float);
__declspec(dllexport) void jp_simple_calc_context_snapshot(uint32_t *);
#endif
