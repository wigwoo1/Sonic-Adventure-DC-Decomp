/* Bounded native PVM memory loading. Source and writable output capacities are
 * supplied by the host; game assets remain local inputs. Serialized game thread. */
#ifndef JP_NATIVE_PVM_H
#define JP_NATIVE_PVM_H
#include "jp_native_texture_upload.h"
struct jp_pvm_header { Uint32 size; Uint16 status; Sint16 count; };
/* filename_bytes is the real capacity of EACH nonnull name.filename buffer.
 * A PVM containing names requires at least PVM_NAME_SIZE bytes per entry. */
JP_TEXTURE_API int jp_pvm_bind_destination(NJS_TEXLIST *list,uint32_t capacity,size_t filename_bytes);
JP_TEXTURE_API int jp_pvm_unbind_destination(void);
JP_TEXTURE_API int jp_pvm_destination_active(void);
/* Returns SDK 1/-1. Invalid archives preserve names/list/palette. Allocation
 * failures may retain successfully loaded entries; clear errors deliberately
 * and call release_loaded before retrying. No cache/file provider is implied. */
JP_TEXTURE_API int jp_pvm_load_memory(NJS_TEXLIST *list,uint32_t capacity,size_t filename_bytes,const void *data,size_t bytes);
JP_TEXTURE_API uint32_t jp_pvm_loaded_count(const NJS_TEXLIST *list);
JP_TEXTURE_API int jp_pvm_release_loaded(NJS_TEXLIST *list);
/* Private original SDK metadata helpers use these bounded cursor providers. */
void njMemOpen(Uint8 *addr);
void njMemRead(void *out,Sint32 bytes);
void njMemFseek(Sint32 offset,Sint32 origin);
Sint32 njCalcPvmStatusSize(Uint16 status);
void njSetTexListToPvmM(NJS_TEXLIST *list,Sint32 stride,struct jp_pvm_header *header);
Sint32 njSearchEntryCount(NJS_TEXLIST *list,Uint32 number);
#endif
