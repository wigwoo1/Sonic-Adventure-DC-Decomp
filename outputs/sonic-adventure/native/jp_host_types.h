#ifndef JP_HOST_TYPES_H
#define JP_HOST_TYPES_H
/* The SDK's non-GCC size_t follows the 32-bit Dreamcast C ABI. Native
 * Windows x64 needs its compiler's size_t for host memory interfaces.
 * This does not widen Uint32 or change any upstream source file. */
#ifndef SIZE_T_DEFINED
typedef __SIZE_TYPE__ size_t;
#define SIZE_T_DEFINED
#endif
_Static_assert(sizeof(size_t) == sizeof(void *), "native size_t must fit a pointer");
#endif
