/* Native boundaries for the externally compiled original print state/body.
 * Font payload and the external SDK translation unit remain private. */
#ifndef JP_NATIVE_PRINT_TRANSPORT_H
#define JP_NATIVE_PRINT_TRANSPORT_H
#include "jp_native_queue_direct.h"
#include "jp_native_standalone_texture.h"
enum jp_print_error { JP_PRINT_OK,JP_PRINT_ARGUMENT,JP_PRINT_STATE,
                      JP_PRINT_ALLOCATION,JP_PRINT_TEXTURE,JP_PRINT_QUEUE };
JP_QUEUE_API int jp_print_status(void);
JP_QUEUE_API void jp_print_clear_error(void);
int jp_print_texture_arguments(const void *,uint32_t,uint32_t);
int jp_print_create_surface(NJS_TEXSURFACE *,uint32_t,uint32_t,uint32_t);
int jp_print_load_surface(NJS_TEXSURFACE *,const void *);
int jp_print_string_count(const char *);
int jp_print_digit_count(int,int);
uint32_t *jp_print_open(uint32_t);
int jp_print_character_room(const uint32_t *);
void jp_print_close(const uint32_t *,const uint32_t[4]);
int16_t jp_print_coordinate(int16_t,uint16_t);
int16_t jp_print_advance(int16_t,uint16_t);
#endif
