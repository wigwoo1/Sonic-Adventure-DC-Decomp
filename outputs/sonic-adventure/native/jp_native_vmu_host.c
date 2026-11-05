/* Host CRT only: do not put the Dreamcast SDK headers on this TU's path. */
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
int jp_vmu_debug_format(char *buffer,size_t capacity,const char *format,va_list args) {
    return vsnprintf(buffer,capacity,format,args);
}
