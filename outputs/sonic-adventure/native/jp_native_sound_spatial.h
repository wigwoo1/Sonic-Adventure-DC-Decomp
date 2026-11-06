/* Authored value-compatible spatial dependencies for the original game.
 * Fixed toward-zero/DN1 policy, exceptions disabled; no FPSCR side effects.
 * Vector pointers address three writable floats. Internal game-thread ABI;
 * callers own pointer validity and renderer/sound lifetimes.
 */
#ifndef JP_NATIVE_SOUND_SPATIAL_H
#define JP_NATIVE_SOUND_SPATIAL_H
#include <stdint.h>
#define JP_SPATIAL_API __declspec(dllexport)
JP_SPATIAL_API void matrix_calc_point(void *point);
JP_SPATIAL_API float njUnitVector(void *vector);
JP_SPATIAL_API float jp_qsound_position_side(float x);
JP_SPATIAL_API int32_t jp_qsound_position_channel(float side);
#endif
