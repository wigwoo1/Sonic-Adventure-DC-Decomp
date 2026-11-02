/* Optional standalone DLL runtime. Link once; omit when the host supplies CRT. */
#include <stddef.h>
#if defined(_WIN32)
int _fltused = 0;
#endif
void *memcpy(void *destination, const void *source, size_t length)
{
    volatile unsigned char *out = destination;
    const volatile unsigned char *in = source;
    for (size_t i = 0; i < length; ++i) out[i] = in[i];
    return destination;
}
