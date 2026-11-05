#ifndef JP_NATIVE_LCD_H
#define JP_NATIVE_LCD_H
#include <stdint.h>
#include <stddef.h>
enum jp_lcd_error { JP_LCD_OK,JP_LCD_ARGUMENT,JP_LCD_NOT_INITIALIZED,
                    JP_LCD_THREAD,JP_LCD_STATE };
enum { JP_LCD_BYTES=192,JP_LCD_PIXELS=1536 };
struct jp_lcd_info { uint64_t accepted,presented; uint32_t connected,pending,
    root_direction,device_direction; };
/* Requires the real VMU store owner. Only enabled and actually attached cards
 * expose LCDs. Commands own their 192 input bytes until the next service pass;
 * another write to a pending endpoint is busy. No raw-card bytes are changed. */
__declspec(dllexport) int jp_lcd_initialize(uint32_t enabled_drives);
__declspec(dllexport) int jp_lcd_shutdown(void);
__declspec(dllexport) int jp_lcd_service(void);
__declspec(dllexport) int jp_lcd_connectors(uint32_t drive,uint32_t root,uint32_t device);
__declspec(dllexport) int jp_lcd_read(uint32_t drive,void *out,size_t capacity,struct jp_lcd_info *info);
#endif
