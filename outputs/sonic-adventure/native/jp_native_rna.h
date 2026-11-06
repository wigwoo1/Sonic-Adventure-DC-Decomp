#ifndef JP_NATIVE_RNA_H
#define JP_NATIVE_RNA_H
#include <stdint.h>
#ifndef JP_RNA_API
#define JP_RNA_API __declspec(dllexport)
#endif
struct jp_rna_info {
    uint64_t initializations,shutdowns,service_calls;
    uint32_t active,stopping,live_renderers,pending_transfers,status,clock_error;
};
/* These operations share the existing creator-thread sound owner, SDK, G2,
 * frame clock and output client. Return jp_sound_error codes. Original mwRna/
 * ADXRNA APIs remain internal: caller-owned joints and buffers must outlive
 * their renderer, and legacy errors use the original error callback.
 * Initialized renderers are serviced by the existing sound/elapsed-time path.
 * Shutdown retires actual copies and renderers before G2/SDK/core disposal.
 */
JP_RNA_API int jp_rna_initialize(void);
JP_RNA_API int jp_rna_service(void);
JP_RNA_API int jp_rna_shutdown(void);
JP_RNA_API int jp_rna_snapshot(struct jp_rna_info *);
#endif
