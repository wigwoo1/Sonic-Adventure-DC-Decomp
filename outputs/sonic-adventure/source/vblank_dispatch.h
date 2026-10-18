#ifndef SA_VBLANK_DISPATCH_H
#define SA_VBLANK_DISPATCH_H
#include "stage00_task.h"
/* RAM view covers original 16 MiB Dreamcast RAM, with original address tokens.
 * Callback invocation translates code tokens through the caller's dispatcher. */
SA_STAGE_EXPORT void sa_vblank_dispatch_8c645b1e(uint8_t *,uint32_t *,void *,
 void (*)(void *,uint32_t,uint32_t,uint32_t *));
#endif
