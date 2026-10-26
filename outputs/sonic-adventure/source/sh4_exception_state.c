/* Authored architectural support; see SH-4 Software Manual (Renesas),
 * sections5.5/9.79 and SH7750 Hardware Manual section5. Software policies do
 * not claim physical interrupt timing, cache/MMU behavior or fault vectoring. */
#include "sh4_exception_state.h"
static int privileged(uint32_t sr,unsigned delay){
 return sr&0x40000000u?0:(delay?-5:-4);
}
int sa_sh4_exception_stc(const struct sa_sh4_exception_state *e,unsigned which,uint32_t sr,uint32_t *out,unsigned delay){
 if(!e||!out||which>SA_EXCEPTION_SGR)return -6;
 int status=privileged(sr,delay);if(status)return status;
 *out=which==SA_EXCEPTION_SSR?e->ssr:which==SA_EXCEPTION_SPC?e->spc:e->sgr;return 0;
}
int sa_sh4_exception_ldc(struct sa_sh4_exception_state *e,unsigned which,uint32_t value,uint32_t sr,unsigned delay){
 if(!e||which>SA_EXCEPTION_SPC)return -6;
 int status=privileged(sr,delay);if(status)return status;
 if(which==SA_EXCEPTION_SSR)e->ssr=value;else e->spc=value;
 return 0;
}
int sa_sh4_rte_begin(const struct sa_sh4_exception_state *e,uint32_t *sr,uint32_t r[16],struct sa_sh4_integer_extra *extra,uint32_t *target,unsigned delay){
 if(!e||!sr||!r||!extra||!target)return -6;
 if(delay)return -5;
 int status=privileged(*sr,0);if(status)return status;
 uint32_t destination=e->spc;
 if(sa_sh4_integer_bank(*sr)!=sa_sh4_integer_bank(e->ssr)){
  for(unsigned i=0;i<8;i++){uint32_t temporary=r[i];r[i]=extra->other[i];extra->other[i]=temporary;}
 }
 *sr=e->ssr;
 *target=destination;return 0;
}
int sa_sh4_interrupt_enter(struct sa_sh4_exception_state *e,uint32_t *sr,uint32_t r[16],struct sa_sh4_integer_extra *extra,uint32_t *pc,uint32_t vbr,uint32_t event,unsigned priority,unsigned delay){
 if(!e||!sr||!r||!extra||!pc||!priority||priority>15u||event&31u)return -6;
 if(delay||(*sr&0x10000000u)||((*sr>>4)&15u)>=priority)return 1;
 uint32_t saved=*sr;
 e->ssr=saved;e->spc=*pc;e->sgr=r[15];e->intevt=event;e->returned=0;
 if(!sa_sh4_integer_bank(saved)){
  for(unsigned i=0;i<8;i++){uint32_t temporary=r[i];r[i]=extra->other[i];extra->other[i]=temporary;}
 }
 *sr=saved|0x70000000u;*pc=vbr+0x600u;return 0;
}
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *out){
 struct sa_exception_bus *b=ctx;
 if(a==0xff000020u||a==0xff000024u||a==0xff000028u){
  if(n!=4u||!out)return -3;
  if(a==0xff000024u){*out=b->state->expevt;b->expevt_reads++;}
  else if(a==0xff000028u){*out=b->state->intevt;b->intevt_reads++;}
  else{*out=b->state->tra;b->tra_reads++;}
  return 0;
 }
 return b->parent.read?b->parent.read(b->parent.context,a,n,out):-2;
}
static int write(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct sa_exception_bus *b=ctx;
 if(a==0xff000020u||a==0xff000024u||a==0xff000028u)return -3;
 return b->parent.write?b->parent.write(b->parent.context,a,value,n):-2;
}
static int cache(void *ctx,uint32_t a,unsigned op){
 struct sa_exception_bus *b=ctx;
 return b->parent.cache?b->parent.cache(b->parent.context,a,op):-2;
}
int sa_exception_bus_init(struct sa_exception_bus *b,const struct sa_vertex_bus *parent,struct sa_sh4_exception_state *e,struct sa_vertex_bus *out){
 if(!b||!e||!out)return -6;
 b->parent=parent?*parent:(struct sa_vertex_bus){0};b->state=e;
 b->expevt_reads=b->intevt_reads=b->tra_reads=0;
 *out=(struct sa_vertex_bus){b,read,write,cache};return 0;
}
