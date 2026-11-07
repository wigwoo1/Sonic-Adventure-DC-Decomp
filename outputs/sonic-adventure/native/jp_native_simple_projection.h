/* Bounded native ownership for the original Ninja Simple projection entries. */
#ifndef JP_NATIVE_SIMPLE_PROJECTION_H
#define JP_NATIVE_SIMPLE_PROJECTION_H
#include <stddef.h>
#include <stdint.h>
#pragma push_macro("NULL")
#undef NULL
#include <ninja.h>
#pragma pop_macro("NULL")
struct jp_simple_calc_work {
    Sint32 count;
    Uint8 *out;
    NJS_POINT3 *points;
    NJS_VECTOR *normals;
    Uint32 clip_and,clip_or;
    Float scale_x,scale_y;
    Float clip_left,clip_right,clip_top,clip_bottom;
};
struct jp_simple_projection_queues {uint32_t words[2][8];};
/* The game thread serializes calls. queue words persist across calls.
 * Output is 32-byte aligned. Positive inputs need exactly count points;
 * only loops1/3 need normals. No speculative read of a next point occurs.
 * Returns 0, argument error1, nested-call error2, or internal-call error3.
 * Invalid calls change no output/work/queue bytes. No hardware QACR/cache,
 * FPSCR flags, or undefined prefetch/register side effects are emulated. */
__declspec(dllexport) int jp_simple_project(
    struct jp_simple_calc_work *,size_t,size_t,size_t,
    struct jp_simple_projection_queues *,unsigned);
void jp_simple_projection_loop(struct jp_simple_calc_work *,unsigned);
#endif
