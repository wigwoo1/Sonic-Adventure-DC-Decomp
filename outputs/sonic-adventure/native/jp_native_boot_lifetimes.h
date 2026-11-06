#ifndef JP_NATIVE_BOOT_LIFETIMES_H
#define JP_NATIVE_BOOT_LIFETIMES_H
#include <stdint.h>
#ifndef JP_BOOT_LIFE_API
#define JP_BOOT_LIFE_API __declspec(dllexport)
#endif
#define JP_EXIT_CARD UINT32_C(1048576)
#define JP_EXIT_PRINT UINT32_C(2097152)
#define JP_EXIT_MAIN_LIFETIMES (JP_EXIT_CARD|JP_EXIT_PRINT)
enum jp_boot_life_kind {JP_BOOT_CARD,JP_BOOT_PRINT};
enum jp_boot_life_error {JP_BOOT_LIFE_OK,JP_BOOT_LIFE_ARGUMENT,JP_BOOT_LIFE_STATE,
    JP_BOOT_LIFE_THREAD,JP_BOOT_LIFE_REENTRANT,JP_BOOT_LIFE_PROVIDER,JP_BOOT_LIFE_TIMEOUT};
struct jp_boot_life_info {
    uint64_t next_token,token,identity,generation;
    uint32_t terminal,closing,finished,status,creator_thread,native_error;
};
/* Claims actual existing card/font providers, never initializes replacements.
 * A successful nonterminal close may be cancelled for same-thread restart.
 * Terminal finish is irreversible. Failed close retains its token and actual
 * remaining resources. VMU media/LCD ownership is not transferred here. */
JP_BOOT_LIFE_API int jp_boot_lifetime_claim(uint32_t,uint64_t *);
JP_BOOT_LIFE_API int jp_boot_lifetime_cancel(uint32_t,uint64_t);
JP_BOOT_LIFE_API int jp_boot_lifetime_close(uint32_t,uint64_t,uint32_t);
JP_BOOT_LIFE_API int jp_boot_lifetime_finish(uint32_t,uint64_t,uint32_t);
JP_BOOT_LIFE_API int jp_boot_lifetime_can_close(uint32_t,uint64_t);
JP_BOOT_LIFE_API int jp_boot_lifetime_snapshot(uint32_t,struct jp_boot_life_info *);
/* Internal hooks linked in the SAME Frames owner as the genuine providers.
 * op0=resource destruction/replacement,1=whole texture-owner destruction,
 * 2=creation,3=drawing. The permission never performs the operation. */
int jp_boot_lifetime_guard(uint32_t,uint32_t);
void jp_boot_lifetime_created(uint32_t);
int jp_boot_card_active(void);
int jp_boot_rf_active(void);
int jp_boot_rf_retired(void);
int jp_boot_print_active(void);
int jp_boot_font_identity(uint64_t *);
#endif
