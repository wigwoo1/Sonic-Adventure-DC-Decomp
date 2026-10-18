#ifndef SA_DRAW_MOTION_STOP_H
#define SA_DRAW_MOTION_STOP_H
#include <stdint.h>
/* Internal connected contexts begin with a uint32_t * publication field. */
static inline void sa_motion_publish_stop(void *ctx,uint32_t pc){
 if(ctx){uint32_t *stop=*(uint32_t **)ctx;if(stop)*stop=pc;}
}
#endif
