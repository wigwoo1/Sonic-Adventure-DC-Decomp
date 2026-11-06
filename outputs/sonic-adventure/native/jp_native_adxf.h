#ifndef JP_NATIVE_ADXF_H
#define JP_NATIVE_ADXF_H
#include <stdint.h>
#ifndef JP_ADXF_API
#define JP_ADXF_API __declspec(dllexport)
#endif
struct jp_adxf_info {
    uint64_t initializations,shutdowns,service_calls,owned_bytes;
    uint32_t active,stopping,live_files,pending_reads,partition_roots,owned_nodes,status;
    int32_t native_error;
};
/* Shares the existing sound/System/GDFS/ADXSTM owner. Return jp_sound_error.
 * Owned partition work is guarded native storage; the original ADXF tables
 * remain authoritative. Rejected loads preserve the previous table. Original
 * raw SDK calls remain internal and require valid caller-owned buffers.
 * Complete shutdown retires real reads/joints before freeing table storage.
 */
JP_ADXF_API int jp_adxf_initialize(void);
JP_ADXF_API int jp_adxf_service(void);
JP_ADXF_API int jp_adxf_shutdown(void);
JP_ADXF_API int jp_adxf_snapshot(struct jp_adxf_info *);
JP_ADXF_API int jp_adxf_partition_load(uint32_t,const char *,uint32_t);
JP_ADXF_API int jp_adxf_partition_add(uint32_t,const char *,uint32_t);
JP_ADXF_API int jp_adxf_partition_unload(uint32_t);
#endif
