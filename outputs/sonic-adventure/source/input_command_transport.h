#ifndef SA_INPUT_COMMAND_TRANSPORT_H
#define SA_INPUT_COMMAND_TRANSPORT_H
#include <stdint.h>
#include "fpu_cleanup_transfer.h"
struct sa_input_transport_request {
 uint32_t port,command,destination,source,payload_words,response_address,packet_mode;
 const uint8_t *payload;
};
typedef int (*sa_input_transport_exchange)(void *,const struct sa_input_transport_request *,uint8_t *,uint32_t);
/* Exchange returns response byte count, or a negative error. Responses use the
 * backend's packet mode. A valid command list is checked before exchanges begin.
 * A later exchange failure leaves earlier delivered responses intact. */
SA_STAGE_EXPORT int sa_input_command_transport(uint8_t *,uint32_t,uint32_t,uint32_t,uint32_t,void *,sa_input_transport_exchange);
#endif
