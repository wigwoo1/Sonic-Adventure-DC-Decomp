#include "frame_countdown.h"
/* Retail globals 8c754ebc, 8c754ec0, 8c754ec4, 8c754ec8.
 * This configuration routine does not implement the interrupt-driven tick. */
void sa_frame_countdown_configure_8c051760(struct sa_frame_countdown *state,
 uint32_t ticks,uint32_t limit,const struct sa_frame_countdown_services *services)
{
 state->initial_ticks=ticks;
 state->current_ticks=ticks;
 state->iteration_limit=limit;
 state->iteration_index=0;
 services->register_8c605234(services->context,0x8c051810u);
 services->finish_8c055c44(services->context);
}
/* Table uses a 32-byte row stride, with an unmasked 8-bit column.
 * Do not constrain the column to eight: retail indexing permits overlap. */
void sa_frame_countdown_tick_8c051810(struct sa_frame_countdown *state,
 const uint32_t *mode,const uint32_t *table,
 const struct sa_frame_tick_services *services)
{
 state->current_ticks -= 1u;
 if (state->current_ticks == 0u) {
  uint32_t variant=services->query_8c04f7e0(services->context);
  uint32_t current_mode=*mode;
  if (current_mode==4u || current_mode==5u || current_mode==9u)
   services->dispatch(services->context,
    table[((variant>>8)&255u)*8u+(variant&255u)]);
  services->zero_8c643f98(services->context);
 }
 services->every_8c602cb8(services->context);
 services->finish_8c09ce40(services->context);
}
void sa_countdown_register_8c605234(uint32_t *handle,uint32_t callback,
 const struct sa_countdown_registration_services *services)
{
 if (*handle != 0u) {
  services->remove_8c645c54(services->context,*handle);
  *handle=0u;
 }
 if (callback != 0u)
  *handle=services->register_8c64594c(services->context,0x900u,callback,0x58u,0u);
}
