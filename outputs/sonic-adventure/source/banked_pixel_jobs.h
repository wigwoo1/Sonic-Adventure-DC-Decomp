#ifndef SA_BANKED_PIXEL_JOBS_H
#define SA_BANKED_PIXEL_JOBS_H
#include "pixel_jobs.h"
/* The game bus retains bank-linear storage. Texture addresses use a separate
 * view, with alternating four-byte words from its two four-MiB banks. */
SA_STAGE_EXPORT int sa_pixel_vram64_view(const uint8_t *,uint32_t,uint8_t *,uint32_t);
SA_STAGE_EXPORT int sa_banked_pixel_job_execute(struct sa_pixel_job_state *,struct sa_render_completion *,const uint8_t *,uint32_t,uint8_t *,uint32_t,uint32_t *,uint32_t *,uint64_t,const struct sa_pixel_options *);
#endif
