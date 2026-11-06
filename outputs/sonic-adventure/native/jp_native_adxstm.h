#ifndef JP_NATIVE_ADXSTM_H
#define JP_NATIVE_ADXSTM_H
#include <stdint.h>
#ifndef JP_ADXSTM_API
#define JP_ADXSTM_API __declspec(dllexport)
#endif
struct jp_adxstm_info {
    uint64_t initializations,shutdowns,service_calls;
    uint32_t active,stopping,live_streams,pending_reads,status;
    int32_t native_error;
};
/* Same creator thread, sound lock, System/GDFS and elapsed-time service as RNA.
 * Returns jp_sound_error codes. Original ADXSTM calls remain internal and require
 * valid caller-owned joints/buffers. Stop the caller's decoder before stopping
 * its file stream: original Stop resets that input joint. Buffers/joints must
 * outlive successful stream retirement. No second clock or output client.
 * Failed cleanup retains ownership and must be retried before GDFS disposal.
 */
JP_ADXSTM_API int jp_adxstm_initialize(void);
JP_ADXSTM_API int jp_adxstm_service(void);
JP_ADXSTM_API int jp_adxstm_shutdown(void);
JP_ADXSTM_API int jp_adxstm_snapshot(struct jp_adxstm_info *);
#endif
