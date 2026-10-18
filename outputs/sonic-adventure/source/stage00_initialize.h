#ifndef SA_STAGE00_INITIALIZE_H
#define SA_STAGE00_INITIALIZE_H
#include "stage00_timer.h"

/* This view covers only the fields touched by 0x0c900d00. */
struct sa_stage00_initialize_services {
 void *context;
 uint32_t (*query_variant)(void *context);
 void (*call_8c07c65e)(void *context,uint32_t r4,uint32_t r5);
 void (*call_8c07c676)(void *context,uint32_t r4,
  uint32_t fr4_bits,uint32_t fr5_bits,uint32_t fr6_bits);
 void (*call_8c60526a)(void *context,uint32_t r4,uint32_t r5,uint32_t r6);
 struct sa_stage00_task *(*call_8c09852e)(void *context,uint32_t r4,
  uint32_t r5,uint32_t callback_address);
 uint32_t (*call_8c014242)(void *context,uint32_t index,uint32_t variant);
 uint32_t (*call_8c014308)(void *context,uint32_t index,uint32_t variant);
 void (*call_8c08a07c)(void *context);
 void (*call_8c04c680)(void *context);
};

SA_STAGE_EXPORT void sa_stage00_initialize_0c900d00(
 struct sa_stage00_task *task,const struct sa_stage00_initialize_services *services);
#endif
