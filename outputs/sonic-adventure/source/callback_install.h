#ifndef SA_CALLBACK_INSTALL_H
#define SA_CALLBACK_INSTALL_H
#include "stage00_task.h"
SA_STAGE_EXPORT void sa_callback_default_8c6411b0(uint8_t *,uint32_t *,uint32_t,uint32_t);
SA_STAGE_EXPORT uint32_t sa_driver_enable_8c658c20(uint32_t);
SA_STAGE_EXPORT uint32_t sa_driver_enable_8c658c24(uint32_t);
SA_STAGE_EXPORT uint32_t sa_secondary_callback_register_8c64137c(uint8_t *,uint32_t *,uint32_t,uint32_t);
SA_STAGE_EXPORT uint32_t sa_vblank_callback_register_8c64135a(uint8_t *,uint32_t *,uint32_t,uint32_t);
SA_STAGE_EXPORT void sa_callback_install_connected(uint8_t *,uint32_t *,uint32_t,
 uint32_t,uint32_t,uint32_t);
SA_STAGE_EXPORT void sa_callback_install_8c6412c2(uint8_t *,uint32_t *,uint32_t,
 uint32_t,uint32_t,uint32_t,void *,void (*)(void *,uint32_t,uint32_t,uint32_t *));
SA_STAGE_EXPORT int sa_callback_register_primary_8c64133e(uint8_t *,uint32_t *,
 uint32_t,uint32_t,void *,void (*)(void *,uint32_t,uint32_t,uint32_t *));
SA_STAGE_EXPORT int sa_callback_register_index3_8c6415f2(uint8_t *,uint32_t *,
 uint32_t,uint32_t,void *,void (*)(void *,uint32_t,uint32_t,uint32_t *));
SA_STAGE_EXPORT int sa_callback_register_index5_8c64162a(uint8_t *,uint32_t *,
 uint32_t,uint32_t,void *,void (*)(void *,uint32_t,uint32_t,uint32_t *));
SA_STAGE_EXPORT int sa_callback_register_index8_8c64167e(uint8_t *,uint32_t *,
 uint32_t,uint32_t,void *,void (*)(void *,uint32_t,uint32_t,uint32_t *));
#endif
