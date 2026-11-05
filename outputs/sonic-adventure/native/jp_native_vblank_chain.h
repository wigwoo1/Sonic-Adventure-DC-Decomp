/* Authored native clock boundary for the original Shinobi vblank chain. */
#ifndef JP_NATIVE_VBLANK_CHAIN_H
#define JP_NATIVE_VBLANK_CHAIN_H
#include "jp_native_frame_wait.h"
#include <stddef.h>
typedef void (*jp_chain_callback)(void *);
enum jp_chain_error {JP_CHAIN_OK,JP_CHAIN_ARGUMENT,JP_CHAIN_NOT_INITIALIZED,
    JP_CHAIN_THREAD,JP_CHAIN_REENTRANT,JP_CHAIN_UNSUPPORTED,JP_CHAIN_FULL,
    JP_CHAIN_WAIT_ERROR};
enum {JP_WAIT_CHAIN_FULL=9};
struct jp_chain_record {uint32_t priority,next;uintptr_t callback,argument;};
/* One serialized game-thread owner, the existing rational clock, and the
 * original 255 usable slots. Event 0x900 is sourced by that logical clock.
 * Other CPU/DMA/hblank event sources fail explicitly until implemented.
 * Smaller priorities run first; equal priorities run newest first.
 * Deletion during dispatch, including self-deletion, is supported. Adding
 * or resetting during dispatch is rejected, preventing recycled live nodes.
 * Callers retain callback/argument lifetimes until removal or wait shutdown.
 */
JP_CLOCK_API unsigned long syChainAddHandler(short event,jp_chain_callback callback,
    unsigned long priority,void *argument);
JP_CLOCK_API void syChainDeleteHandler(unsigned long handle);
JP_CLOCK_API void syChainInit(void);
JP_CLOCK_API int jp_vblank_chain_status(void);
JP_CLOCK_API void jp_vblank_chain_clear_error(void);
JP_CLOCK_API uint32_t jp_vblank_chain_count(void);
JP_CLOCK_API uint32_t jp_vblank_chain_mask(void);
JP_CLOCK_API int jp_vblank_chain_describe(uint32_t,struct jp_chain_record *);
/* Arm the genuine njBlank routine; its first logical vblank wakes the real
 * startup counter wait. A completed render retires this handler on its next
 * tick. Repeated arming while active fails rather than orphaning a handler.
 */
JP_CLOCK_API int jp_vblank_arm_ninja_blank(void);
JP_CLOCK_API int jp_vblank_wait_ninja_blank(void);

/* Internal ABI to the pinned external SDK's private adapted chain object.
 * These functions contain original SDK code and are never public artifacts.
 */
unsigned long jp_chain_original_add(short,jp_chain_callback,unsigned long,void *);
void jp_chain_original_delete(unsigned long);
void jp_chain_original_init(void);
void jp_chain_original_vblank(void *);
uint32_t jp_chain_original_count(void);
int jp_chain_original_live(uint32_t);
int jp_chain_original_describe(uint32_t,struct jp_chain_record *);
uint32_t jp_ninja_blank_handle(void);
void jp_ninja_blank_bind(uint32_t);
void njBlank(void *);
void njSetEORflag(void *);
extern volatile int _nj_blank_count_;
uint16_t jp_chain_native_mask_get(void);
void jp_chain_native_mask_set(int);
void jp_chain_native_callback(jp_chain_callback,void *);
#endif
