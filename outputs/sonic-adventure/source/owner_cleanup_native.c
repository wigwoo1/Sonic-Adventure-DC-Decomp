#include "fpu_cleanup_transfer.h"
#include "owner_helper_prepare.h"
#include "owner_helper_terminal.h"
#include "heap_free_ram.h"
#include "helper_record_release.h"
typedef uint32_t (*sa_helper)(void *,uint32_t,uint32_t *);
uint32_t sa_owner_cleanup_with_vector(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t,uint32_t,void *,sa_helper,int *);
static uint32_t release(void *ram,uint32_t target,uint32_t argc,uint32_t a,uint32_t b,uint32_t incoming,uint32_t *sr){(void)argc;(void)b;if(target==0x8c07a49cu)return sa_helper_record_release_8c07a49c(ram,sr,a);return sa_heap_free_ram_8c098b42(ram,sr,a,incoming);}
struct context{uint8_t *ram;struct sa_fpu_banks *f;};
static uint32_t terminal(void *v,uint32_t config,uint32_t *sr){struct context *c=v;uint8_t *ram=c->ram;c->f->bank[(c->f->fpscr>>21)&1u][4]=0xbf800000u;struct sa_owner_helper_selection selection;sa_owner_helper_prepare(ram,sr,config,&selection);return sa_owner_helper_terminal(ram,sr,config,1526u,ram,release);}
/* Parent writes helper state14 before invocation; native terminal cleanup. */
SA_STAGE_EXPORT uint32_t sa_owner_cleanup_native(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *sr,uint32_t owner,uint32_t stack,int *status){struct context c={ram,f};return sa_owner_cleanup_with_vector(ram,f,fpul,sr,owner,stack,&c,terminal,status);}

SA_STAGE_EXPORT uint32_t sa_owner_helper_terminal_native(uint8_t *ram,struct sa_fpu_banks *f,uint32_t config,uint32_t *sr){struct context c={ram,f};return terminal(&c,config,sr);}
