#ifndef SA_G2_TRANSFER_HOST_H
#define SA_G2_TRANSFER_HOST_H
#include <stdint.h>
#include "fpu_cleanup_transfer.h"
struct sa_g2_transfer {uint32_t channel,g2_address,root_address,length,direction;};
struct sa_g2_bus {void *user;int (*read)(void *,uint32_t,uint8_t *,uint32_t);int (*write)(void *,uint32_t,const uint8_t *,uint32_t);};
SA_STAGE_EXPORT int sa_g2_transfer_execute(const struct sa_g2_transfer *,const struct sa_g2_bus *,uint32_t *,uint32_t *);
SA_STAGE_EXPORT int sa_g2_transfer_capture(uint32_t,const uint32_t *,struct sa_g2_transfer *);
#endif

