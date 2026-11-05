#ifndef JP_NATIVE_G2_AUDIO_H
#define JP_NATIVE_G2_AUDIO_H
#include <stdint.h>
#ifndef JP_G2_API
#define JP_G2_API __declspec(dllexport)
#endif
enum jp_g2_error { JP_G2_OK, JP_G2_ARGUMENT, JP_G2_STATE, JP_G2_THREAD,
    JP_G2_MEMORY, JP_G2_CORE, JP_G2_BUSY, JP_G2_UNSUPPORTED };
#define JP_G2_SOUND_RAM UINT32_C(0xa0800000)
#define JP_G2_REGISTERS UINT32_C(0xa0700000)
typedef void (*jp_g2_callback)(void *);
struct jp_g2_info {
    uint64_t cpu_reads,cpu_writes,dma_started,dma_ended,callbacks,bytes_copied;
    uint32_t ready,status,core_status,creator_thread,channel_open,busy,
        in_callback,gate_level,configured,failures;
};
/* Borrows the creator thread's live jp_aica owner. Closing this transport never
 * closes the core. CPU copies support widths 1/2/4 and FIX/ADD/SUB (0/1/2).
 * Register access currently supports one aligned 32-bit SDK access per call.
 * Guest bus tokens remain 32-bit; host buffers and callback contexts do not.
 */
JP_G2_API int jp_g2_open(void);
JP_G2_API int jp_g2_close(void);
JP_G2_API int jp_g2_read(uint32_t bus,void *host,uint32_t width,int32_t count,
    uint32_t source_mode,uint32_t destination_mode);
JP_G2_API int jp_g2_write(const void *host,uint32_t bus,uint32_t width,
    int32_t count,uint32_t source_mode,uint32_t destination_mode);
JP_G2_API int jp_g2_dma_open(void **handle,uint32_t type);
/* Native DMA transport copies actual data at trigger and posts completion for
 * explicit creator-thread service. No SH-4 DMA timing/hardware is simulated.
 * DMA spans must be 32-byte aligned, rather than silently truncating pointers.
 * Only the AICA channel and CPU trigger are supported. SetPrm copies metadata;
 * the caller retains the source/destination buffer through trigger's return.
 */
JP_G2_API int jp_g2_dma_prepare(void *handle,uint32_t bus,void *host,int32_t bytes,
    uint32_t direction,uint32_t transfer_control,uint32_t suspend_mode,
    jp_g2_callback callback,void *context);
JP_G2_API int jp_g2_dma_trigger(void *handle,uint32_t trigger);
JP_G2_API int jp_g2_dma_busy(void *handle,uint32_t *busy);
JP_G2_API int jp_g2_service(uint32_t budget,uint32_t *completed);
/* SDK interrupt-mask adaptations use a real host critical section and defer
 * completion dispatch while held. These levels are a host callback gate, not
 * exports that claim to read/write an SH-4 processor status register.
 */
JP_G2_API int jp_g2_gate_get(void);
JP_G2_API int jp_g2_gate_set(int level);
JP_G2_API int jp_g2_snapshot(struct jp_g2_info *out);
#endif
