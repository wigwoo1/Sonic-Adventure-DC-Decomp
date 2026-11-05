#ifndef JP_NATIVE_SDK_HEAP_INTERNAL_H
#define JP_NATIVE_SDK_HEAP_INTERNAL_H
#include <stdint.h>
struct jp_sdk_heap_record {uint32_t units,requested;};
uint16_t jp_sdk_heap_mask_get(void);
void jp_sdk_heap_mask_set(uint16_t);
int jp_sdk_source_validate(uintptr_t,uint32_t,const struct jp_sdk_heap_record *);
uint32_t jp_sdk_source_units(const void *);
uint32_t jp_sdk_source_header_bytes(void);
void jp_sdk_source_reset(void);
#ifdef JP_SDK_HEAP_HOST
void jp_sdk_source_init(void *,unsigned long);
void jp_sdk_source_stat(unsigned long *,unsigned long *);
void *jp_sdk_source_syMalloc(unsigned long);
void jp_sdk_source_syFree(void *);
void *jp_sdk_source_calloc(size_t,size_t);
#endif
#endif
