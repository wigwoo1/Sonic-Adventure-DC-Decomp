#ifndef JP_NATIVE_GDFS_INTERNAL_H
#define JP_NATIVE_GDFS_INTERNAL_H
#include "jp_native_gdfs.h"
struct jp_gdfs_allocation {void *base,*data;size_t bytes,reserved;};
uint32_t jp_gdfs_host_thread(void);
uint64_t jp_gdfs_host_time(void);
void jp_gdfs_host_yield(void);
int jp_gdfs_host_path(const wchar_t *);
const wchar_t *jp_gdfs_host_path_get(void);
int jp_gdfs_host_allocate(size_t,struct jp_gdfs_allocation *);
int jp_gdfs_host_free(struct jp_gdfs_allocation *);
/* Internal source boundary; no global CPU interrupt service is exported. */
long jp_gdfs_mask_get(void);
void jp_gdfs_mask_set(long);
int jp_gdfs_directory_valid(const unsigned char *);
int jp_gdfs_directory_begin(long,long,long);
int jp_gdfs_directory_step(void);
int jp_gdfs_expired(uint64_t);
long jp_gdfs_timeout(void *);
void jp_gdfs_note_tick(void);
void jp_gdfs_dispatch_callbacks(void);
#endif
