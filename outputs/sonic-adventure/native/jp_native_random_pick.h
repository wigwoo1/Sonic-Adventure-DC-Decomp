/* Numerical/argument boundary used by the genuine game shuffle body. */
#ifndef JP_NATIVE_RANDOM_PICK_H
#define JP_NATIVE_RANDOM_PICK_H
#include <stdint.h>
#include <stddef.h>
#define JP_RANDOM_API __declspec(dllexport)
JP_RANDOM_API uint32_t jp_random_positive_float_word(uint32_t integer);
JP_RANDOM_API int32_t jp_random_pick(uint32_t draw,int32_t left);
JP_RANDOM_API int jp_shuffle_arguments(const void *array,int count,int size);
JP_RANDOM_API int jp_shuffle_status(void);
#endif
