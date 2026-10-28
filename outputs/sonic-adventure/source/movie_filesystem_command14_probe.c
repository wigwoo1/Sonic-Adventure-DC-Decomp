#include "flash_storage.h"
#include "transfer_runtime.h"
#include "sh4_exception_state.h"
#include "movie_clear_probe.h"
#include "native_console_service.h"
struct sa_disc_queue;
typedef void (*observer)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
int sa_movie_filesystem_command14_connected(uint8_t*,struct sa_clear_cpu*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t,void*,observer,const struct sa_vertex_bus*,uint32_t*,struct sa_sh4_integer_extra*,uint32_t*,struct sa_disc_queue*,uint32_t,struct sa_console_service*);
int sa_movie_filesystem_command14_exception_connected(uint8_t*,struct sa_clear_cpu*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t,void*,observer,const struct sa_vertex_bus*,uint32_t*,struct sa_sh4_integer_extra*,uint32_t*,struct sa_disc_queue*,uint32_t,struct sa_console_service*,struct sa_sh4_exception_state*);
int sa_movie_filesystem_command14_running_connected(uint8_t*,struct sa_clear_cpu*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t,void*,observer,const struct sa_vertex_bus*,uint32_t*,struct sa_sh4_integer_extra*,uint32_t*,struct sa_disc_queue*,uint32_t,struct sa_console_service*,struct sa_sh4_exception_state*,struct sa_transfer_runtime*);
int sa_movie_filesystem_command14_storage_connected(uint8_t*,struct sa_clear_cpu*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t,void*,observer,const struct sa_vertex_bus*,uint32_t*,struct sa_sh4_integer_extra*,uint32_t*,struct sa_disc_queue*,uint32_t,struct sa_console_service*,struct sa_sh4_exception_state*,struct sa_transfer_runtime*,struct sa_flash_storage*);
struct probe {struct sa_clear_stats *stats;observer observe;void *user;const struct sa_vertex_bus *bus;uint32_t failure,pending;struct sa_fpu_banks initial;uint32_t initialized,*fpul,initial_fpul;};
static uint64_t fold(uint64_t h,uint32_t word){return (h^word)*UINT64_C(1099511628211);}
static void init(struct sa_clear_stats *s){volatile uint8_t *p=(volatile uint8_t*)s;for(unsigned i=0;i<sizeof(*s);i++)p[i]=0;s->cpu_hash=s->write_hash=UINT64_C(14695981039346656037);}
static void boundary(struct sa_clear_stats *s,const uint32_t *r,uint32_t sr,uint32_t pr){
 for(unsigned i=0;i<16;i++){if(!s->calls)s->first[i]=r[i];s->last[i]=r[i];s->cpu_hash=fold(s->cpu_hash,r[i]);}
 if(!s->calls){s->first_sr=sr;s->first_pr=pr;}s->last_sr=sr;s->last_pr=pr;
 s->cpu_hash=fold(fold(s->cpu_hash,sr),pr);s->calls++;
}
static void written(struct sa_clear_stats *s,uint32_t a,uint32_t value){s->write_hash=fold(fold(fold(s->write_hash,a),value),4);s->writes++;}
static void observe(void *ctx,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct probe *p=ctx;
 if(target==0x8c65253eu&&*pr==0x8c665994u){
  boundary(p->stats,r,*sr,*pr);p->pending=1;
  uint32_t k=(uint32_t)(p->stats->calls-1u);
  for(unsigned i=0;i<16;i++){
   uint32_t expected=p->stats->first[i];
   if(i==0u&&k)expected=0;
   if(i==4u||i==14u)expected=0xa5000000u+k*4u;
   if(i==5u)expected=p->stats->first[15];
   if(i==6u)expected=4u;
   if(i==13u)expected=k;
   if(r[i]!=expected)p->stats->code_errors++;
  }
  if(*pr!=0x8c665994u||*sr!=(k?(p->stats->first_sr&~1u):p->stats->first_sr)||r[11]!=target||r[12]!=0x200000u)p->stats->code_errors++;
  if(!p->initialized){
   volatile uint32_t *out=(volatile uint32_t*)&p->initial;const uint32_t *in=(const uint32_t*)f;
   for(unsigned i=0;i<sizeof(*f)/4u;i++)out[i]=in[i];
   p->initial_fpul=*p->fpul;p->initialized=1;
  }
  for(unsigned b=0;b<2;b++)for(unsigned i=0;i<16;i++)if(p->initial.bank[b][i]!=f->bank[b][i])p->stats->fpu_errors++;
  if(p->initial.fpscr!=f->fpscr||p->initial_fpul!=*p->fpul)p->stats->fpu_errors++;
  return;
 }
 if(p->observe)p->observe(p->user,target,m,f,sr,r,pr);
}
static int read(void *ctx,uint32_t a,unsigned n,uint32_t *out){struct probe *p=ctx;return p->bus->read(p->bus->context,a,n,out);}
static int write(void *ctx,uint32_t a,uint32_t value,unsigned n){
 struct probe *p=ctx;
 if(p->pending){
  p->pending=0;p->stats->attempts++;
  if(p->stats->writes==p->failure)return -3;
  int rc=p->bus->write(p->bus->context,a,value,n);if(rc)return rc;
  if(n!=4u||value!=0u||a!=0xa5000000u+(uint32_t)(p->stats->writes*4u))p->stats->code_errors++;
  written(p->stats,a,value);return 0;
 }
 return p->bus->write(p->bus->context,a,value,n);
}
static int cache(void *ctx,uint32_t a,unsigned op){struct probe *p=ctx;return p->bus->cache(p->bus->context,a,op);}
SA_STAGE_EXPORT int sa_movie_filesystem_command14_observed(uint8_t *m,struct sa_clear_cpu *s,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *macl,uint32_t entry,void *user,observer callback,const struct sa_vertex_bus *bus,uint32_t *gbr,struct sa_sh4_integer_extra *integer,uint32_t *vbr,struct sa_disc_queue *disc,uint32_t firmware,struct sa_console_service *console,struct sa_clear_stats *stats,uint32_t failure){
 init(stats);struct probe p;p.stats=stats;p.observe=callback;p.user=user;p.bus=bus;p.failure=failure;p.pending=0;p.initialized=0;p.fpul=fpul;
 struct sa_vertex_bus decorated={&p,read,write,cache};
 return sa_movie_filesystem_command14_connected(m,s,f,fpul,macl,entry,&p,observe,bus?&decorated:0,gbr,integer,vbr,disc,firmware,console);
}
/* Independent, verification-only decoder. Execute every loop and helper
 * instruction, including delay slots. No loop reduction or bulk VRAM fill. */
SA_STAGE_EXPORT int sa_clear_reference(uint8_t *ram,uint8_t *vram,struct sa_clear_cpu *s,struct sa_clear_stats *stats,uint32_t failure){
 uint32_t pc=s->pc,delayed=0;init(stats);
 while(pc!=0x8c66599cu){
  if(stats->steps++>=40000000u)return -9;
  unsigned bit;
  if(pc>=0x8c66598cu&&pc<=0x8c66599au)bit=(pc-0x8c66598cu)/2;
  else if(pc>=0x8c65253eu&&pc<=0x8c65254cu)bit=8+(pc-0x8c65253eu)/2;
  else {s->pc=pc;stats->code_errors++;return -9;}
  stats->coverage|=UINT64_C(1)<<bit;
  if(pc==0x8c65253eu)boundary(stats,s->r,s->sr,s->pr);
  uint32_t off=pc&0xffffffu;uint16_t w=(uint16_t)(ram[off]|(uint16_t)ram[off+1]<<8);
  unsigned n=(w>>8)&15u,m=(w>>4)&15u;uint32_t next=delayed?delayed:pc+2u;delayed=0;
  if((w&0xf00fu)==0x6003u)s->r[n]=s->r[m];
  else if((w&0xf000u)==0xe000u)s->r[n]=(uint32_t)(int32_t)(int8_t)w;
  else if((w&0xf0ffu)==0x400bu){s->pr=pc+4u;delayed=s->r[n];}
  else if((w&0xf000u)==0x7000u)s->r[n]+=(uint32_t)(int32_t)(int8_t)w;
  else if((w&0xf00fu)==0x3003u)s->sr=(s->sr&~1u)|((int32_t)s->r[n]>=(int32_t)s->r[m]);
  else if((w&0xff00u)==0x8f00u){if(!(s->sr&1u))delayed=pc+4u+(uint32_t)((int32_t)(int8_t)w*2);}
  else if((w&0xf0ffu)==0x4009u)s->r[n]>>=2;
  else if((w&0xf00fu)==0x6006u){
   uint32_t a=s->r[m];if((a&0x1f000000u)!=0x0c000000u||(a&0xffffffu)>0xfffffcu){s->pc=pc;return -3;}
   uint32_t value=0;for(unsigned b=0;b<4;b++)value|=(uint32_t)ram[(a&0xffffffu)+b]<<(8*b);
   s->r[n]=value;if(n!=m)s->r[m]+=4u;
  }
  else if((w&0xf0ffu)==0x4010u){s->r[n]--;s->sr=(s->sr&~1u)|!s->r[n];}
  else if((w&0xf00fu)==0x2002u){
   uint32_t a=s->r[n],value=s->r[m];stats->attempts++;
   if(stats->writes==failure){s->pc=pc;return -3;}
   if((a&3u)||(a&0x1fffffffu)<0x05000000u||(a&0x1fffffffu)>=0x05800000u){s->pc=pc;return -3;}
   for(unsigned b=0;b<4;b++)vram[(a&0x1fffffffu)-0x05000000u+b]=(uint8_t)(value>>(8*b));
   written(stats,a,value);
  }
  else if(w==0x000bu)delayed=s->pr;
  else if(w!=0x0009u){s->pc=pc;stats->code_errors++;return -9;}
  pc=next;
 }
 s->pc=pc;return 0;
}

SA_STAGE_EXPORT int sa_movie_filesystem_command14_exception_observed(uint8_t *m,struct sa_clear_cpu *s,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *macl,uint32_t entry,void *user,observer callback,const struct sa_vertex_bus *bus,uint32_t *gbr,struct sa_sh4_integer_extra *integer,uint32_t *vbr,struct sa_disc_queue *disc,uint32_t firmware,struct sa_console_service *console,struct sa_clear_stats *stats,uint32_t failure,struct sa_sh4_exception_state *exception){
 init(stats);struct probe p;p.stats=stats;p.observe=callback;p.user=user;p.bus=bus;p.failure=failure;p.pending=0;p.initialized=0;p.fpul=fpul;
 struct sa_vertex_bus decorated={&p,read,write,cache};
 return sa_movie_filesystem_command14_exception_connected(m,s,f,fpul,macl,entry,&p,observe,bus?&decorated:0,gbr,integer,vbr,disc,firmware,console,exception);
}

SA_STAGE_EXPORT int sa_movie_filesystem_command14_running_observed(uint8_t *m,struct sa_clear_cpu *s,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *macl,uint32_t entry,void *user,observer callback,const struct sa_vertex_bus *bus,uint32_t *gbr,struct sa_sh4_integer_extra *integer,uint32_t *vbr,struct sa_disc_queue *disc,uint32_t firmware,struct sa_console_service *console,struct sa_clear_stats *stats,uint32_t failure,struct sa_sh4_exception_state *exception,struct sa_transfer_runtime *runtime){
 init(stats);struct probe p;p.stats=stats;p.observe=callback;p.user=user;p.bus=bus;p.failure=failure;p.pending=0;p.initialized=0;p.fpul=fpul;
 struct sa_vertex_bus decorated={&p,read,write,cache};
 return sa_movie_filesystem_command14_running_connected(m,s,f,fpul,macl,entry,&p,observe,bus?&decorated:0,gbr,integer,vbr,disc,firmware,console,exception,runtime);
}

SA_STAGE_EXPORT int sa_movie_filesystem_command14_storage_observed(uint8_t *m,struct sa_clear_cpu *s,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *macl,uint32_t entry,void *user,observer callback,const struct sa_vertex_bus *bus,uint32_t *gbr,struct sa_sh4_integer_extra *integer,uint32_t *vbr,struct sa_disc_queue *disc,uint32_t firmware,struct sa_console_service *console,struct sa_clear_stats *stats,uint32_t failure,struct sa_sh4_exception_state *exception,struct sa_transfer_runtime *runtime,struct sa_flash_storage *storage){
 init(stats);struct probe p;p.stats=stats;p.observe=callback;p.user=user;p.bus=bus;p.failure=failure;p.pending=0;p.initialized=0;p.fpul=fpul;
 struct sa_vertex_bus decorated={&p,read,write,cache};
 return sa_movie_filesystem_command14_storage_connected(m,s,f,fpul,macl,entry,&p,observe,bus?&decorated:0,gbr,integer,vbr,disc,firmware,console,exception,runtime,storage);
}
