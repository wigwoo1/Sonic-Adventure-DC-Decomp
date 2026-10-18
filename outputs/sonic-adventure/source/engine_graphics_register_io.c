/* Retail SH-4 primitives653440/653444/653476. Explicit register state and MMIO. */
#include "fpu_cleanup_transfer.h"
#include "input_io.h"
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,const struct sa_input_io *io,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)
#define UNUSED (void)m;(void)f;(void)sr;(void)r;(void)pr;(void)fpul;(void)macl;(void)io;(void)ctx;(void)call
static uint32_t rd(uint8_t *m,uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
SA_STAGE_EXPORT int sa_engine_graphics_initialize_leaf(ARGS){UNUSED;return 0;}
SA_STAGE_EXPORT int sa_engine_graphics_register_write(ARGS){UNUSED;if(!io||!io->write)return -8;r[3]=rd(m,0x8c6534c0u);r[4]+=r[3];io->write(io->user,r[4],r[5],4);r[0]=1;return 0;}
SA_STAGE_EXPORT int sa_engine_graphics_register_read(ARGS){UNUSED;if(!io||!io->read)return -8;r[0]=rd(m,0x8c6534c0u);r[0]=io->read(io->user,r[0]+r[4],4);return 0;}
