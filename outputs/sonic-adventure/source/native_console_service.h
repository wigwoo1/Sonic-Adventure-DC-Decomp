#ifndef SA_NATIVE_CONSOLE_SERVICE_H
#define SA_NATIVE_CONSOLE_SERVICE_H
#include <stdint.h>
#ifdef _WIN32
#define SA_CONSOLE_EXPORT __declspec(dllexport)
#else
#define SA_CONSOLE_EXPORT
#endif
#define SA_NATIVE_SYSINFO_TARGET UINT32_C(0x8c00f110)
#define SA_NATIVE_FLASH_TARGET UINT32_C(0x8c00f120)
#define SA_NATIVE_SYSINFO_ADDRESS UINT32_C(0x8c000068)
/* New host platform policy, not a decompiled BIOS. Single-threaded. Immutable
 * input spans are borrowed until reconfiguration; they must not alias guest RAM.
 * Guest RAM is a stable sixteen-MiB array. No default console identity/flash is
 * manufactured. Init copies the explicit 24-byte info input into guest RAM.
 * The pointer query requires that initialization. Read-only flash access returns
 * a byte count or guest -1 for an out-of-range request. Other opcodes stop.
 * Host errors leave RAM, counters, and output arguments unchanged. */
struct sa_console_service {
 const uint8_t *info,*flash;
 uint32_t info_size,flash_size,info_configured,flash_configured,initialized;
 uint64_t info_calls,flash_calls,flash_bytes;
};
SA_CONSOLE_EXPORT uint32_t sa_console_service_size(void);
SA_CONSOLE_EXPORT int sa_console_service_init(struct sa_console_service *);
SA_CONSOLE_EXPORT int sa_console_set_info(struct sa_console_service *,const uint8_t *,uint32_t);
SA_CONSOLE_EXPORT int sa_console_set_flash(struct sa_console_service *,const uint8_t *,uint32_t);
SA_CONSOLE_EXPORT int sa_console_bind_vectors(const struct sa_console_service *,uint8_t *);
/* channel 0: sysinfo selectors 0 and 3; channel 1: flash selector 1.
 * A void init leaves result untouched and sets write_result to zero.
 * Success with write_result one supplies the new R0 value in result. */
SA_CONSOLE_EXPORT int sa_console_service(struct sa_console_service *,uint8_t *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *,uint32_t *);
#endif
