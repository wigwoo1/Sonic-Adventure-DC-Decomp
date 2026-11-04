/* Authored native-memory ABI. These are not on-disc pointer records. */
#ifndef JP_NINJA_POLYGON2D_H
#define JP_NINJA_POLYGON2D_H
#include <stdint.h>
#define JP_POLYGON_API __declspec(dllexport)
typedef struct { float x, y; } jp_point2;
typedef union { uint32_t color; struct { int16_t u, v; } tex; } jp_color2;
typedef struct { jp_point2 *p; jp_color2 *col, *tex; uint32_t num; } jp_point2col;
enum jp_present_kind {
    JP_PRESENT_MODEL, JP_PRESENT_SPRITE, JP_PRESENT_NJ_SPRITE,
    JP_PRESENT_POLYGON_2D, JP_PRESENT_TEXTURE, JP_PRESENT_TEXTURE_MEMLIST,
    JP_PRESENT_POLYGON, JP_PRESENT_SPRITE_2D
};
typedef int32_t (*jp_sprite_record_fn)(enum jp_present_kind, const void *, int32_t,
                                     int32_t, float, uint32_t);
JP_POLYGON_API extern jp_sprite_record_fn port_sprite_record;
JP_POLYGON_API extern float port_draw_aspect;
JP_POLYGON_API extern int32_t port_pillarbox;
extern float port_wide_anchor;
extern int32_t port_wide_cover;
JP_POLYGON_API float port_wide_x(float);
JP_POLYGON_API int32_t port_wide_squeezes(void);
JP_POLYGON_API int32_t port_wide_covers(const float *, int32_t, int32_t);
/* Valid pointers cover count elements. Fill: 3..32; outline: >=2, subject
 * to queue budget. Invalid arguments fail before replay recording or writes.
 * Queue errors retain the queue's sticky error contract. */
enum jp_polygon_error { JP_POLYGON_ARGUMENT = -1 };
JP_POLYGON_API int jp_polygon2d_draw(jp_point2col *, int32_t, float, uint32_t);
JP_POLYGON_API void njDrawPolygon2D(jp_point2col *, int32_t, float, uint32_t);
#endif
