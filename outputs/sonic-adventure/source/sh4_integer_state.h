#ifndef SA_SH4_INTEGER_STATE_H
#define SA_SH4_INTEGER_STATE_H
#include "sh4_signed_product.h"
/* r[0..7] is the bank selected by SR; other[0..7] is the opposite bank.
 * R8..R15 remain in the existing visible array. All SR changes use this API.
 * Faults preserve the faulting operation's input state. Exception-vector
 * delivery, interrupt acceptance and instruction timing belong to the caller.
 */
struct sa_sh4_integer_extra {uint32_t mach,other[8];};
enum {SA_SH4_PRIVILEGE_FAULT=-4,SA_SH4_SLOT_FAULT=-5,SA_SH4_BANK_ARGUMENT=-6};
SA_STAGE_EXPORT unsigned sa_sh4_integer_bank(uint32_t sr);
SA_STAGE_EXPORT int sa_sh4_ldc_sr(uint32_t value,uint32_t *sr,uint32_t r[16],struct sa_sh4_integer_extra *extra,unsigned delay_slot);
SA_STAGE_EXPORT int sa_sh4_stc_sr(uint32_t sr,uint32_t *out,unsigned delay_slot);
SA_STAGE_EXPORT int sa_sh4_ldc_bank(uint32_t value,unsigned bank_register,uint32_t sr,struct sa_sh4_integer_extra *extra,unsigned delay_slot);
SA_STAGE_EXPORT int sa_sh4_stc_bank(unsigned bank_register,uint32_t sr,const struct sa_sh4_integer_extra *extra,uint32_t *out,unsigned delay_slot);
SA_STAGE_EXPORT void sa_sh4_dmuls_state(uint32_t left,uint32_t right,uint32_t *macl,struct sa_sh4_integer_extra *extra);
#endif
