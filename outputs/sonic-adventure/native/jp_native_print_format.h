/* Authored native argument/buffer boundary for the external SHC formatter.
 * Native Windows varargs, fixed single RZ/DN1 value policy. One game thread.
 */
#ifndef JP_NATIVE_PRINT_FORMAT_H
#define JP_NATIVE_PRINT_FORMAT_H
#include <stddef.h>
#include <stdint.h>
typedef __builtin_va_list jp_print_va_list;
#define JP_FORMAT_API __declspec(dllexport)
enum jp_format_error {JP_FORMAT_OK,JP_FORMAT_ARGUMENT,JP_FORMAT_STATE,
                     JP_FORMAT_BOUNDS,JP_FORMAT_DIRECTIVE};
JP_FORMAT_API int jp_print_format_status(void);
JP_FORMAT_API int jp_print_format(char *,size_t,const char *,...);
JP_FORMAT_API int jp_print_vformat(char *,size_t,const char *,jp_print_va_list);
JP_FORMAT_API uint64_t jp_print_format_widen_word(uint32_t);
JP_FORMAT_API uint32_t jp_print_format_narrow_word(uint64_t);
JP_FORMAT_API int jp_print_format_scaled_exponent(int);
double jp_print_format_argument(double);
size_t jp_print_format_strlen(const char *);
int jp_print_format_span(const void *,int);
int jp_print_format_failed(void);
int jp_print_fixed_arguments(float *,int);
#endif
